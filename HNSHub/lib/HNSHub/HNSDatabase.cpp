#ifdef __linux__
#include "HNSDatabase.hpp"
#include "HNSDevice_class.hpp"
#include "../HNSCore/HNSMacro.h"
#include "../HNSHubLib/HNSTemperatureHumidity/HNSTemperatureHumidity.hpp"
#include "../HNSHubLib/HNSLight/HNSLight.hpp"
#include <list>

#include "HNSAddressMap.hpp"

std::mutex HNSDatabase_mutex;
std::list<address_t> freeAddressList;

//HNSDevice* HNSDatabase_device[MAX_N_DEVICE];
//uint16_t HNSDatabase_device_index=0; //indicate new address available

uint16_t HNSDatabase_deviceList_index=0; //indicate new address available
HNSDatabase_entry HNSDatabase_deviceList[MAX_N_DEVICE]; //auto set to zero (stored in BSS segment)

res_t HNSDatabase_getNewAddress(address_t* address){
  if(! freeAddressList.empty()){
    *address = freeAddressList.front();
  }
  else{
    if(HNSDatabase_deviceList_index < MAX_N_DEVICE){
      *address = HNSDatabase_deviceList_index;
    }
    else{
      error=ERR_NO_FREE_ADDRESS;
      return -1;
    }
  }

  return 0;
}

res_t HNSDatabase_init(){
  return 0;
}

//on success return address
res_t HNSDatabase_add(uint16_t model, address_t* address){

    if(! freeAddressList.empty()){
      *address = freeAddressList.front();
    }
    else{
      if(HNSDatabase_deviceList_index < MAX_N_DEVICE){
        *address = HNSDatabase_deviceList_index;
      }
      else{
        error=ERR_NO_FREE_ADDRESS;
        return -1;
      }
    }

    address_t index = *address;
    HNSDevice* newDevice;

    switch(model){
      case HNSModel_HNSTemperatureHumiditySensor:
        newDevice=new HNSTemperatureHumiditySensor(index);
      break;

      case HNSModel_HNSLight:
        newDevice=new HNSLight(index);
      break;

      default:
        error=ERR_UNKNOWN_MODEL;
        return -1;
      break;
    }
    HNSDatabase_deviceList[index].device=newDevice;
    HNSDatabase_deviceList[index].status=deviceStatus_active;

    if(! freeAddressList.empty()) freeAddressList.pop_front();
    else HNSDatabase_deviceList_index++;

    //add in address map
    HNSAddressMap_add(newDevice->getDeviceName(), newDevice->getAddress());
    return 0;
}

res_t HNSDatabase_remove(address_t address_to_remove){
  std::string name = HNSDatabase_deviceList[address_to_remove].device->getDeviceName();
  HNSAddressMap_remove(name);
  
  HNSDatabase_deviceList[address_to_remove].status=deviceStatus_deleted;
  freeAddressList.push_back(address_to_remove);
  return 0;
}

#endif /*__linux__*/
