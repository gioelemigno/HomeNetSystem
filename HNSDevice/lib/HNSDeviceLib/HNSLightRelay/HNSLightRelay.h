#ifndef HNS_LIGHT_RELAY_H_
#define HNS_LIGHT_RELAY_H_
#include <stdint.h>
#include "../../HNSCore/HNSError.h"
#ifdef __cplusplus
extern "C" {
#endif

res_t HNSLightRelay_init();

void HNSLightRelay_netHandler(void);

res_t HNSLightRelay_routine();

// return status
uint8_t HNSLightRelay_getStatus();

res_t HNSLightRelay_setStatus(uint8_t status);

#ifdef __cplusplus
}
#endif
#endif /*HNS_LIGHT_RELAY_H_*/