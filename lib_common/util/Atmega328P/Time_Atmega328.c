/*******************************************************************************
*
*   Based on wiring.c file of Arduino core and explanation by
*           https://ucexperiment.wordpress.com/2012/03/16
*                                   /examination-of-the-arduino-millis-function/
*   It supports only Atmega328p
*
*******************************************************************************/
/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "Time_Atmega328.h"

#ifdef __arduino__
#include "../../HNSArduino/Arduino.h"
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include "avrMacro.h"

#ifdef __arduino__

void Time_Atmega328_init(){
	init();
}

unsigned long Time_Atmega328_millis(){
	return millis();
}

unsigned long Time_Atmega328_micros(){
	return micros();
}

void Time_Atmega328_delay_ms(unsigned long timer){
	delay(timer);
}

void Time_Atmega328_delay_us(unsigned long timer){
	delayMicroseconds((unsigned int)timer);
}

#else
//add from https://ucexperiment.wordpress.com/2012/03/16/examination-of-the-arduino-millis-function/
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

ISR(TIMER0_OVF_vect){
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}

unsigned long Time_Atmega328_millis(){
  unsigned long m;
	uint8_t oldSREG = SREG; //backup SREG

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli(); //disable all interrupt (clear Bit 7 Global Interrupt Enable in SREG)
	m = timer0_millis;
	SREG = oldSREG; //restore SREG (including sei())

	return m;
}
unsigned long Time_Atmega328_micros(){
  unsigned long m;
	uint8_t oldSREG = SREG, t;

	cli();
	m = timer0_overflow_count;
#if defined(TCNT0)
	t = TCNT0;
#elif defined(TCNT0L)
	t = TCNT0L;
#else
	#error TIMER 0 not defined
#endif

#ifdef TIFR0
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
		m++;
#else
	if ((TIFR & _BV(TOV0)) && (t < 255))
		m++;
#endif

	SREG = oldSREG;

	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

void Time_Atmega328_init(){

  // enable all interrupts
  sei(); //enable all interrupt (set Bit 7 Global Interrupt Enable in SREG)

  // this combination is for the standard 168/328/1280/2560
  sbi(TCCR0B, CS01);
  sbi(TCCR0B, CS00);

  // enable timer 0 overflow interrupt
  sbi(TIMSK0, TOIE0);

  return;
}

void Time_Atmega328_delay_ms(unsigned long timer){
	unsigned long start = Time_Atmega328_millis();
	while(Time_Atmega328_millis() - start < timer);
}

void Time_Atmega328_delay_us(unsigned long timer){
	unsigned long start = Time_Atmega328_micros();
	while(Time_Atmega328_micros() - start < timer);
}
#endif
