/******************************************************************************
*
* FILENAME :        HNSRouting.h            DESIGN REF:
*
* DESCRIPTION :
*       This file contains function for send and read packet to USART
*       according to HNS protocol.
*
* PUBLIC FUNCTIONS :
*
*      res_t HNSRouting_init();
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
#ifndef _HNSROUTING_H_
#define _HNSROUTING_H_

#ifdef __AVR_ATmega328P__
	//#pragma message("Compiling for __AVR_ATmega328P__ ...")
	#include <util/delay.h>

#endif

#ifdef __linux__
	//#pragma message("Compiling for __linux__ ...")
	#include <unistd.h>
#endif

//---------------FLAGS----------------------------------------------------------
//    #define PRINT
    #define TEST 	//PRINT MUST NOT BE DEFINED HERE
//------------------------------------------------------------------------------

//-------------INCLUDE and DEFINE for all platform------------------------------
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "HNSError.h"


typedef enum HNSRouting_role{
		HNSRouting_role_default,
		HNSRouting_role_HNSDevice = 1,
		HNSRouting_role_HNSHub
}HNSRouting_role;

#define PRE_DATA 6

#define START    0xFFFF
//Special bytes with specific purpose [250 - 255]
#define START_0                               0xFF     //start trasmission
#define START_1                               0xFF     //start trasmission
//#define WIZARD                              252     //the address that use the controller                                            // to give the address
#define NO_ADDRESS                          253     //default address for module
#define HUB_ADDRESS 												252

#define MAX_DATA_SIZE 											64

#define T_MAX                               100*1E03 //us = ms*10^3
#define T_MAX_READ                          T_MAX - 0*1E03  //us

#define MAX_N_DEVICE												32
/*
  Warning! If changed these typedef you must change write() and read() function
*/
typedef uint8_t address_t;
typedef uint8_t type_t;

typedef uint16_t start_t;
typedef uint8_t data_size_t;

//#pragma pack(1)
typedef struct packet_t {
  start_t start;   //2 BYTE START

  address_t recipient;    //dest
  address_t sender;       //source

  data_size_t size;  //specific the size of data (1, 2, 3, 4)

  type_t type;
  uint8_t data[MAX_DATA_SIZE];

  uint8_t crc_0;
  uint8_t crc_1;

} packet_t;


//------------------------------------------------------------------------------

//---------------INCLUDE and DEFINE for __AVR_ATmega328P__ ---------------------
#ifdef __AVR_ATmega328P__
	#include <stdbool.h>

	extern volatile packet_t USART_Atmega328_packetRX;
	extern volatile bool packet_ready;
#endif
//------------------------------------------------------------------------------

//-------------------INCLUDE and DEFINE for __linux__ --------------------------
#ifdef __linux__
#endif
//------------------------------------------------------------------------------

//-----------------GLOBAL VARIABLE for all platform-----------------------------
extern address_t myAddress;
//------------------------------------------------------------------------------

//------------------GLOBAL VARIABLE for__AVR_ATmega328P__-----------------------
//------------------------------------------------------------------------------

//------------------GLOBAL VARIABLE for __linux__-------------------------------
//------------------------------------------------------------------------------

//-------------------FUNCTION for __AVR_ATmega328P__----------------------------
//------------------------------------------------------------------------------

//-------------------FUNCTION for __linux__-------------------------------------
//------------------------------------------------------------------------------

//-------------------FUNCTION for FLAGS-----------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#if defined(PRINT)
  extern void print_packet(packet_t* packet);
	extern void print_packet_name(const char* packet_name, packet_t* packet);

//  extern void info_error(const char* msg);  //like to perror()
#endif

//------------------------------------------------------------------------------

/*
  Initializes uart, return -1 when an error occored
  On error, -1 is returned, and error  is  set  appropriately.
*/
extern res_t HNSRouting_init(HNSRouting_role role);

/*
  Return packet.size on success and -1 when an error occorred
  On error, -1 is returned, and error  is  set  appropriately.
*/
extern res_t HNSRouting_write(packet_t* packet);

/*
  Return packet.size on success and -1 when an error occorred
  On error, -1 is returned, and error  is  set  appropriately.
*/
extern res_t HNSRouting_read(packet_t* packet);

#ifdef __cplusplus
}
#endif

#endif /* _HNSROUTING_H_*/
