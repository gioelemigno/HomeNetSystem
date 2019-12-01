
#include "HNSMQTTPublish.hpp"

#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "HNSDatabase.hpp"
#include "HNSAddressMap.hpp"


#include "../HNSCore/HNSError.h"

std::string prefix = std::string("/home/living_room/controls/");

//#include "/home/gio/Workspace/HomeNetSystem/HNSHub/lib/HNSCore/HNSError.h"
void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
static inline int splitTopic(std::string topic, std::string& device, std::string& command);
struct mosquitto *mosq = NULL;

res_t HNSMQTTPublish_publish(string topic, string payload){
  int res = mosquitto_publish(mosq, NULL, topic.c_str(), payload.length(), payload.c_str(), 0, 0);
  if(res != MOSQ_ERR_SUCCESS){
    perror("mosuitto publish");
    exit(EXIT_FAILURE);
  }
  return 0;
}

res_t HNSMQTTPublish_init(const char* host){

	int port = 1883;
	int keepalive = 60;
	bool clean_session = true;

  int res = 0;
  res = mosquitto_lib_init();
  if(res != MOSQ_ERR_SUCCESS){
    printf("ERRORE mosquitto init\n");
    exit(EXIT_FAILURE);
  }

  mosq = mosquitto_new(NULL, clean_session, NULL);
  if(mosq == NULL){
    perror("mosquitto_new");
    exit(EXIT_FAILURE);
  }

  mosquitto_message_callback_set(mosq, message_callback);

 // printf("mqtt: %d\n", (int)mosq);

  res = mosquitto_connect(mosq, host, port, keepalive);
  if(res != MOSQ_ERR_SUCCESS){
		perror("mosquitto_connect");
		exit(EXIT_FAILURE);
	}
  
  std::string sub_topic = prefix + "#";

  mosquitto_subscribe(mosq, NULL, sub_topic.c_str(), 0);

  res = mosquitto_loop_start(mosq);
  if(res!=MOSQ_ERR_SUCCESS){
    perror("mosquitto_loop_start");
		exit(EXIT_FAILURE);
  }
  return 0;
}

res_t HNSMQTTPublish_publishAll(){
  for(uint16_t i=0; i<HNSDatabase_deviceList_index; i++){
    HNSDatabase_entry entry = HNSDatabase_deviceList[i];
    if(entry.status==deviceStatus_deleted) continue;

    for(unsigned j=0; j<entry.device->number_topics; j++){
      HNSMQTTPublish_publish(entry.device->MQTT_message[j].topic,entry.device->MQTT_message[j].payload);
      //HNSMQTTPublish_publish(std::string("temp_0"), std::string("30.0C°"));
    }
  }
  return 0;
}

res_t HNSMQTTPublish_publishDevice(address_t address){

  if(address >= HNSDatabase_deviceList_index){
    error=ERR_INVALID_ADDRESS;
    return -1;
  }

  HNSDatabase_entry entry = HNSDatabase_deviceList[address];
  //thread safe
  //auto unlock end function
  //std::unique_lock<std::mutex> lck (entry.device->mtx);

  if(entry.status==deviceStatus_deleted) {
  	error =ERR_DELETED_ADDRESS;
    return -1;
  }
  
  //if(entry.device->MQTTUpdated) return -1; // device already published
  printf("HNSMQTT_Publish device: %d\n", address);

  for(unsigned j=0; j<entry.device->number_topics; j++){
    HNSMQTTPublish_publish(entry.device->MQTT_message[j].topic,entry.device->MQTT_message[j].payload);
      //HNSMQTTPublish_publish(std::string("temp_0"), std::string("30.0C°"));
    printf("\tUpload Topic: %s, Message: %s\n", entry.device->MQTT_message[j].topic.c_str(), entry.device->MQTT_message[j].payload.c_str());
  }
  //entry.device->MQTTUpdated=true;
  return 0;
}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{

	printf("got message '%.*s' for topic '%s'\n", message->payloadlen, (char*) message->payload, message->topic);

  std::string device;
  std::string command;

  int res =  splitTopic(std::string(message->topic), device, command);
  if(res==0){
    printf("DEVICE=%s\tCOMMAND=%s\n", device.c_str(), command.c_str());
  }

  address_t address=255;
  res_t res2 = HNSAddressMap_getAddress(device, &address);
  if(res2==-1){
    printf("errore get address\n");
   
  }

  HNSDatabase_mutex.lock();

printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>EEE\n");
  HNSDatabase_entry entry = HNSDatabase_deviceList[address];
  entry.device->runCommand(command);

  HNSDatabase_mutex.unlock();
}

static inline int splitTopic(std::string topic, std::string& device, std::string& command){
  std::string tmp = topic.substr(prefix.length());

  std::size_t pos_slash = tmp.find("/");
  if(pos_slash == std::string::npos) return -1;

  command = tmp.substr(pos_slash+1);
  device = tmp.substr(0, tmp.length()-command.length()-1);

  return 0;

}
/*

//https://gist.github.com/piccaso/f463f6ad134a8a2479e62e6e2349e7c0#file-lobmosq-c-L28
//  needs libmosquitto-dev
//  $ gcc -o libmosq libmosq.c -lmosquitto

#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void mosq_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	// Pring all log messages regardless of level.

  switch(level){
    //case MOSQ_LOG_DEBUG:
    //case MOSQ_LOG_INFO:
    //case MOSQ_LOG_NOTICE:
    case MOSQ_LOG_WARNING:
    case MOSQ_LOG_ERR: {
      printf("%i:%s\n", level, str);
    }
  }
}

struct mosquitto *mosq = NULL;
char *topic = NULL;
void mqtt_setup(){

	char *host = "localhost";
	int port = 1883;
	int keepalive = 60;
	bool clean_session = true;
  topic = "/testtopic";

  mosquitto_lib_init();
  mosq = mosquitto_new(NULL, clean_session, NULL);
  if(!mosq){
		fprintf(stderr, "Error: Out of memory.\n");
		exit(1);
	}

  mosquitto_log_callback_set(mosq, mosq_log_callback);

  if(mosquitto_connect(mosq, host, port, keepalive)){
		fprintf(stderr, "Unable to connect.\n");
		exit(1);
	}
  int loop = mosquitto_loop_start(mosq);
  if(loop != MOSQ_ERR_SUCCESS){
    fprintf(stderr, "Unable to start loop: %i\n", loop);
    exit(1);
  }
}

int mqtt_send(char *msg){
  return mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
}

int main(int argc, char *argv[])
{
  mqtt_setup();
  int i = -1000;
  char *buf = malloc(64);
  while(1){
    sprintf(buf,"i=%i",i++);
    int snd = mqtt_send(buf);
    if(snd != 0) printf("mqtt_send error=%i\n", snd);
    usleep(100000);
  }
}
*/
