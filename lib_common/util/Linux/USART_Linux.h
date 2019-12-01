#ifndef USART_LINUX_H_
#define USART_LINUX_H_

#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>

#include "../../HNSCore/HNSError.h"

#ifdef __cplusplus
extern "C" {
#endif

 res_t USART_read_byte(uint8_t* data, double* timer);
 res_t USART_send_buffer(uint8_t* buffer, uint8_t size_buffer);
 res_t USART_init(unsigned long baud_rate);


#ifdef __cplusplus
}
#endif
#endif
#endif /*USART_LINUX_H_*/
