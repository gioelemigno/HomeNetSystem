#ifndef HNS_TEST_H_
#define HNS_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------FLAGS----------------------------------------------------------
//------------------------------------------------------------------------------

//-------------INCLUDE and DEFINE for all platform------------------------------
#include "HNSError.h"
#include "HNSRouting.h"

	#define TEST_N_SLAVE  2    //Number of slave to testing, max 8 device

	#define TEST_N      	100

	#define TEST_MAX_SIZE		64

  #define TEST_TYPE     1
  #define TEST_DATA_0   2	//test_data_1 = test_data_0 + 1 ecc..

  extern const address_t test_slave[8];
  extern const address_t test_master;

//------------------------------------------------------------------------------

//---------------INCLUDE and DEFINE for __AVR_ATmega328P__ ---------------------
#ifdef __AVR_ATmega328P__
#endif
//------------------------------------------------------------------------------

//-------------------INCLUDE and DEFINE for __linux__ --------------------------
#ifdef __linux__

typedef struct record_test_t{
  double array_time_size[TEST_MAX_SIZE+1]; //i element refer to size_i
  int array_success_size[TEST_MAX_SIZE+1];
} record_test_t;
#endif
//------------------------------------------------------------------------------

//-----------------GLOBAL VARIABLE for all platform-----------------------------
//------------------------------------------------------------------------------

//------------------GLOBAL VARIABLE for__AVR_ATmega328P__-----------------------
#ifdef __AVR_ATmega328P__
#endif
//------------------------------------------------------------------------------

//------------------GLOBAL VARIABLE for __linux__-------------------------------
#ifdef __linux__
#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for __AVR_ATmega328P__----------------------------
#ifdef __AVR_ATmega328P__
void HNSTest_netHandler();
#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for __linux__-------------------------------------
#ifdef __linux__
/*
* Test address_device for every type of size
* return -1 when an error occurred
*/
res_t HNSTest_test(address_t address_device_to_test, record_test_t* record);
res_t HNSTest_testNetwork();
#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for FLAGS-----------------------------------------
//------------------------------------------------------------------------------
/*
* set address
*/
res_t HNSTest_init(uint8_t role);



void testNetwork(uint8_t role); //Master send a request to slave_n and collect





#ifdef __cplusplus
}
#endif

#endif /*HNS_TEST_H_*/
