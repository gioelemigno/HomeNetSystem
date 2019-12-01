#ifndef HNSLIGHT_HPP_
#define HNSLIGHT_HPP_
#include "../../HNSHub/HNSDevice_class.hpp"
#include "../../HNSCommand/HNSLight.h"

#define HNSLIGHT_NUM_TOPICS  1
#define MQTT_STATUS   0

#define MQTT_STATUS_ON    "ON"
#define MQTT_STATUS_OFF   "OFF"

#define POWER_ON           STATUS_POWER_ON
#define POWER_OFF          STATUS_POWER_OFF

#include <stdint.h>

class HNSLight : public HNSDevice{
  public:
    HNSLight(address_t address, const char* MQTT_topic);

    HNSLight(address_t address) : HNSLight(address, "/home/living_room/sensors/light_0"){}

    //Override
    virtual res_t refresh();    //refresh data
    virtual std::string toString();
    virtual res_t runCommand(std::string command);

    uint8_t getStatus(){return this->status;}
    res_t setStatus(uint8_t status);
    void updateMQTT_message();

  private:
    uint8_t status;

    std::string statusOn;
    std::string statusOff;
};






#endif/*HNSLIGHT_HPP_*/