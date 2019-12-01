#ifndef HNS_TEMPERATURE_HUMIDITY
#define HNS_TEMPERATURE_HUMIDITY

#include "../HNSCore/HNSMacro.h"



/*
* MASTER:  size 0 | type READ_TEMP_HUM
*
* SLAVE:   size 8 | type SEND_TEMP_HUM | data temp3 ... temp0 hum3 .. hum0
* SLAVE:   size 16 | type SEND_TEMP_HUM | data temp7 ... temp0 hum7 .. hum0
*/
#define READ_TEMP_HUM   READ_1
#define SEND_TEMP_HUM   RESPONSE_1

#define READ_TEMP       READ_2
#define SEND_TEMP       RESPONSE_2

#define READ_HUM        READ_3
#define SEND_HUM        RESPONSE_3

#endif /*HNS_TEMPERATURE_HUMIDITY*/
