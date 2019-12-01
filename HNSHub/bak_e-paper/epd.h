#ifndef EPD_H_
#define EPD_H_

#define EPD2IN7

#ifdef EPD2IN7
  #include "epd2in7.h"
#elif defined(ANOTHER_MODEL) //EXAMPLE
  #define ANOTHER_MODEL
#else
  #error INDICATE DISPLAY MODEL IN MAKEFILE
#endif

#endif /*EPD_H_*/
