#ifndef HNS_SYNC_H_
#define HNS_SYNC_H_
#ifdef __cplusplus
extern "C" {
#endif
//---------------FLAGS----------------------------------------------------------
//------------------------------------------------------------------------------

//---------------INCLUDE and DEFINE for __AVR_ATmega328P__ ---------------------
#ifdef __AVR_ATmega328P__
#include "../HNSCore/HNSError.h"
#endif
//------------------------------------------------------------------------------


//------------------GLOBAL VARIABLE for__AVR_ATmega328P__-----------------------
#ifdef __AVR_ATmega328P__

#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for __AVR_ATmega328P__----------------------------
#ifdef __AVR_ATmega328P__

/*
* Read EEPROM to restore address or init sync for HNSSync()
*/
  res_t HNSSync_init();

/*
* Get an address from master or auto reset after timeout
* if address is not set normal nethandler is disable but normal routine works
* Must be placed in loop
*/
  res_t HNSSync_sync();

/*
* Reset address and clear EEPROM, it's automatic after timeout
*/
  res_t HNSSync_reset();

#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for FLAGS-----------------------------------------
//------------------------------------------------------------------------------



#ifdef __cplusplus
}
#endif
#endif /* HNS_SYNC_H_ */
