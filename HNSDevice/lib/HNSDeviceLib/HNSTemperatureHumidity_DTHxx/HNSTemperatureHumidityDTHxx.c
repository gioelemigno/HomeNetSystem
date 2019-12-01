#include "HNSTemperatureHumidityDTHxx.h"
#include "../../HNSCore/HNSRouting.h"
#include "../../HNSCommand/HNSTemperatureHumiditySensor.h"


float temperature = -10.50;
float humidity = 10.80;

uint8_t HNSTemperatureHumidityDTHxx_init(){

}


/*
* MASTER:  size 0 | type READ_TEMP_HUM
*
* SLAVE:   size 8 | type SEND_TEMP_HUM | data temp3 ... temp0 hum3 .. hum0
* SLAVE:   size 16 | type SEND_TEMP_HUM | data temp7 ... temp0 hum7 .. hum0
*/
void HNSTemperatureHumidityDTHxx_netHandler(void){
  packet_t packet;
  res_t res = HNSRouting_read(&packet);
  if(res == -1){
    return;
  }

  uint32_t* pointer_raw;
  uint32_t raw;

  switch(res){
    case 0:
      if(packet.type==READ_TEMP_HUM){
        packet.recipient=HUB_ADDRESS;
        packet.size=8;
        packet.type=SEND_TEMP_HUM;

        pointer_raw = (uint32_t*) &temperature;
        raw = (uint32_t)(*pointer_raw);
        packet.data[3] = (uint8_t) raw;
        packet.data[2] = (uint8_t) (raw >> 8);
        packet.data[1] = (uint8_t) (raw >> 16);
        packet.data[0] = (uint8_t) (raw >> 24);

        pointer_raw = (uint32_t*) &humidity;
        raw = (uint32_t)(*pointer_raw);
        packet.data[7] = (uint8_t) raw;
        packet.data[6] = (uint8_t) (raw >> 8);
        packet.data[5] = (uint8_t) (raw >> 16);
        packet.data[4] = (uint8_t) (raw >> 24);

        res=HNSRouting_write(&packet);
        if(res==-1) return;

      }
    break;

    default:
    return;
    break;
  }
}
//  read temperature and humidity from dhtxx sensor
uint8_t HNSTemperatureHumidityDTHxx_routine(){

}

// return last temperature read
float HNSTemperatureHumidityDTHxx_getTemperature(){

}
//return last humidity read
float HNSTemperatureHumidityDTHxx_getHumidity(){

}
