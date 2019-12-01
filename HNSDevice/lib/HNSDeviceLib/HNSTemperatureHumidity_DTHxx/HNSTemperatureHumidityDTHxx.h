#ifndef HNS_TEMPERATURE_HUMIDITY_DTHXX_H_
#define HNS_TEMPERATURE_HUMIDITY_DTHXX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint8_t HNSTemperatureHumidityDTHxx_init();

void HNSTemperatureHumidityDTHxx_netHandler(void);

//  read temperature and humidity from dhtxx sensor
uint8_t HNSTemperatureHumidityDTHxx_routine();

// return last temperature read
float HNSTemperatureHumidityDTHxx_getTemperature();

//return last humidity read
float HNSTemperatureHumidityDTHxx_getHumidity();

#ifdef __cplusplus
}
#endif

#endif /*HNS_TEMPERATURE_HUMIDITY_DTHXX_H_*/
