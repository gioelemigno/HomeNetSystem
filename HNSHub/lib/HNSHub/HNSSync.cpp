#include "HNSSync.hpp"
//---------------FLAGS----------------------------------------------------------
//------------------------------------------------------------------------------

//-------------------INCLUDE and DEFINE for __linux__ --------------------------
#ifdef __linux__
#include "../HNSCore/HNSRouting.h"
#include "../HNSCore/HNSMacro.h"
#include "HNSDatabase.hpp"
#endif
//------------------------------------------------------------------------------

//------------------GLOBAL VARIABLE for __linux__-------------------------------
#ifdef __linux__
#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for __linux__-------------------------------------
#ifdef __linux__

res_t HNSSync_sync(){

  static uint8_t num_sync = 0; //[0-255]

  packet_t packetTX;
  packetTX.recipient=NO_ADDRESS;
  packetTX.type=SYNC;
  packetTX.size=1;
  packetTX.data[0]=num_sync++;

  res_t res_w = HNSRouting_write(&packetTX);
  if(res_w==-1){
    #if defined(PRINT) && defined(DEBUG_SYNC)
    print_packet_name("packetTX with SYNC", &packetTX);
    info_error("HNSSync_sync(): HNSRouting_write(SYNC)");
    #endif

    return -1;
  }

  packet_t packetRX;
  res_t res_r = HNSRouting_read(&packetRX);
  if(res_r==-1){
    #if defined(PRINT) && defined(DEBUG_SYNC)
    info_error("HNSSync_sync(): HNSRouting_read()");
    #endif

    return -1;
  }
  #if defined(PRINT) && defined(DEBUG_SYNC)
  print_packet_name("packetRX response of SYNC", &packetRX);
  #endif

  if(res_r == 3 && packetRX.type==SEND_MODEL){
    uint16_t model = ((packetRX.data[2] << 8) | ((uint8_t)packetRX.data[1]));

    #if defined(PRINT) && defined(DEBUG_SYNC)
    printf("HNSSync_sync():\t N° sync:%d \t Received model %x (data[1]=%x, data[0]=%x)\n",
            num_sync-1, model, packetRX.data[2], packetRX.data[1]);
    #endif

    address_t address_newDevice;

    //res_t ran = HNSSync_get_newDevice(model, &address_newDevice);
    res_t res = HNSDatabase_getNewAddress(&address_newDevice);
    if(res==-1){
      printf("errore \n");
      return -1;
    }

    packetTX.recipient=NO_ADDRESS;
    packetTX.type=ADDRESS_ASSIGNMENT;
    packetTX.data[0]=address_newDevice;
    packetTX.size=1;
    res_w = HNSRouting_write(&packetTX);
    if(res_w==-1){
      #if defined(PRINT) && defined(DEBUG_SYNC)
      print_packet_name("packetTX for address assignment",&packetTX);
      info_error("HNSSync_sync(): HNSRouting_write(ADDRESS_ASSIGNMENT)");
      #endif
      return -1;
    }

    res_r = HNSRouting_read(&packetRX);
    if(res_r == -1 ){
      error=ERR_ACK_SYNC_FAILED;
      return -1;
    }
    else if(packetRX.sender != address_newDevice){
      #if defined(PRINT) && defined(DEBUG_SYNC)
      print_packet_name("packet ACK", &packetRX);
      printf("HNSSync_sync(): bad sender (recived:expected) -> (%x, %x)\n", packetRX.sender, address_newDevice);
      #endif
      return -1;
    }
    else if(packetRX.size != 0){
      #if defined(PRINT) && defined(DEBUG_SYNC)
      print_packet_name("packet ACK", &packetRX);
      printf("HNSSync_sync(): bad size\n");
      #endif
      return -1;
    }

    res_t ran = HNSDatabase_add(model, &address_newDevice);
    if(ran==-1){
      printf("errore \n");

      return -1;
    }


    #if defined(PRINT) && defined(DEBUG_SYNC)
    printf("HNSSync_sync(): \033[1;32m SUCCESS \033[0m address_newDevice=%d\n", address_newDevice);
    #endif

  }
  else{
    printf("errore send model\n");
    return -1;
  }

  return 0;
}

#endif
//------------------------------------------------------------------------------

//-------------------FUNCTION for FLAGS-----------------------------------------
//------------------------------------------------------------------------------
