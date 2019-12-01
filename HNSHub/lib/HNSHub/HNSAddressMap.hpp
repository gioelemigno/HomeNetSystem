#ifndef HNS_ADDRESS_MAP_HPP_
#define HNS_ADDRESS_MAP_HPP_

#include "../HNSCore/HNSRouting.h"
#include "../HNSCore/HNSError.h"

#include <string>

res_t HNSAddressMap_add(std::string device_name, address_t address);
res_t HNSAddressMap_remove(std::string device_name);
res_t HNSAddressMap_getAddress(std::string device_name, address_t* address);


#endif /*HNS_ADDRESS_MAP_HPP_*/