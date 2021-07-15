#include "TouchButtonModule.h"

TouchButtonModule::TouchButtonModule(
  int pin, 
  int buttonNumber,
  uint8_t mode,
  TouchButtonModule *lighterArray[],
  FlashMan *_flash
) {
  this->_pin = pin;
  this->_buttonNumber = buttonNumber;
  this->_mode = mode;
  this->_lighterArray = lighterArray;

  this->setDefaultHoldTimeOut(BUTTON_HOLD_TIMEOUT);
  this->setDefaultHoldPeriod(BUTTON_HOLD_PERIOD);
  this->setDefaultLevel(BUTTON_DEFAULT_LEVEL);

  // Declare default callbacks
  clickCallback = &GenericCallbacks::defaultCallbackReturnVoidArgSelf;
  clickCallbackArg = this;
  holdCallback = &GenericCallbacks::defaultCallbackReturnFalseArgSelf;
  holdCallbackArg = this;
  unholdCallback = &GenericCallbacks::defaultCallbackReturnFalseArgSelf;
  unholdCallbackArg = this;
  
  pinMode(pin, INPUT);
  os_timer_setfn(&_buttonHoldTimer, reinterpret_cast<ETSTimerFunc*>(&TouchButtonModule::buttonTimerCallback), reinterpret_cast<void*>(this));
  
  if (mode >= 1 && mode <= 6) {
    delete [] lighterArray[mode];
    uint8_t modeIndex = mode - 1;
    bool pinDimmable = (_buttonPinDimmable[modeIndex] && _flash->getButtonDimmer(mode) != 0);

    this->lighter = new Lighter(_outputPin[modeIndex], pinDimmable);

    if (!_buttonPinDimmable[modeIndex])
      this->lighter->setLockDimm(true);
  }
  
  this->enable();

#if TOUCH_MODULE_DEBUG
  Serial.print("criado botao: ");
  Serial.println(_holdPeriod);
#endif
}

TouchButtonModule::~TouchButtonModule(void)
{
  this->disable();
  delete this->bounceFx;
  delete this->lighter;
}

void TouchButtonModule::removeLighter(uint8_t index)
{
  for (uint8_t i = 0 ; i < 5 ; i++)
    if (this->_lighterArray[i] == std::nullptr) {

    }
}

void TouchButtonModule::setDefaultLevel(bool level)
{
  delete this->bounceFx;
  this->_defaultLevel = level;
  
  // Satart bouce with 3 cycles to unbounce (unhold)
  this->bounceFx = new BounceEffect(this->_pin, INPUT, BOUNCE_EFFECT_MID, 3, this->_defaultLevel); 
  this->bounceFx->unholdCallback = reinterpret_cast<unholdcallback>(TouchButtonModule::buttonUnholdCallback);
  this->bounceFx->unholdCallbackArg = static_cast<void*>(this);
}

void TouchButtonModule::setDefaultHoldTimeOut(uint8_t time)
{
  this->_buttonHoldTimeOut = time * 1000;
  
}

void TouchButtonModule::setDefaultHoldPeriod(uint8_t time)
{
  this->_holdPeriod = time * 1000;
  
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
