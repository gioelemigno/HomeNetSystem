#ifndef MAX485_H_
#define MAX485_H_

#include "avrMacro.h"
#include <avr/io.h>



#define enableTX() PORTB |= (1 << PB0) //digitalWRite(8, HIGH)
#define enableRX() PORTB &= ~(1 << PB0) //digitalWRite(8, LOW)

#define MAX485_init() sbi(DDRB, PB0) //pinMode(8, OUTPUT)

#endif /*MAX485_H_*/
