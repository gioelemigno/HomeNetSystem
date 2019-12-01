//---------------FLAGS----------------------------------------------------------
//------------------------------------------------------------------------------

//-------------INCLUDE and DEFINE for all platform------------------------------
#include "HNSTest.h"
#include "HNSRouting.h"
#include <stdbool.h>

const address_t test_slave[8] = {248, 247, 246, 245, 244, 243, 242, 241};
const address_t test_master =     249;

//------------------------------------------------------------------------------

//---------------INCLUDE and DEFINE for __AVR_ATmega328P__ ---------------------
#ifdef __AVR_ATmega328P__
#include "../util/Atmega328P/avrMacro.h"
#endif
//------------------------------------------------------------------------------

//-------------------INCLUDE and DEFINE for __linux__ --------------------------
#ifdef __linux__
#include <time.h>


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
void HNSTest_netHandler(){
  packet_t packetRX;

  res_t res=HNSRouting_read(&packetRX);

  if(res==-1) {
    if(error != ERR_RECIPIENT && error != ERR_START) info_error("HNSRead()");
  //  info_error("HNSRead()");
//    printf("packet.recipient vs myAddress \t %d : %d", packetRX.recipient, myAddress);
    return;
  }
  if(packetRX.type != TEST_TYPE) return ;

  uint8_t expected_data_value = TEST_DATA_0;
  for(uint8_t i=0; i < packetRX.size ; i++){
    if(packetRX.data[i] != expected_data_value) return;
    expected_data_value++;
  }

  packet_t packetTX;
  packetTX.recipient=test_master;
  packetTX.size=packetRX.size;
  packetTX.type=TEST_TYPE;

  for(int i=0; i<packetTX.size; i++){
    packetTX.data[i]=TEST_DATA_0+i;
  }

  uint8_t oldSREG = SREG;
  cli();

    res=HNSRouting_write(&packetTX);
    if(res==-1)     info_error("HNSWrite()");

  SREG =oldSREG;
  //  if(res==-1) info_error("HNSRouting_write()");
}



#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for __linux__-------------------------------------
#ifdef __linux__

static void _print_record(record_test_t* record){
  for(int i=0; i<=TEST_MAX_SIZE ; i++){
    printf("\tsize_%d (%.2lfms, %s)\n", i, record->array_time_size[i],
        (record->array_success_size[i])? "SUCCESS": "ERROR");
  }
}
res_t HNSTest_testNetwork(){

  record_test_t array_test[TEST_N_SLAVE];

  uint8_t* rst_mem = (uint8_t*) &array_test;
  unsigned int rst_index;
  for(rst_index=0; rst_index<sizeof(record_test_t)*TEST_N_SLAVE; rst_index++){
      rst_mem[rst_index]=0;
  }

  for(int index_test = 0; index_test < TEST_N; index_test++){
    printf("TEST %d\n", index_test);
    int index_slave;
    for(index_slave=0; index_slave< TEST_N_SLAVE; index_slave++){
      record_test_t record;

      res_t res = HNSTest_test(test_slave[index_slave], &record);
      if(res==-1) {
        printf("Error testing device %d\n", index_slave);
        continue;
      }

      for(int index_size = 0; index_size<=TEST_MAX_SIZE; index_size++){
        if(record.array_success_size[index_size] == false) continue;

        array_test[index_slave].array_success_size[index_size]++;// += record.array_success_size[index_size];
        array_test[index_slave].array_time_size[index_size] += record.array_time_size[index_size];
      }
    }
  }

  for(int dev=0; dev<TEST_N_SLAVE; dev++){
    for(int siz=0; siz<=TEST_MAX_SIZE; siz++){
      if(array_test[dev].array_success_size[siz]==0) continue;
      array_test[dev].array_time_size[siz] /= array_test[dev].array_success_size[siz];
    }
  }

  for(int dev=0; dev<TEST_N_SLAVE; dev++){
    printf("\nSLAVE N°%d\n\t", dev);
    for(int siz=0; siz<=TEST_MAX_SIZE; siz++){
      double perc = ((double)array_test[dev].array_success_size[siz]/((double)TEST_N))*100;
      printf("size_%d (Time: %.2lfms, Success: %.2lf%%)", siz, array_test[dev].array_time_size[siz], perc);
//      printf("size_%d (Time: %.2lfms, Success: %.2lf%%)\n\t", siz, array_test[dev].array_time_size[siz], perc);
      if(perc != (double)100) printf("\t<-\t!!!");
      printf("\n\t");
    }
    printf("\n");
  }

  return 0;
}
res_t HNSTest_test(address_t address_device_to_test, record_test_t* record){

  struct timespec ts;

  packet_t packetTX;
  packetTX.recipient=address_device_to_test;
  packetTX.type=TEST_TYPE;
  for(int i=0; i< TEST_MAX_SIZE; i++){
    packetTX.data[i]=TEST_DATA_0+i;
  }

  for(int index_size=0; index_size<=TEST_MAX_SIZE; index_size++){
    packetTX.size=index_size;

    packetTX.recipient=address_device_to_test;
    packetTX.type=TEST_TYPE;
    for(int i=0; i< index_size; i++){
      packetTX.data[i]=TEST_DATA_0+i;
    }

    record->array_success_size[index_size]=true;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    double timeStart = ts.tv_sec*1E03+ts.tv_nsec*1E-06;

    res_t res = HNSRouting_write(&packetTX);
    if(res==-1) {
      printf("\tError testing device: %d and size: %d\t",
                address_device_to_test, index_size);
      info_error("HNSRouting_write()");

      record->array_success_size[index_size]=false;
      continue;
    }

    packet_t packetRX;
    res=HNSRouting_read(&packetRX);

    clock_gettime(CLOCK_MONOTONIC, &ts);
    double elapsed = ts.tv_sec*1E03+ts.tv_nsec*1E-06 - timeStart;
    record->array_time_size[index_size]=elapsed;

    if(res==-1){
      printf("\tError testing device: %d and size: %d\t",
                address_device_to_test, index_size);
      info_error("HNSRouting_read()");

      record->array_success_size[index_size]=false;
      continue;
    }
    if(packetRX.sender!=address_device_to_test){
      printf("\tError testing device: %d and size: %d\t",
                address_device_to_test, index_size);
      printf("Error sender\n");

      record->array_success_size[index_size]=false;
      continue;
    }
    if(packetRX.size!=index_size){
      printf("\tError testing device: %d and size: %d\t",
                address_device_to_test, index_size);
      printf("Error size: %d, expected %d \n", packetRX.size, index_size);

      record->array_success_size[index_size]=false;
      continue;
    }

    uint8_t* data_packetRX = (uint8_t*) &(packetRX.data[0]);
    uint8_t expected_data_value = TEST_DATA_0;

    uint8_t index_data;
    for(index_data=0; index_data < index_size ; index_data++){
      if(data_packetRX[index_data] != expected_data_value){
        printf("\tError testing device: %d and size: %d\t",
                  address_device_to_test, index_size);
        printf("Error data: %d, expected %d \n",
                data_packetRX[index_data], expected_data_value);

        record->array_success_size[index_size]=false;
        continue;
      }
      expected_data_value++;
    }

  }

//  printf("Slave: %d\n", address_device_to_test);
//  _print_record(record);
  return 0;
}
#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for FLAGS-----------------------------------------
//------------------------------------------------------------------------------

res_t HNSTest_init(uint8_t role){
  res_t res = HNSRouting_init(HNSRouting_role_default); //ignore role og hnsinit()
  if(res==-1) return -1;
  myAddress=role;
  return 0;
}
