#ifndef HNSDEVICE_CLASS_HPP_
#define HNSDEVICE_CLASS_HPP_

#ifdef __linux__

#include "../HNSCore/HNSError.h"
#include "../HNSCore/HNSMacro.h"
#include "../HNSCore/HNSRouting.h"

#include <stdint.h>
#include <string>
#include <mutex>          // std::mutex, std::unique_lock
#include <stdbool.h>
/*
typedef struct MQTT_message{
  std::string topic = "";
  std::string payload = "";
}MQTT_message_t;
*/
class MQTT_message_c{
  public:
      std::string topic;
      std::string payload;

      MQTT_message_c(const char* topic, const char* payload){
        this->topic = std::string(topic);
        this->payload = std::string(payload);
      }
      MQTT_message_c(){}

};

class HNSDevice{

  protected:
    uint16_t model;
    address_t address;
    std::string deviceName;


  public:

    //thread safe
    //std::mutex mtx;

    //avoid re-publish mqtt message
    //bool MQTTUpdated;

    //MQTT stuff
    MQTT_message_c* MQTT_message;
    //MQTT_message_c MQTT_message[2];
    unsigned int number_topics; //a device can publish multiple topics
                                  // ex: temperature and humidity
    HNSDevice(uint16_t model, address_t address){
      this->model = model;
      this->address =address;
      deviceName="no_name_device";
      //MQTTUpdated=true; //avoid upload default value
    }

    void setDeviceName(std::string device_name){
      this->deviceName=device_name;
    }

    std::string getDeviceName(){return this->deviceName;}

    address_t getAddress(){return address;}
    uint16_t getModel(){return model;}
    virtual res_t refresh();    //refresh data
    virtual std::string toString();

    virtual res_t runCommand(std::string command);

};

#endif /*__linux__*/
#endif /* HNSDEVICE_CLASS_HPP_*/
