#include "HNSDevice.h"


#if defined(__arduino__)
  #include "../HNSArduino/Arduino.h"
#endif

#include "../HNSCore/HNSRouting.h"
#include "../util/Atmega328P/avrMacro.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../util/Atmega328P/USART_Atmega328.h"
#include <stdio.h>
#include "../HNSCore/HNSTest.h"

#include "HNSSync.h"
static void print_packetRX_ISR();

//HNSModel myModel = HNSModel_HNSTemperatureHumiditySensor;
//HNSModel myModel = HNSModel_HNSLightRelay;

void HNSDevice_setHNSModel(HNSModel model){
  myModel=model;
}



unsigned char oldPORTD = 0; //in ISR we can determinate pin changed
ISR(PCINT2_vect){

  unsigned char pins_changed = oldPORTD ^ PORTD;
  oldPORTD = PORTD;

  /*
  * HANDLER PACKET READY INTERRUPT
  */
  if(rbi(pins_changed, PD4)){  //execute if only pin 4 changed state
    disable_packet_ready_interrupt();

    sei();

    #if defined(PRINT) && defined(DEBUG_PACKET_READY)
      print_packetRX_ISR();
    #endif

    netHandler();

    enable_packet_ready_interrupt();
  }

}


int main(){
#if defined(__arduino__)

  init();

  //initVariant();

#if defined(USBCON)
  USBDevice.attach();
#endif

#endif

  oldPORTD = PORTD; //in ISR we can determinate which pin changed

  res_t res = HNSRouting_init(HNSRouting_role_HNSDevice);
  if(res==-1){
    while(1){
      printf("INTERNAL ERROR\n");
      _delay_ms(1000);
    }
  }

  res = HNSSync_init();
	if(res==-1){
		while(1){
			printf("INTERNAL ERROR\n");
			_delay_ms(1000);
		}
	}
//  myAddress = 1;

  _setup();

  while(1){
    res = HNSSync_sync();
    if(res==-1){
      while(1){
        printf("ERRORE INTERNO\n");
        _delay_ms(1000);
      }
    }

    routine();
  }

}


static void print_packetRX_ISR(){
  printf("Packet:\n");
  printf("\tSTART: %d", USART_Atmega328_packetRX.start);
  printf("\tRX: %d", USART_Atmega328_packetRX.recipient);
  printf("\tTX: %d", USART_Atmega328_packetRX.sender);
  printf("\tSIZE: %d", USART_Atmega328_packetRX.size);
  printf("\tTYPE: %d", USART_Atmega328_packetRX.type);

  for(int i=0; i < USART_Atmega328_packetRX.size; i++){
    printf("\tDATA %d: %d", i, USART_Atmega328_packetRX.data[i]);
  }

  printf("\tCRC 0: %d", USART_Atmega328_packetRX.crc_0);
  printf("\tCRC 1: %d", USART_Atmega328_packetRX.crc_1);
  printf("\n");
}
