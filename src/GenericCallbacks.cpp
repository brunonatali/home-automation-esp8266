
#include "GenericCallbacks.h"

IRAM_ATTR void GenericCallbacks::defaultCallbackReturnVoidArgSelf(void *self, uint16_t number)
{
  return;
}

IRAM_ATTR bool GenericCallbacks::defaultCallbackReturnFalseArgSelf(void *self, uint16_t number)
{
  return false;
}