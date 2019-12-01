/******************************************************************************
*
* FILENAME :        HNSRouting.c            DESIGN REF:
*
* DESCRIPTION :
*       This file contains function for send and read packet to USART
*       according to HNS protocol.
*
* PUBLIC FUNCTIONS :
*
*      res_t HNSRouting_init(HNSRouting_role_ role);
*
*      res_t HNSRouting_write(packet_t* packet);
*
*      res_t HNSRouting_read(packet_t* packet);
*
* NOTES :
*       The functions for Atmel328P USART are based on Atmel328P datasheet and
*       resource of Operating Systems Course:
*     									 https://gitlab.com/grisetti/sistemi_operativi_2018_19
*
*       The functions for RPi USART are based on:
*                   https://raspberry-projects.com/
*                           pi/programming-in-c/uart-serial-port/using-the-uart
*
*
* AUTHOR :    Gioele Migno      START DATE :    10 july 2017
*       (gioele.migno@gmail.com)
*
******************************************************************************/

/******************************************************************************
*
*    LICENSE:  GNU General Public License v3.0
*
******************************************************************************/

//-------------INCLUDE and DEFINE for all platform------------------------------
#include "HNSRouting.h"
#include "../util/CRC.h"
#include "HNSError.h"

#include <stdint.h>
#include <stdbool.h>

//#define PRE_DATA 6

#define USART_SPEED 115200//19200

//------------------------------------------------------------------------------

//---------------INCLUDE and DEFINE for __AVR_ATmega328P__ ---------------------
#ifdef __AVR_ATmega328P__
  #include <util/delay.h>
  #include "../util/Atmega328P/USART_Atmega328.h"

  typedef unsigned long time_ms_t;
  typedef unsigned long time_us_t;

#endif
//------------------------------------------------------------------------------

//-------------------INCLUDE and DEFINE for __linux__ --------------------------
#if defined(__linux__)
  #include "../util/Linux/USART_Linux.h"
  typedef double time_ms_t;
  typedef double time_us_t;
#endif

//------------------------------------------------------------------------------

//------------------GLOBAL VARIABLE for all platform----------------------------
address_t myAddress = NO_ADDRESS;
//------------------------------------------------------------------------------

//------------------GLOBAL VARIABLE for__AVR_ATmega328P__-----------------------
#ifdef __AVR_ATmega328P__
#endif
//------------------------------------------------------------------------------

//------------------GLOBAL VARIABLE for __linux__-------------------------------
#ifdef __linux__
#endif
//------------------------------------------------------------------------------

//-------------------PRIVATE FUNCTION for all platform--------------------------
//------------------------------------------------------------------------------

//-------------------PRIVATE FUNCTION for __AVR_ATmega328P__--------------------
#ifdef __AVR_ATmega328P__
#endif
//------------------------------------------------------------------------------

//-------------------PRIVATE FUNCTION for __linux__-----------------------------
#ifdef __linux__
#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for FLAGS-----------------------------------------
#if defined(PRINT)
  void print_packet(packet_t* packet){
    printf("Packet:\n");
    printf("\tSTART: %d", packet->start);
    printf("\tRX: %d", packet->recipient);
    printf("\tTX: %d", packet->sender);
    printf("\tSIZE: %d", packet->size);
    printf("\tTYPE: %d", packet->type);

    for(int i=0; i < packet->size; i++){
      printf("\tDATA %d: %d", i, packet->data[i]);
    }

    printf("\tCRC 0: %d", packet->crc_0);
    printf("\tCRC 1: %d", packet->crc_1);
    printf("\n");
  }

  static void printHexBuffer(uint8_t* buf, int size){
    int i;
    for(i=0; i<size;i++){
      printf("%x ", buf[i]);
    }
    printf("\n");
  }

  void print_packet_name(const char* packet_name, packet_t* packet){
    printf("\n%s:\n", packet_name);
    printf("\t| \033[1;33mSTART:\033[0m %d |", packet->start);
    printf(" \033[1;33mRX:\033[0m %d |", packet->recipient);
    printf(" \033[1;33mTX:\033[0m %d |", packet->sender);
    printf(" \033[1;33mSIZE:\033[0m %d |", packet->size);
    printf(" \033[1;33mTYPE:\033[0m %d |", packet->type);

    for(int i=0; i < packet->size; i++){
      printf(" \033[1;33mDATA %d:\033[0m %d |", i, packet->data[i]);
    }

    printf(" \033[1;33mCRC 0:\033[0m %d |", packet->crc_0);
    printf(" \033[1;33mCRC 1:\033[0m %d |", packet->crc_1);
    printf("\n\n");
  }

#endif

//------------------------------------------------------------------------------

//------------------------PUBLIC FUNCTION for all platform----------------------

res_t HNSRouting_init(HNSRouting_role role){
    if(role == HNSRouting_role_HNSHub) myAddress=HUB_ADDRESS;
    USART_init(USART_SPEED);
    crcInit(); //build lookup table
    return 0;
}


res_t HNSRouting_write(packet_t* packet){

#ifdef __AVR_ATmega328P__
uint8_t oldSREG = SREG;
cli();
#endif

  packet->start= START;
	packet->sender=myAddress;

//  print_packet(packet);

  data_size_t packet_size = packet->size;
  data_size_t pre_crc = PRE_DATA+packet_size;

  crc_t crc = crcFast((unsigned char*) packet, pre_crc);
	packet->crc_0 = (uint8_t)(crc>>8);
	packet->crc_1 = (uint8_t)crc;

  #if defined(PRINT) && defined(DEBUG_CRC)
   printf("HNSRouting_write(): Calculating CRC for -> size=%d\t", pre_crc);
   printHexBuffer((unsigned char*) packet, pre_crc);
	 printf("\t\tCRC: %x\tCRC_0: %x, CRC_1: %x\n", crc, packet->crc_0, packet->crc_1);
  #endif

  if(packet_size <= MAX_DATA_SIZE-2){ //put crc at tail of data
    packet->data[packet_size]=packet->crc_0;
    packet->data[packet_size+1]=packet->crc_1;
  }
  else{
    if(packet_size != MAX_DATA_SIZE){
      error=ERR_DATA;
      return -1;
    }
  }



  USART_send_buffer((uint8_t*) packet, pre_crc+2);

  #ifdef __AVR_ATmega328P__
  SREG=oldSREG;
  #endif

	return packet_size;
}

res_t old_HNSRouting_write(packet_t* packet){

	packet->start = (START_0<<8) | START_1;
	packet->sender=myAddress;

  //print_packet(packet);

	uint8_t* data = (uint8_t*)packet;
	//uint8_t* txBuff = malloc(sizeof(packet_t));
  uint8_t txBuff[sizeof(packet_t)];

	uint8_t i;
	for(i=0; i<PRE_DATA; i++){
		txBuff[i] = data[i];
	}

	for(; i-PRE_DATA < packet->size; i++){
		txBuff[i] = data[i];
	}


  crc_t crc = crcFast((unsigned char*)packet, i);
	packet->crc_0 = (uint8_t)(crc>>8);
	packet->crc_1 = (uint8_t)crc;

  #if defined(PRINT) && defined(DEBUG_CRC)
   printf("HNSRouting_write(): Calculating CRC for -> size=%d\t", i);
   printHexBuffer((unsigned char*) packet, i);
	 printf("\t\tCRC: %x\tCRC_0: %x, CRC_1: %x\n", crc, packet->crc_0, packet->crc_1);
  #endif

	txBuff[i++] = packet->crc_0;
	txBuff[i++] = packet->crc_1;
#ifdef __AVR_ATmega328P__
//_delay_ms(20);
#endif
  USART_send_buffer(txBuff, i);

	return packet->size;
}
//------------------------------------------------------------------------------

//--------------------PUBLIC FUNCTION for __AVR_ATmega328P__--------------------

#ifdef __AVR_ATmega328P__


res_t HNSRouting_read(packet_t* packet){

  if(packet_ready == false){
    error = ERR_NO_PACKET;
    return -1;
  }
  packet_ready = false; //to avoid re reading same packet

  if(USART_Atmega328_packetRX.recipient != myAddress){
    error=ERR_RECIPIENT;
    return -1;
  }

  uint8_t index = PRE_DATA+USART_Atmega328_packetRX.size;

  uint16_t crc = crcFast((unsigned char*) &USART_Atmega328_packetRX, index);

  uint8_t crc_1 = (uint8_t)crc;
  uint8_t crc_0 = (uint8_t)(crc>>8);
  if( crc_1 != USART_Atmega328_packetRX.crc_1 ||crc_0 != USART_Atmega328_packetRX.crc_0){

    #if defined(PRINT) && defined(DEBUG_CRC)
     printf("HNSRouting_read(): Wrong CRC! (Expected:Received) crc_0(%x : %x)"
                "\tcrc_1(%x : %x)\n", crc_0, packet->crc_0, crc_1, packet->crc_1);
    #endif

		error=ERR_CRC;
	  return -1;
	}

  uint8_t* src = (uint8_t*) &USART_Atmega328_packetRX;
  uint8_t* dst = (uint8_t*) packet;

  for(index=0; index < PRE_DATA; index++){
    dst[index]=src[index];
  }
  for(index=0; index < packet->size; index++){
    packet->data[index]=USART_Atmega328_packetRX.data[index];
  }

	return packet->size;
}

#endif

//------------------------------------------------------------------------------

//------------------------PUBLIC FUNCTION for __linux__-------------------------
#ifdef __linux__

res_t HNSRouting_read(packet_t* packet){

  bool received_start=false;
  time_us_t timer = (time_us_t) T_MAX_READ;
  res_t resR;

  uint8_t tmp;

  uint8_t num_start=0;
  while(! received_start){
    resR = USART_read_byte(&tmp, &timer);
    if(resR==-1) {
      if(timer<=0) error=ERR_START; //no received  start
      return -1; //throw error from usart_read
    }

    if(tmp==START_0)num_start++;
    else num_start=0;

    if(num_start == sizeof(start_t)) received_start=true;
  }

  packet->start=START;

  uint8_t* data = (uint8_t*)packet;
  uint8_t i = sizeof(start_t);

  bool ignore_packet = false; //i am not the dest

	resR = USART_read_byte(&tmp, &timer);
  if(resR==-1) return -1;       //dst

  if(tmp!=myAddress)  ignore_packet=true;

	data[i++] = tmp;    //dst

  resR = USART_read_byte(&data[i++], &timer); //src
  if(resR==-1) return -1;

	uint8_t size = 0;// = USART_read_byte();   //size
  resR = USART_read_byte(&size, &timer);
  if(resR==-1) return -1;

  data[i++] = size;

	if(size > MAX_DATA_SIZE){
	   error = ERR_DATA;
		 return -1;
	}

  resR = USART_read_byte(&data[i++], &timer); //type
  if(resR==-1) return -1;

  for(; i-PRE_DATA<size; i++){
    resR = USART_read_byte(&data[i], &timer);
    if(resR==-1) return -1;
	}

  resR = USART_read_byte(&packet->crc_0, &timer);
  if(resR==-1) return -1;

  resR = USART_read_byte(&packet->crc_1, &timer);
  if(resR==-1) return -1;

  if(ignore_packet){
    error = ERR_RECIPIENT;
    return -1;
  }

	uint16_t crc = crcFast((unsigned char*) packet, i);
  uint8_t crc_1 = (uint8_t)crc;
  uint8_t crc_0 = (uint8_t)(crc>>8);

  if( crc_1 != packet->crc_1 ||crc_0 != packet->crc_0){
    #if defined(PRINT) && defined(DEBUG_CRC)
     printf("HNSRouting_read(): Wrong CRC! (Expected:Received) crc_0(%x : %x)"
                "\tcrc_1(%x : %x)\n", crc_0, packet->crc_0, crc_1, packet->crc_1);
    #endif
		error=ERR_CRC;
	  return -1;
	}

	return size;
}

#endif

//------------------------------------------------------------------------------
