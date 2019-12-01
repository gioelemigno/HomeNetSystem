#include "HNSAddressMap.hpp"
#include "../HNSCore/HNSRouting.h"
#include "../HNSCore/HNSError.h"

#include <map>
#include <string>
#include <mutex>          // std::mutex, std::unique_lock

std::mutex mtx;           // mutex for critical section
std::map<std::string, int> addressMap;

res_t HNSAddressMap_add(std::string device_name, address_t address){
    mtx.lock();
    
    addressMap.insert(std::pair<std::string, address_t>(device_name, address));
    
    mtx.unlock();
    return 0;
}
res_t HNSAddressMap_remove(std::string device_name){
    mtx.lock();

    addressMap.erase(device_name);

    mtx.unlock();
    return 0;
}
res_t HNSAddressMap_getAddress(std::string device_name, address_t* address){
    mtx.lock();

    std::map<std::string, int>::iterator it;

    it=addressMap.find(device_name);
    if(it==addressMap.end()){
        mtx.unlock();
        return -1;//not found
    }

    *address = it->second;
    
    mtx.unlock();
    return 0;
}
