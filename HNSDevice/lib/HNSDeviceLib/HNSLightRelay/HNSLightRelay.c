#include "HNSLightRelay.h"
#include "../../HNSCore/HNSError.h"
#include "../../HNSCommand/HNSLight.h"
#include <stdint.h>
#include <stdbool.h>
#include "../../HNSCore/HNSRouting.h"

#include "../../HNSArduino/Arduino.h"

#define RELAY_LIGHT     13
#define BUTTON	        9 //with internal pull_up resistor

//they must not have the same value! there is a difference of 20/30ms
#define TIME_RELAY_LIGHT 80 //80
#define TIME_CHANGE_STATUS_LIGHT 110 //110

volatile uint8_t status = STATUS_POWER_OFF;

bool pressButton;
unsigned long timePressButton;
bool change;
unsigned long timeRelayStatus;
unsigned long _time;

//return new status
static inline uint8_t toggle_status();

res_t HNSLightRelay_init(){
  pinMode(RELAY_LIGHT, OUTPUT);

  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH); //enable internal pull-up resistor

  pressButton = false;
  timePressButton = 0;
  change = false;
  timeRelayStatus = 0;
  _time = 0;
  
}

/*
* MASTER:  size 0 | type READ_STATUS
*
* SLAVE:   size 1 | type SEND_STATUS | data status0
*/
/*  
* MASTER:   size 0 | type SET_STATUS_ON
*           size 0 | type SET_STATUS_OFF
*
* SLAVE:    size 1 | type ACK_SET_STATUS | data status0
*/
void HNSLightRelay_netHandler(void){
  packet_t packet;
  res_t res = HNSRouting_read(&packet);
  if(res == -1){
    return;
  }

  switch(res){
      case 0:
        switch(packet.type){
            case READ_STATUS:
                packet.recipient=HUB_ADDRESS;
                packet.size=1;
                packet.type=SEND_STATUS;

                packet.data[0]=status;
                
                res=HNSRouting_write(&packet);
                if(res==-1) return;   
            break;

            case SET_STATUS_ON:
                HNSLightRelay_setStatus(STATUS_POWER_ON);

                packet.recipient=HUB_ADDRESS;
                packet.size=1;
                packet.type=ACK_SET_STATUS;

                packet.data[0]=status;
                res=HNSRouting_write(&packet);
                if(res==-1) return; 
            break;

            case SET_STATUS_OFF:
                HNSLightRelay_setStatus(STATUS_POWER_OFF);
                
                packet.recipient=HUB_ADDRESS;
                packet.size=1;
                packet.type=ACK_SET_STATUS;

                packet.data[0]=status;
                res=HNSRouting_write(&packet);
                if(res==-1) return; 
            break;

            default:
                return -1;
            break;
        }
      break;

      case 1:
      break;

      default:
      break;
  }
}

res_t HNSLightRelay_routine(){
  	 _time = millis();

  	 if(pressButton)
  	 {
  		 if(digitalRead(BUTTON) == LOW)
  		 {
  			 if(	(_time - timePressButton > TIME_CHANGE_STATUS_LIGHT) )
  			 {
  				 change = true;
  				 digitalWrite(RELAY_LIGHT, HIGH);
  				 timePressButton = _time;
  				 timeRelayStatus = _time;
  				 pressButton = false;
  			 }
  		 }
  		 else
  		 {
  			 pressButton = false;
  		 }
  	 }
  	 else
  	 {
  		 if( digitalRead(BUTTON) == LOW )
  		 {
  			 pressButton = true;
  			 timePressButton = _time;
  		 }
  	 }

  	 if(change)
  	 {
       if(digitalRead(BUTTON)==HIGH)
       {
          // toggle_status();
  		     if ((_time - timeRelayStatus) > TIME_RELAY_LIGHT)
  		     {
  			        digitalWrite(RELAY_LIGHT, LOW);
                    
                    //status = !status;
  			        status=(status==STATUS_POWER_ON)?STATUS_POWER_OFF:STATUS_POWER_ON;
                    
                    timeRelayStatus = _time;
  			        change = false;
  		     }
       }
  	 }
    return 0;
}

// return status
uint8_t HNSLightRelay_getStatus(){
    return status;
}

res_t HNSLightRelay_setStatus(uint8_t _status){
    if(status == _status)   return -1;
    toggle_status();
    return 0;
}

static inline uint8_t toggle_status(){
    unsigned long timer=millis();
    digitalWrite(RELAY_LIGHT, HIGH);

    while(millis()-timer < TIME_RELAY_LIGHT); //delay(TIME_RELAY_LIGHT)
  	
    digitalWrite(RELAY_LIGHT, LOW);

    //toggle status
    status= (status==STATUS_POWER_ON)?SET_STATUS_OFF:STATUS_POWER_ON;

    return status;
}
