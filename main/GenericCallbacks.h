
#ifndef GenericCallbacks_h
#define GenericCallbacks_h

#include <Arduino.h>

extern "C"{
#include "user_interface.h"
}

typedef void (*clickcallback)(void *, uint16_t number);

typedef bool (*unholdcallback)(void *, uint16_t number);

typedef bool (*holdcallback)(void *, uint16_t number);

class GenericCallbacks
{
  public:
    static ICACHE_RAM_ATTR void defaultCallbackReturnVoidArgSelf(void* self, uint16_t number = 0xFFFF);
    static ICACHE_RAM_ATTR bool defaultCallbackReturnFalseArgSelf(void* self, uint16_t number = 0xFFFF);
};


#endif