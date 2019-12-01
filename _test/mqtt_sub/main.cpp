#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <mosquitto.h>
#include <stdlib.h>


void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	bool match = 0;
	printf("got message '%.*s' for topic '%s'\n", message->payloadlen, (char*) message->payload, message->topic);

	mosquitto_topic_matches_sub("light_0/controls/ON", message->topic, &match);
	if (match) {
		printf("got message for ON topic\n");
	}
  else printf("not match recived %s\n", message->topic);

}
struct mosquitto *mosq = NULL;

int main(int argc, char *argv[])
{
    const char* host = "192.168.12.187";
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

    mosquitto_subscribe(mosq, NULL, "light_0/controls/#", 0);

  res = mosquitto_loop_start(mosq);
  if(res!=MOSQ_ERR_SUCCESS){
    perror("mosquitto_loop_start");
		exit(EXIT_FAILURE);
  }

    while(1);

}