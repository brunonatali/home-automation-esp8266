
#include "GenericCallbacks.h"

ICACHE_RAM_ATTR void GenericCallbacks::defaultCallbackReturnVoidArgSelf(void* self, uint16_t number)
{
  return;
}

ICACHE_RAM_ATTR bool GenericCallbacks::defaultCallbackReturnFalseArgSelf(void* self, uint16_t number)
{
  return false;
}