#include "HNSLight.hpp"
#include "../../HNSCore/HNSMacro.h"
#include "../../HNSCore/HNSError.h"
#include "../../HNSCore/HNSRouting.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "../../HNSCommand/HNSLight.h"

HNSLight::HNSLight(address_t address, const char* MQTT_topic) : HNSDevice((uint16_t)HNSModel_HNSLight, address){
    this->setDeviceName("light_0");

    this->status=POWER_OFF;

    this->statusOn=std::string(MQTT_STATUS_ON);
    this->statusOff=std::string(MQTT_STATUS_OFF);

    this->number_topics = HNSLIGHT_NUM_TOPICS;
    this->MQTT_message = new MQTT_message_c[HNSLIGHT_NUM_TOPICS];

    MQTT_message[MQTT_STATUS] = MQTT_message_c(MQTT_topic, "err");  //-> room_name/Temperature
 
    updateMQTT_message();
}

res_t HNSLight::setStatus(uint8_t _status){
    /*  
    * MASTER:   size 0 | type SET_STATUS_ON
    *           size 0 | type SET_STATUS_OFF
    *
    * SLAVE:    size 1 | type ACK_SET_STATUS | data status0
    */

    packet_t packet;
    packet.recipient = this->address;
    packet.size=0;
    packet.type=(_status==POWER_ON)? SET_STATUS_ON:SET_STATUS_OFF;

    res_t res = HNSRouting_write(&packet);
    if(res==-1) return -1;

    res = HNSRouting_read(&packet);
    if(res==-1) {
                   printf("errore lettura\n");

      return -1;
    }
    switch(res){
        case 1:
            if(packet.type != ACK_SET_STATUS){
              printf("errore tipo\n");
                error=ERR_TYPE;
                return -1;
            }

            if(packet.data[0] != _status){
              printf("errore stato ricevuto \n");
              const char* stato_to_set = (_status==POWER_ON)?"POWER_ON":"POWER_OFF";
              const char* stato_read = (packet.data[0]==POWER_ON)?"POWER_ON":"POWER_OFF";

              printf("!!!!!!!!!set: %s\t read: %s\n", stato_to_set, stato_read);
                error=ERR_SET;
                return -1;
            }
        break;

        default:
            return -1;
        break;
    }

    return 0;
}
res_t HNSLight::runCommand(std::string command){

  printf("run command..\n");
  printf("command: %s\n", command.c_str());
  if(command.compare("toggle")==0){
    printf("   >>>>>>>>>>><   command ok\n");
    uint8_t status_to_set = (this->status==POWER_ON)?POWER_OFF:POWER_ON;
    if(this->setStatus(status_to_set)==-1) {
      printf("Errore setStatus\n");
      return -1;
    }
  }
  else {
    printf(">>>>>>>>>>>><COMANDO NON RICONOSCIUTO\n");
    return -1;
  }
  return 0;
}

res_t HNSLight::refresh(){

  packet_t packet;
  packet.recipient = this->address;
  packet.size=0;
  packet.type=READ_STATUS;

  res_t res = HNSRouting_write(&packet);
  if(res==-1) return -1;

  res = HNSRouting_read(&packet);
  if(res==-1) return -1;

/*
* MASTER:  size 0 | type READ_STATUS
*
* SLAVE:   size 1 | type SEND_STATUS | data status0
*/
  switch (res) {
    case 1: // size = 8

      if(packet.type != SEND_STATUS){
        error=ERR_TYPE;
        return -1;
      }

      this->status = packet.data[0];
      updateMQTT_message();

      return 0;
    break;

    default:
      return -1;
    break;
  }

  //printf("Error: no refresh for HNSDevice base class\n");
  return -1;
}

std::string HNSLight::toString(){
    std::string str = "Status:\t" + (status==POWER_ON)? statusOn: statusOff + "\n";
    return str;
}

void HNSLight::updateMQTT_message(){
    std::string str_status = (this->status==POWER_ON)?this->statusOn : this->statusOff;
    this->MQTT_message[MQTT_STATUS].payload= str_status;

    return;
}