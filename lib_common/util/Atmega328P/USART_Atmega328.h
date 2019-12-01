#ifndef USART_ATMEGA328_H_
#define USART_ATMEGA328_H_

#include <avr/io.h>
#include "avrMacro.h"
#include "../../HNSCore/HNSError.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t index_packet;  //used in readingPacket()

#define readingPacket() (index_packet < sizeof(packet_t))

#define enable_packet_ready_interrupt() \
  sbi(PCMSK2, PCINT20)

#define disable_packet_ready_interrupt()  \
  cbi(PCMSK2, PCINT20)

#define data_available() (UCSR0A & (1<<RXC0))

//bool USART_rx_buffer_available();
res_t USART_read_byte(uint8_t* data, unsigned long* timer);
res_t USART_send_buffer(uint8_t* buffer, uint8_t size_buffer);
res_t USART_init(unsigned long baud_rate);


#ifdef __cplusplus
}
#endif

#endif /*USART_ATMEGA328_H_*/
