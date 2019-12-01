#include "USART_Atmega328.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

#include "Time_Atmega328.h"
#include "../../HNSCore/HNSError.h"
#include "MAX485.h"

#include "avrMacro.h"
/*
Based on arduino library / OS resource / Atemega328 datasheet
see https://www.sparkfun.com/datasheets/Components/SMD/ATMega328.pdf
p195
*/

#define DELAY_PRE_TX      1//50//5//40 // 30 for RPi USART
#define DELAY_POST_TX     1//1//10//1

////////////////////////////////////////////////////////////////////////////////

#include "../../HNSCore/HNSRouting.h"
#include <stdbool.h>
#include <stdio.h>
//////////////////////////////////////////////////////
static void ready_packet_interrupt_init(); //set interrupt pin 4 (PICINT20)

static void write_str(char* buf);

static void printHexBuffer(uint8_t* buf, int size);
static void write_buf(uint8_t* buf, int size);
void write(uint8_t data);
/////////////////////////////////////////7
#define throw_packet_ready_interrupt() \
  if(rbi(PORTD, PD4)) cbi(PORTD, PD4); \
  else sbi(PORTD, PD4)

#define INF 255

#define INDEX_CRC_1 PRE_DATA+MAX_DATA_SIZE+1
#define INDEX_CRC_0 PRE_DATA+MAX_DATA_SIZE

#define INDEX_SIZE 4

volatile packet_t USART_Atmega328_packetRX;
volatile bool packet_ready=false;

uint8_t* packet = (uint8_t*)&USART_Atmega328_packetRX;

uint8_t num_start = 2;

uint8_t index_packet = INF;
uint8_t breakpoint = INF;



ISR(USART_RX_vect){
  unsigned char data = UDR0;

  if(num_start == 0){
    num_start=2;

    index_packet=0;
    breakpoint = INF;
  }
  else if(data==START_0) num_start--;
  else  num_start=2;

  if(index_packet < sizeof(packet_t)){

    switch (index_packet) {
      case 0: //start
        packet[index_packet++]=START_0;
        packet[index_packet++]=START_1;

        packet[index_packet++]=data;

        packet_ready=false;
        break;

      case INDEX_CRC_1:   //end packet
        packet[index_packet]=data;

        //reset_read_packet
        index_packet=INF;
        breakpoint = INF;

        packet_ready=true;

        throw_packet_ready_interrupt();
        break;

      case INDEX_SIZE:

        if(data>MAX_DATA_SIZE) {
          //reset_read_packet
          index_packet=INF;
          breakpoint = INF;

          packet_ready=false;
        }
        else{
          breakpoint=PRE_DATA+data;
          packet[index_packet++]=data;
        }
        break;

      default:
        if(index_packet==breakpoint)  index_packet = INDEX_CRC_0;
        packet[index_packet++]=data;

        break;
    }
  }
}
////////////////////////////////////////////////////////////////////////////////



//////////////////////_PRINTF_///////////////////////////////////////////
void write(uint8_t data){
  // Wait for empty transmit buffer
  while ( !( UCSR0A & (1<<UDRE0)) );
  // Put data into buffer, sends the data
  UDR0 = data;
}
// this function is called by printf as a stream handler
int usart_putchar_printf(char var, FILE *stream) {
  // translate \n to \r for br@y++ terminal
  if (var == '\n') write('\r');
  write((uint8_t)var);
  return 0;
}
static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);
////////////////////////////////////////////////////////////////////////////////

res_t USART_init(unsigned long baud_rate){
    stdout = &mystdout;
    MAX485_init();
    enableRX();

/// delete
//  cbi(DDRB, PB4); //pin 11 as input
//  sbi(PORTB, PB4); //enable pull up pin 11
////////////////


    uint8_t oldSREG = SREG;
    cli();

    sbi(UCSR0A, U2X0);  //enable Double the USART Transmission Speed

    sbi(UCSR0B, RXEN0); //enable RX
    sbi(UCSR0B, TXEN0); //enable TX

    ///////   USELESS CODE    //////////////////////////////////////////////////
        //Asynchronous USART
          cbi(UCSR0C, UMSEL01);
          cbi(UCSR0C, UMSEL00);

        //Disable Parity Mode
          cbi(UCSR0C, UPM01);
          cbi(UCSR0C, UPM00);


      //    cbi(UCSR0C, USBS0);// 1 stop bit

    ////////////////////////////////////////////////////////////////////////////
    sbi(UCSR0C, USBS0);// 2 stop bit

    // set 8 bit data size
    cbi(UCSR0C, UCSZ02);
    sbi(UCSR0C, UCSZ01);
    sbi(UCSR0C, UCSZ00);

    //Set baud rate
    uint16_t ubrr = F_CPU/8/baud_rate-1;
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;

    SREG = oldSREG;

/*
    uint16_t ubrr = F_CPU/16/baud_rate-1;
    //Set baud rate
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;
    //Enable receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // Set frame format: 8data, 2stop bit
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);

*/


    //Enable USART RX interrupt
    enable_RX_interrupt();//sbi(UCSR0B, RXCIE0);//UCSR0B |= (1<<RXCIE0);

  #ifndef __arduino__
    Time_Atmega328_init();
  #endif
  
    ready_packet_interrupt_init();

    sei(); //enable all interrupt (set Bit 7 Global Interrupt Enable in SREG)
    return 0;
}

res_t USART_read_byte(uint8_t* data, unsigned long* timer){
  return -1;
}

void USART_Flush(){
  unsigned char dummy;
  while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}

res_t USART_send_buffer(uint8_t* buffer, uint8_t size_buffer){
  _delay_ms(DELAY_PRE_TX);

  USART_Flush();

////////// DELETE
//  printf("\nENABLE RPi: %d\n", rbi(PINB, PB4)>0?1:0);
//////////////////
  enableTX();

  for(uint8_t i=0; i<size_buffer; i++){
      // Wait for empty transmit buffer
      while ( !( UCSR0A & (1<<UDRE0)) );
      // Put data into buffer, sends the data
      UDR0 = buffer[i];
  }
//  printf("\nENABLE RPi: %d\n", rbi(PINB, PB4)>0?1:0);

  while ( !( UCSR0A & (1<<UDRE0)) );  //wait tx end

  _delay_ms(DELAY_POST_TX);
  enableRX();

#if defined(PRINT) && defined(DEBUG_USART_WRITE)
  printf("USART_send_buffer(): Send ->\t");
  printHexBuffer(buffer, size_buffer);
  printf("\n");
#endif

  return 0;
}

static void ready_packet_interrupt_init(){
  sbi(DDRD, PD4); //set pin 4 as output
  sbi(PCMSK2, PCINT20); //enable PICINT20 (pin4) interrupt

  sbi(PCICR, PCIE2); //enable PCINT23..16 pin interrupt
  sei(); //enable all interrupt
}

static void printHexBuffer(uint8_t* buf, int size){
  int i;
  for(i=0; i<size;i++){
    printf("%x ", buf[i]);
  }
  printf("\n");
}

static void write_str(char* buf){
  while(*buf){
    write(*buf++);
  }
}

static void write_buf(uint8_t* buf, int size){

}
