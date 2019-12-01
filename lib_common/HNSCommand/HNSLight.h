#ifndef HNS_LIGHT_H_
#define HNS_LIGHT_H_
#include "../HNSCore/HNSMacro.h"



/*
* MASTER:  size 0 | type READ_STATUS
*
* SLAVE:   size 1 | type SEND_STATUS | data status0
*/
    #define READ_STATUS     READ_1
    #define SEND_STATUS     RESPONSE_1

/*  
* MASTER:   size 0 | type SET_STATUS_ON
*           size 0 | type SET_STATUS_OFF
*
* SLAVE:    size 1 | type ACK_SET_STATUS | data status0
*/
    #define SET_STATUS_ON   SET_1
    #define SET_STATUS_OFF  SET_2
    #define ACK_SET_STATUS  ACK_1


#define STATUS_POWER_ON     STATUS_2
#define STATUS_POWER_OFF    STATUS_3


#endif /*HNS_LIGHT_H_*/
