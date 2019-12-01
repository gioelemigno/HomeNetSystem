#ifndef AVR_MACRO_H_
#define AVR_MACRO_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define sbi(port,bit) \
	(port) |= (1 << (bit))

#define cbi(port,bit) \
	(port) &= ~(1 << (bit))

#define enable_RX_interrupt() sbi(UCSR0B, RXCIE0)
#define disable_RX_interrupt() cbi(UCSR0B, RXCIE0)

#define rbi(port,bit) \
	( (port) & (1 << (bit)) )

#endif /* AVR_MACRO_H_ */
