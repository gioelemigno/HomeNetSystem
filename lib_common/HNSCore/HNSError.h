#ifndef HNS_ERROR_H_
#define HNS_ERROR_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
//  ERROR FLAGS to set error variable    :
#define CORRECT                               0

#define ERR_DATA                            254
#define ERR_RESOURCE_UNAVAILABLE            253
#define ERR_CRC                             252
#define ERR_RESPONSIVE                      251
#define ERR_RECIPIENT                       250
#define ERR_TIME                            249
#define ERR_SET                             248

#define ERR_READ                            247
#define ERR_WRITE                           246

#define ERR_START                           245
#define ERR_ADDRESS_NOT_FOUND_EEPROM        244
#define ERR_INTERNAL_ERROR                  243
#define ERR_NO_PACKET                       242

#define ERR_ACK_SYNC_FAILED                 241
#define ERR_TYPE                            240
#define ERR_NO_FREE_ADDRESS                 239
#define ERR_UNKNOWN_MODEL                   238
#define ERR_INVALID_ADDRESS                 237
#define ERR_DELETED_ADDRESS                 236

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

typedef int8_t res_t;
typedef uint8_t err_t;



extern err_t error;
extern void info_error(const char* msg);  //like to perror()


#ifdef __cplusplus
}
#endif

#endif /*HNS_ERROR_H_*/
