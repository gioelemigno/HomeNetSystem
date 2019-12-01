#ifndef HNS_REFRESH_HPP_
#define HNS_REFRESH_HPP_
#ifdef __linux__
#include "../HNSCore/HNSError.h"
#include "../HNSCore/HNSRouting.h"

res_t HNSRefresh_refreshAllDevice();

res_t HNSRefresh_printAllDevice();

res_t HNSRefresh_refresh(address_t address);

#endif/*__linux__*/
#endif /*HNS_REFRESH_HPP_*/
