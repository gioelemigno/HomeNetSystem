#ifndef HNS_TEMPERATURE_HUMIDITY_HPP_
#define HNS_TEMPERATURE_HUMIDITY_HPP_
#ifdef __linux__
#include "../../HNSHub/HNSDevice_class.hpp"

#define HNSTemperatureHumiditySensor_NUM_TOPICS  2
#define MQTT_TEMP   0
#define MQTT_HUM    1

class HNSTemperatureHumiditySensor : public HNSDevice{
  public:
    HNSTemperatureHumiditySensor(address_t address, 
                                  const char* MQTT_topic_temperature,
                                  const char* MQTT_topic_humidity);

    HNSTemperatureHumiditySensor(address_t address) : 
              HNSTemperatureHumiditySensor(address, "/home/living_room/sensors/temp_0", "/home/living_room/sensors/hum_0"){}

    //Override
    virtual res_t refresh();    //refresh data
    virtual std::string toString();

    double getTemperature(){return temperature;}
    double getHumidity(){return humidity;}

    std::string getUnitTemp(){return this->unitTemp;}
    std::string getUnitHum(){return this->unitHum;}

    void updateMQTT_message();
  private:
    double temperature;
    double humidity;

    std::string unitTemp;
    std::string unitHum;
};

#endif /* HNS_TEMPERATURE_HUMIDITY_HPP_*/
#endif /* __linux__*/
