#ifndef MAX485_H_
#define MAX485_H_
#ifndef _PC_
#include <wiringPi.h>

#define PIN_ENABLE 1

#define enableTX() digitalWrite(PIN_ENABLE, HIGH)
#define enableRX() digitalWrite(PIN_ENABLE, LOW)

#define MAX485_init() {\
  wiringPiSetup();  \
  pinMode(PIN_ENABLE, OUTPUT);  \
}
#endif
#endif /*MAX485_H_*/
