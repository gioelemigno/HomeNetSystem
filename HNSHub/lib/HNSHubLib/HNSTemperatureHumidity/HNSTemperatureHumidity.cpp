#ifdef __linux__

#include "HNSTemperatureHumidity.hpp"
#include "../../HNSCore/HNSMacro.h"
#include "../../HNSCore/HNSError.h"
#include "../../HNSCommand/HNSTemperatureHumiditySensor.h"
#include "../../HNSCore/HNSRouting.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
/*
HNSTemperatureHumiditySensor::HNSTemperatureHumiditySensor(address_t address)
                : HNSDevice((uint16_t)HNSModel_HNSTemperatureHumiditySensor, address){
                  this->HNSTemperatureHumiditySensor
}
*/
void HNSTemperatureHumiditySensor::updateMQTT_message(){
  this->MQTT_message[MQTT_TEMP].payload= std::to_string(this->temperature)+ this->unitTemp;
  this->MQTT_message[MQTT_HUM].payload= std::to_string(this->humidity)+ this->unitHum;

  return;
}
HNSTemperatureHumiditySensor::HNSTemperatureHumiditySensor(address_t address, const char* MQTT_topic_temperature, const char* MQTT_topic_humidity)
                : HNSDevice((uint16_t)HNSModel_HNSTemperatureHumiditySensor, address){

  this->setDeviceName("temp_hum_0");
  
  temperature=0.0;
  humidity=0.0;

  this->unitTemp = std::string(" C°");
  this->unitHum = std::string(" %");

  this->number_topics = HNSTemperatureHumiditySensor_NUM_TOPICS;
  this->MQTT_message = new MQTT_message_c[HNSTemperatureHumiditySensor_NUM_TOPICS];

  
  //std::string tmp = std::to_string(temperature) + unitTemp;
  MQTT_message[MQTT_TEMP] = MQTT_message_c(MQTT_topic_temperature, "err");  //-> room_name/Temperature
 
  //tmp = std::to_string(humidity)+unitHum;
  MQTT_message[MQTT_HUM] = MQTT_message_c(MQTT_topic_humidity, "err");  //-> room_name/Temperature
  
  updateMQTT_message();
}
                                  
res_t HNSTemperatureHumiditySensor::refresh(){

//printf("popoopo\n");
  packet_t packet;
  packet.recipient = this->address;
  packet.size=0;
  packet.type=READ_TEMP_HUM;

  res_t res = HNSRouting_write(&packet);
  if(res==-1) return -1;

  res = HNSRouting_read(&packet);
  if(res==-1) return -1;


  uint32_t raw32;
  float tmpF;
  float* pointer_tmpF;
  /*
  * MASTER:  size 0 | type READ_TEMP_HUM
  *
  * SLAVE:   size 8 | type SEND_TEMP_HUM | data temp3 ... temp0 hum3 .. hum0
  * SLAVE:   size 16 | type SEND_TEMP_HUM | data temp7 ... temp0 hum7 .. hum0
  */
  switch (res) {
    case 8: // size = 8

      if(packet.type != SEND_TEMP_HUM){
        error=ERR_TYPE;
        return -1;
      }

      raw32 = packet.data[3];
      raw32 |= ((uint32_t)packet.data[2]) << 8;
      raw32 |= ((uint32_t)packet.data[1]) << 16;
      raw32 |= ((uint32_t)packet.data[0]) << 24;
      pointer_tmpF = (float*) &raw32;
      tmpF = *pointer_tmpF;
      this->temperature = (double) tmpF;

      raw32 = packet.data[7];
      raw32 |= ((uint32_t)packet.data[6]) << 8;
      raw32 |= ((uint32_t)packet.data[5]) << 16;
      raw32 |= ((uint32_t)packet.data[4]) << 24;
      pointer_tmpF = (float*) &raw32;
      tmpF = *pointer_tmpF;
      this->humidity = (double) tmpF;
      updateMQTT_message();
/*
      raw32 = (uint32_t*) &packet.data[0];
      tmpF = (float) (*raw32);
      this->temperature = (double) tmpF;

      raw32 = (uint32_t*) &packet.data[4];
      tmpF=(float)(*raw32);
      this->humidity=(double)tmpF;
      */
      return 0;
    break;

    default:
      return -1;
    break;
  }

  //printf("Error: no refresh for HNSDevice base class\n");
  return -1;
}

std::string HNSTemperatureHumiditySensor::toString(){
  std::string str = "Temperature:\t" + std::to_string(this->temperature) + "\n";
  str += "Humidity:\t" + std::to_string(this->humidity) + "\n";

  return str;
}
#endif /*__linux__*/
