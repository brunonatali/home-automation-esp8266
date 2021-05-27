#include "TouchButtonModule.h"

TouchButtonModule::TouchButtonModule(int pin, int buttonNumber, bool buttonDefLevel, int buttonHoldTimeOut, int buttonHoldPeriod) {
  _pin = pin;
  _buttonNumber = buttonNumber;
  _buttonHoldTimeOut = buttonHoldTimeOut * 1000;
  _defaultLevel = buttonDefLevel;
  _holdPeriod = buttonHoldPeriod * 1000;

  bounceFx = new BounceEffect(pin, INPUT, BOUNCE_EFFECT_MID, 3, buttonDefLevel); // Satart bouce with 3 cycles to unbounce (unhold)

  // bounceFx->setUnholdFunction(reinterpret_cast<unholdcallback*>(&TouchButtonModule::buttonUnholdCallback), static_cast<void*>(this));

  bounceFx->unholdCallback = reinterpret_cast<unholdcallback>(TouchButtonModule::buttonUnholdCallback);
  bounceFx->unholdCallbackArg = static_cast<void*>(this);

  // Declare default callbacks
  clickCallback = &GenericCallbacks::defaultCallbackReturnVoidArgSelf;
  clickCallbackArg = this;
  holdCallback = &GenericCallbacks::defaultCallbackReturnFalseArgSelf;
  holdCallbackArg = this;
  unholdCallback = &GenericCallbacks::defaultCallbackReturnFalseArgSelf;
  unholdCallbackArg = this;
  
  pinMode(pin, INPUT);
  os_timer_setfn(&_buttonHoldTimer, reinterpret_cast<ETSTimerFunc*>(&TouchButtonModule::buttonTimerCallback), reinterpret_cast<void*>(this));
  this->enable();

#if TOUCH_MODULE_DEBUG
  Serial.print("criado botao: ");
  Serial.println(_holdPeriod);
#endif
}

TouchButtonModule::~TouchButtonModule(void)
{
  this->disable();
}

void TouchButtonModule::setClickFunction(clickcallback *callback, void *arg)
{
  clickCallback = *callback;
  clickCallbackArg = arg;
}

void TouchButtonModule::setHoldFunction(holdcallback *callback, void *arg)
{
  holdCallback = *callback;
  holdCallbackArg = arg;
}

void TouchButtonModule::setUnholdFunction(unholdcallback *callback, void *arg)
{
  unholdCallback = *callback;
  unholdCallbackArg = arg;
}

ICACHE_RAM_ATTR bool TouchButtonModule::buttonUnholdCallback(TouchButtonModule* self, uint16_t number)
{
  self->bounceFx->stop();
  self->enable();
  self->_buttonHolded = false;
#if TOUCH_MODULE_DEBUG
  Serial.println("passou funcao");
#endif
  self->unholdCallback(self->unholdCallbackArg, self->_buttonNumber);
  return true;
}

ICACHE_RAM_ATTR void TouchButtonModule::buttonChangeCallback(TouchButtonModule* self)
{
#if TOUCH_MODULE_DEBUG
  Serial.print("botao change:");
#endif
  if (!self->_enabled)
    return;
#if TOUCH_MODULE_DEBUG
  Serial.print(self->_pin);
#endif

  int currentBtnState = digitalRead(self->_pin);
#if TOUCH_MODULE_DEBUG
  Serial.print(",");
  Serial.print(currentBtnState);
#endif

  if (currentBtnState != self->_defaultLevel) { // Button touched
#if TOUCH_MODULE_DEBUG
    Serial.println("touched");
#endif
    os_timer_arm(&self->_buttonHoldTimer, self->_buttonHoldTimeOut, false); // Trigger hold timer once
  } else { // Button untouch -> release
#if TOUCH_MODULE_DEBUG
    Serial.println("un-touched");
#endif
    if (!self->_buttonHolded) {
      // --->> trigger click
      self->clickCallback(self->clickCallbackArg, self->_buttonNumber);
    }
    
    os_timer_disarm(&self->_buttonHoldTimer);
  }
  
  self->_lastState = currentBtnState;
}

ICACHE_RAM_ATTR void TouchButtonModule::buttonTimerCallback(TouchButtonModule* self)
{
  if (self->_enabled) {
    self->_buttonHolded = true;
    self->disable();
    self->bounceFx->start();

    // ----->> trigger holded
    (void) self->holdCallback(self->holdCallbackArg, self->_buttonNumber);
    
    if (self->_holdPeriod)
      os_timer_arm(&self->_buttonHoldTimer, self->_holdPeriod, false);
  } else if (self->_buttonHolded && self->_holdPeriod) {

    // ----->> trigger unholded
    (void) TouchButtonModule::buttonUnholdCallback(self, self->_buttonNumber);
  }
}

void TouchButtonModule::enable(void)
{
#if TOUCH_MODULE_DEBUG
  Serial.print("enable:");
#endif
  if (_enabled)
    return;
#if TOUCH_MODULE_DEBUG
  Serial.println("OK");
#endif

  // Turn on button led
  if (_defaultLevel) {
    digitalWrite(_pin, HIGH);
    pinMode(_pin, INPUT);
  }

  os_timer_disarm(&_buttonHoldTimer);
  attachInterruptArg(digitalPinToInterrupt(_pin), reinterpret_cast<void (*)(void*)>(&TouchButtonModule::buttonChangeCallback), this, CHANGE);

  _enabled = true;
}

void TouchButtonModule::disable(void)
{
#if TOUCH_MODULE_DEBUG
  Serial.print("dsabled: ");
#endif
  if (!_enabled)
    return;
#if TOUCH_MODULE_DEBUG
  Serial.println("ok");
#endif

  detachInterrupt(_pin);
  os_timer_disarm(&_buttonHoldTimer);

  // Turn off button led
  if (_defaultLevel) {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
  }

  _enabled = false;
}
