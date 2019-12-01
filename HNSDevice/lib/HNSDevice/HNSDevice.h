#ifndef HNS_DEVICE_H_
#define HNS_DEVICE_H_
#include <stdint.h>
#include "../HNSCore/HNSMacro.h"
#ifdef __cplusplus
extern "C" {
#endif

extern void (*netHandler)(void);

extern HNSModel myModel;
void HNSDevice_setHNSModel(HNSModel model);

void _setup();
void routine();

#ifdef __cplusplus
}
#endif


#endif /*HNS_DEVICE_H_*/
