#ifdef __linux__

#include "HNSDevice_class.hpp"
#include <stdint.h>
#include <string>


res_t HNSDevice::refresh(){
  printf("Error: no refresh for HNSDevice base class\n");
  return -1;
}

std::string HNSDevice::toString(){
  std::string str = "Hi i'm HNSDev base\n";
  return str;
}

res_t HNSDevice::runCommand(std::string command){
  return -1;
}

/*
HNSDevice::HNSDevice(uint16_t mod){
  model=mod;
}


uint16_t HNSDevice::getModel(){
  return model;
}
*/
#endif /*__linux__*/
