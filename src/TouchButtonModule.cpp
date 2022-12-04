#include "TouchButtonModule.h"

TouchButtonModule::TouchButtonModule(
    int pin,
    int buttonNumber,
    bool buttonDefLevel,
    int buttonHoldTimeOut,
    int buttonHoldPeriod)
{
  _pin = pin;
  _buttonNumber = buttonNumber;
  _buttonHoldTimeOut = buttonHoldTimeOut * 1000;
  _defaultLevel = buttonDefLevel;
  _holdPeriod = buttonHoldPeriod * 1000;

  // Declare default callbacks
  clickCallback = &GenericCallbacks::defaultCallbackReturnVoidArgSelf;
  clickCallbackArg = this;
  // holdCallback = &GenericCallbacks::defaultCallbackReturnFalseArgSelf;
  // holdCallbackArg = this;
  // unholdCallback = &GenericCallbacks::defaultCallbackReturnFalseArgSelf;
  // unholdCallbackArg = this;

  pinMode(pin, INPUT);
  os_timer_setfn(
      &_buttonHoldTimer,
      reinterpret_cast<ETSTimerFunc *>(&TouchButtonModule::buttonTimerCallback),
      reinterpret_cast<void *>(this));
  this->enable();

  SERIALPRINT("new btn: ");
  SERIALPRINTLN(_holdPeriod);
}

TouchButtonModule::~TouchButtonModule(void)
{
  this->disable();
}

IRAM_ATTR void TouchButtonModule::buttonChangeCallback(TouchButtonModule *self)
{
  SERIALPRINT("btn change: ");

  if (!self->_enabled)
  {
    SERIALPRINTLN("disabled");
    return;
  }

  SERIALPRINT(self->_pin);

  int currentBtnState = digitalRead(self->_pin);

  SERIALPRINT(",");
  SERIALPRINT(currentBtnState);

  if (currentBtnState != self->_defaultLevel)
  { // Button touched
    SERIALPRINTLN("touch");

    os_timer_arm(&self->_buttonHoldTimer, self->_buttonHoldTimeOut, false); // Trigger hold timer once
  }
  else
  { // Button untouch -> release
    SERIALPRINTLN("un-touch");

    /* BUTTON CLICK WAS DISABLED, CLICKS MUST BE TRIGGERED BY HOLD
    if (!self->_buttonHolded)
    {
      // --->> trigger click
      self->clickCallback(self->clickCallbackArg, self->_buttonNumber);
    }
    */

    os_timer_disarm(&self->_buttonHoldTimer);
  }

  self->_lastState = currentBtnState;
}

IRAM_ATTR void TouchButtonModule::buttonTimerCallback(TouchButtonModule *self)
{
  if (self->_enabled)
  {
    // self->disable();
    os_timer_disarm(&self->_buttonHoldTimer);

    // ----->> trigger holded
    // (void)self->holdCallback(self->holdCallbackArg, self->_buttonNumber);
    (void)self->clickCallback(self->clickCallbackArg, self->_buttonNumber);
  }
}

void TouchButtonModule::enable(void)
{
  SERIALPRINT("enable: ");

  if (!_enabled)
  { // Turn on button led

    if (_defaultLevel)
    {
      digitalWrite(_pin, HIGH);
      pinMode(_pin, INPUT);
    }

    os_timer_disarm(&_buttonHoldTimer);
    attachInterruptArg(
        digitalPinToInterrupt(_pin),
        reinterpret_cast<void (*)(void *)>(&TouchButtonModule::buttonChangeCallback),
        this,
        CHANGE);

    _enabled = true;
  }

  SERIALPRINTLN("OK");
}

void TouchButtonModule::disable(void)
{
  SERIALPRINT("disable: ");

  if (!_enabled)
  {
    detachInterrupt(_pin);
    os_timer_disarm(&_buttonHoldTimer);

    // Turn off button led
    if (_defaultLevel)
    {
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, LOW);
    }

    _enabled = false;
  }

  SERIALPRINTLN("ok");
}
