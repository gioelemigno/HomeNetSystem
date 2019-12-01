#include "HNSSync.h"



//---------------FLAGS----------------------------------------------------------
//------------------------------------------------------------------------------


//---------------INCLUDE and DEFINE for __AVR_ATmega328P__ ---------------------
#ifdef __AVR_ATmega328P__
 #include <avr/eeprom.h>
 #include <avr/interrupt.h>
 #include <avr/io.h>

 #include "../HNSCore/HNSRouting.h"

 #include "../HNSCore/HNSMacro.h"
 #include "HNSDevice.h"
 #include "../HNSCore/HNSError.h"
 #include <stdbool.h>
 #include <stdlib.h>

 #include "../util/Atmega328P/Time_Atmega328.h"
 #include "../util/Atmega328P/USART_Atmega328.h"
 #include "../util/Atmega328P/ADC_Atmega328P.h"


//rand() % NUM_INTERVALS_DELAY return a integer [0 - NUM_INTERVALS_DELAY-1]
// NUM_INTERVALS_DELAY must be a prime number
// considering T_MAX_READ = 200 ms
 #define NUM_INTERVALS_DELAY     47

 #define EEPROM_INDEX_ADDRESS_SET_FLAG        0
 #define EEPROM_INDEX_MY_ADDRESS              1

 // 0xAA = 10101010
 //Indicate that in EEPROM_INDEX_MY_ADDRESS thre is myAddress
 #define EEPROM_VALUE_ADDRESS_SETTED  0xAA

#define EEPROM_DEFAULT_VALUE  0xFF

 //after timeout autodelete address (HNSSync_reset())
 #define TIMEOUT_RESET            T_MAX * MAX_N_DEVICE * 2
#endif
//------------------------------------------------------------------------------

//-----------------GLOBAL VARIABLE for all platform-----------------------------

//------------------------------------------------------------------------------

//------------------GLOBAL VARIABLE for__AVR_ATmega328P__-----------------------
#ifdef __AVR_ATmega328P__
#endif
//------------------------------------------------------------------------------


//-------------------FUNCTION for __AVR_ATmega328P__----------------------------
#ifdef __AVR_ATmega328P__

volatile bool new_address_received = false;
volatile address_t new_address = NO_ADDRESS;

void (*bak_netHandler)(void);

typedef enum sync_status_t{
  sync_status_waiting_sync_command,
  sync_status_waiting_address_assignment,
  sync_status_sync_done
}sync_status_t;

volatile sync_status_t sync_status = sync_status_waiting_sync_command;

static res_t read_address_eeprom(address_t* address);
static res_t write_address_eeprom(address_t* address);

static uint16_t random_init();
void HNSSync_netHandler(void);


res_t HNSSync_init(){

  res_t res_rae = read_address_eeprom(&myAddress);
  printf("heila gamberono %d\n", res_rae);
  if(res_rae != -1){
    sync_status=sync_status_sync_done;
    return 0;
  }
  else{
    if(error==ERR_ADDRESS_NOT_FOUND_EEPROM){
      random_init();  //used for rand(), set srand() with real random seed
      sync_status=sync_status_waiting_sync_command;

      bak_netHandler=netHandler;
      netHandler=HNSSync_netHandler;

      return 0;
    }
  }

  return -1;
}

res_t HNSSync_sync(){
  switch (sync_status) {
    case sync_status_sync_done:
      printf("My address: %d\n", myAddress);
    break;

    case sync_status_waiting_sync_command:
    break;

    case sync_status_waiting_address_assignment:
    break;

    default:
    return -1;
    break;
  }

  return 0;
}


void HNSSync_netHandler(void){
  #if defined(PRINT) && defined(DEBUG_SYNC)
    printf("HNSSync_netHandler(): Run Interrupt\n");
  //  printf("HI\n");
  #endif

  packet_t packet;
  res_t resR = HNSRouting_read(&packet);
  if(resR == -1){
    return;
  }

  switch(sync_status){
    case sync_status_waiting_sync_command:

      #if defined(PRINT) && defined(DEBUG_SYNC)
        printf("WAITING SYNC COMAND\n");
      #endif

      if(resR == 1 && packet.type==SYNC){ // data size of  SYNC should be 1
        //NUM_INTERVALS_DELAY = 47(prime number)
        // delay_to_response = 0ms 2ms 4ms ... 47*2=94ms
        //suppose T_MAX = 100ms
        uint16_t delay_to_response = (rand() % NUM_INTERVALS_DELAY)*2;
        #if defined(PRINT) && defined(DEBUG_SYNC)
          printf("HNSSync_netHandler(): delay_to_response -> %d ms\n", delay_to_response);
        #endif

        unsigned long sync_timeStart = Time_Atmega328_millis();
        while(Time_Atmega328_millis() - sync_timeStart < delay_to_response){
          if(packet_ready){
            packet_t packet_sync;
            HNSRouting_read(&packet_sync);
            return;
          }
        }

        packet_t packetTX;
        packetTX.recipient = HUB_ADDRESS;

        packetTX.type=SEND_MODEL;
        packetTX.data[0]=packet.data[0];
        packetTX.data[1]=(uint8_t) myModel;
        packetTX.data[2]=(uint8_t) (myModel >> 8);
        packetTX.size=3;

        res_t res_w = HNSRouting_write(&packetTX);
        if(res_w == -1){
          #if defined(PRINT) && defined(DEBUG_SYNC)
           info_error("HNSSync_netHandler(): HNSRouting_write()");
          #endif
          return;
        }

        sync_status = sync_status_waiting_address_assignment;
      }
    break;

    case sync_status_waiting_address_assignment:
      #if defined(PRINT) && defined(DEBUG_SYNC)
        printf("WAITING ADDRESS ASSIGNMENT\n");
      #endif

      if(resR==1 && packet.type==ADDRESS_ASSIGNMENT){ //data size is 1 (address)
        new_address = packet.data[0];
        new_address_received=true;

        uint8_t oldSREG = SREG;
        cli();      //  avoid packet ready interrupt after received new_address
        myAddress = new_address;    //apply new address

        #if defined(PRINT) && defined(DEBUG_SYNC)
          printf("HNSSync_sync(): Address received -> %x\n", myAddress);
        #endif

        netHandler=bak_netHandler;  //restore normal netHandler defined in main.cpp
        SREG=oldSREG;

        packet_t packetTX;
        packetTX.recipient = HUB_ADDRESS;

        packetTX.sender =myAddress;

        packetTX.type=ACK;
        packetTX.size=0;

        res_t res_w = HNSRouting_write(&packetTX);
        if(res_w == -1){
          #if defined(PRINT) && defined(DEBUG_SYNC)
           info_error("HNSSync_sync(): HNSRouting_write()");
          #endif

          sync_status=sync_status_waiting_sync_command;
          return;
        }

      //  write_address_eeprom(&myAddress);

        sync_status=sync_status_sync_done;
      }
      else{
          sync_status=sync_status_waiting_sync_command;
      }
    break;

    default:
      error = ERR_INTERNAL_ERROR;
      while(1){
        printf("Internal error\n");
        _delay_ms(1000);
      } // unrecoverable error
    break;
  }
}

res_t HNSSync_reset(){

  uint8_t oldSREG = SREG;
  cli();

  while(! eeprom_is_ready()); // Wait until EEPE becomes zero.
  while(rbi(SPMCSR, SELFPRGEN)); // Wait until SELFPRGEN in SPMCSR becomes zero
  eeprom_update_byte((uint8_t*)EEPROM_INDEX_ADDRESS_SET_FLAG, EEPROM_DEFAULT_VALUE);

  while(! eeprom_is_ready()); // Wait until EEPE becomes zero.
  while(rbi(SPMCSR, SELFPRGEN)); // Wait until SELFPRGEN in SPMCSR becomes zero
  eeprom_update_byte((uint8_t*)EEPROM_INDEX_MY_ADDRESS, EEPROM_DEFAULT_VALUE);

  SREG = oldSREG;
  return 0;
}


static res_t read_address_eeprom(address_t* address){
  uint8_t oldSREG = SREG;
  cli();

  while(! eeprom_is_ready()); // Wait until EEPE becomes zero.
  while(rbi(SPMCSR, SELFPRGEN)); // Wait until SELFPRGEN in SPMCSR becomes zero

  uint8_t flag = eeprom_read_byte((uint8_t*)EEPROM_INDEX_ADDRESS_SET_FLAG);

  if(flag == EEPROM_VALUE_ADDRESS_SETTED){ //found address
    while(! eeprom_is_ready()); // Wait until EEPE becomes zero.
    while(rbi(SPMCSR, SELFPRGEN)); // Wait until SELFPRGEN in SPMCSR becomes zero

    *address = eeprom_read_byte((uint8_t*)EEPROM_INDEX_MY_ADDRESS);

    SREG = oldSREG;
    return 0;
  }
  else{
    error = ERR_ADDRESS_NOT_FOUND_EEPROM;

    SREG = oldSREG;
    return -1;
  }
}

static res_t write_address_eeprom(address_t* address){
  uint8_t oldSREG = SREG;
  cli();

  while(! eeprom_is_ready()); // Wait until EEPE becomes zero.
  while(rbi(SPMCSR, SELFPRGEN)); // Wait until SELFPRGEN in SPMCSR becomes zero
  eeprom_update_byte((uint8_t*)EEPROM_INDEX_MY_ADDRESS, *address);

  while(! eeprom_is_ready()); // Wait until EEPE becomes zero.
  while(rbi(SPMCSR, SELFPRGEN)); // Wait until SELFPRGEN in SPMCSR becomes zero
  eeprom_update_byte((uint8_t*)EEPROM_INDEX_ADDRESS_SET_FLAG, EEPROM_VALUE_ADDRESS_SETTED);

  SREG = oldSREG;
  return 0;
}

static uint16_t random_init(){
  ADC_Atmega328P_init();

  uint16_t seed = 0;
  uint16_t analog_read;

  analog_read=ADC_Atmega328P_read(ADC_pin_A7);
  analog_read &= 0x000F;  // 000x
  seed |= analog_read;

  analog_read=ADC_Atmega328P_read(ADC_pin_A6);
  analog_read &= 0x000F;  // 000x
  seed |= analog_read << 4; //00x0

  analog_read=ADC_Atmega328P_read(ADC_pin_A5);
  analog_read &= 0x000F;  // 000x
  seed |= analog_read << 8;

  analog_read=ADC_Atmega328P_read(ADC_pin_A4);
  analog_read &= 0x000F;  // 000x
  seed |= analog_read << 12;

  /*
  *   seed = [4LSB A7 | 4LSB A6 | 4LSB A5 | 4LSB A4]
  */

  srand(seed);

  return seed;
}


#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for FLAGS-----------------------------------------
//------------------------------------------------------------------------------
