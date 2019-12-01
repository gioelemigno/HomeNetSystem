#ifndef HNS_DATABASE_HPP_
#define HNS_DATABASE_HPP_
#ifdef __linux__
#include "HNSDevice_class.hpp"
#include "../HNSCore/HNSRouting.h"
#include "../HNSCore/HNSError.h"
#include <mutex>

extern std::mutex HNSDatabase_mutex;

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif


using namespace std; //string

//extern HNSDevice* HNSDatabase_device[MAX_N_DEVICE];
//extern uint16_t HNSDatabase_device_index;

//default value = deviceStatus_empty = 0
typedef enum deviceStatus{
    deviceStatus_empty=0,

    deviceStatus_notResponse1,
    deviceStatus_notResponse2,
    deviceStatus_notResponse_toDelete,

    deviceStatus_deleted,

    deviceStatus_active
}deviceStatus;

#define N_MAX_REFRESH_FAIL    deviceStatus_notResponse_toDelete


typedef struct HNSDatabase_entry{
    HNSDevice* device;
    deviceStatus status;
}HNSDatabase_entry;

extern HNSDatabase_entry HNSDatabase_deviceList[MAX_N_DEVICE];
extern uint16_t HNSDatabase_deviceList_index; //indicate new address available

res_t HNSDatabase_init();

res_t HNSDatabase_add(uint16_t model, address_t* address);

res_t HNSDatabase_getNewAddress(address_t* address);
res_t HNSDatabase_remove(address_t address_to_remove);

#ifdef __cplusplus
}
#endif
#endif /*__linux__*/
#endif /*HNS_DATABASE_HPP_*/
