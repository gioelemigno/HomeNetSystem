#include "HNSRefresh.hpp"
#ifdef __linux__
#include "HNSDatabase.hpp"
#include <stdint.h>
#include "../HNSCore/HNSError.h"

//#include <mutex>          // std::mutex, std::unique_lock

res_t HNSRefresh_refresh(address_t address){

  if(address >= HNSDatabase_deviceList_index){
    error=ERR_INVALID_ADDRESS;
    return -1;
  }
  HNSDatabase_entry entry = HNSDatabase_deviceList[address];

  //thread safe
  //auto unlock end function
  //std::unique_lock<std::mutex> lck (entry.device->mtx);

  if(entry.status==deviceStatus_deleted) {
  	error =ERR_DELETED_ADDRESS;
    return -1;
  }

  res_t res = entry.device->refresh();
  printf("Index=%d, res=%d\n", address, res);

  if(res==-1){
    switch(entry.status){
      case deviceStatus_active:
        entry.status=deviceStatus_notResponse1;
      break;

      case deviceStatus_notResponse1:
        entry.status=deviceStatus_notResponse2;
      break;

      // device to delete (entry.status+1 == deviceStatus_notResponse_toDelete)
      case deviceStatus_notResponse2:
        res=HNSDatabase_remove(address);
        if(res==-1) return -1;
      break;

      default:
        return -1;
      break;
    }

  }
  else{ //success, reset not response counter
    if(entry.status < N_MAX_REFRESH_FAIL && entry.status >= deviceStatus_notResponse1){
       entry.status = deviceStatus_active;
    }
    
    //entry.device->MQTTUpdated=false;

  }
  return 0;
}

res_t HNSRefresh_refreshAllDevice(){
  res_t res = -1;
  for(uint16_t i=0; i<HNSDatabase_deviceList_index; i++){
    HNSDatabase_entry entry = HNSDatabase_deviceList[i];

    if(entry.status==deviceStatus_deleted) continue;

    res = entry.device->refresh();
    printf("Index=%d, res=%d\n", i, res);

    if(res==-1){
      switch(entry.status){
        case deviceStatus_active:
          entry.status=deviceStatus_notResponse1;
        break;

        case deviceStatus_notResponse1:
          entry.status=deviceStatus_notResponse2;
        break;

        // device to delete (entry.status+1 == deviceStatus_notResponse_toDelete)
        case deviceStatus_notResponse2:
          res=HNSDatabase_remove(i);
          if(res==-1) return -1;
        break;

        default:
          return -1;
        break;
      }

    }
    else{ //success, reset not response counter
      if(entry.status < N_MAX_REFRESH_FAIL && entry.status >= deviceStatus_notResponse1){
         entry.status = deviceStatus_active;
      }
    }


//  !!! AGGIUNGI CAMPI A DEVICE RIGUARDO L'ULTIMO REFRESH
  }
  return 0;
}

res_t HNSRefresh_printAllDevice(){
  for(uint16_t i=0; i<HNSDatabase_deviceList_index; i++){
    HNSDatabase_entry entry = HNSDatabase_deviceList[i];
    if(entry.status==deviceStatus_deleted) continue;

    printf("\nDevice %d:\n%s", i, entry.device->toString().c_str());


  }

  return 0;
}


#endif /*__linux__*/
