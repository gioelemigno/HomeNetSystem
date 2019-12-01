
#if defined(__AVR_ATmega328P__)


#include "../lib/HNSArduino/Arduino.h"
#include "../lib/HNSDevice/HNSDevice.h"

#include <util/delay.h>
#include <avr/io.h>
#include "../lib/util/Atmega328P/Time_Atmega328.h"
#include "../lib/util/Atmega328P/avrMacro.h"

#include <stdio.h>
#include <stdint.h>

#include "../lib/HNSDeviceLib/HNSTemperatureHumidity_DTHxx/HNSTemperatureHumidityDTHxx.h"
#include "../lib/HNSDeviceLib/HNSLightRelay/HNSLightRelay.h"

#include "../lib/HNSCore/HNSMacro.h"


//#define TEMP_HUM
#define LIGHT

#ifdef LIGHT
HNSModel myModel = HNSModel_HNSLight;

void (*netHandler)(void)=HNSLightRelay_netHandler;


void _setup(){
	//HNSDevice_setHNSModel()
	HNSLightRelay_init();

}

void routine(){
	HNSLightRelay_routine();
}
#endif

#ifdef TEMP_HUM

HNSModel myModel = HNSModel_HNSTemperatureHumiditySensor;

void (*netHandler)(void)=HNSTemperatureHumidityDTHxx_netHandler;


void _setup(){
	pinMode(13, OUTPUT);
}

void routine(){

	digitalWrite(13, HIGH);//sbi(PORTB, PB5); //HIGH
	Time_Atmega328_delay_ms(1000);
	digitalWrite(13, LOW);//cbi(PORTB, PB5); //LOW
	Time_Atmega328_delay_ms(1000);

}
#endif

#endif /*__AVR_ATmega328P__*/
