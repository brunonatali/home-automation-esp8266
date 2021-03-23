/*
General bounce effect for IO
Copyright (c) 2021 Bruno Natali - b010010010n@gmail.com

License (MIT license):
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/

#include "TouchButtonModule.h"

/*
  @var int pin IO number 
  @var int buttonNumber System button number (generically a index)
  @var bool buttonDefLevel Tell if button is normally HIGH or LOW state
  @var int buttonHoldTimeOut Time in miliseconds button must be holded to set it`s hold
  @var int buttonHoldPeriod Time in miliseconds that button still in hold mode
*/
TouchButtonModule::TouchButtonModule(int pin, int buttonNumber, bool buttonDefLevel, int buttonHoldTimeOut, int buttonHoldPeriod)
{
  _pin = pin;
  _buttonNumber = buttonNumber;
  _buttonHoldTimeOut = buttonHoldTimeOut;
  _defaultLevel = buttonDefLevel;
  _holdPeriod = buttonHoldPeriod;

  bounceFx = new BounceEffect(pin, INPUT, BOUNCE_EFFECT_MID);
  
  pinMode(pin, INPUT);
  os_timer_setfn(&_buttonHoldTimer, reinterpret_cast<ETSTimerFunc*>(&TouchButtonModule::buttonTimerCallback), reinterpret_cast<void*>(this));
  this->enable();
}

ICACHE_RAM_ATTR void TouchButtonModule::buttonChangeCallback(TouchButtonModule* self)
{
  int currentBtnState = digitalRead(self->_pin);
  
#if TOUCH_MODULE_DEBUG
  Serial.print("Button: ");
  Serial.print(self->_buttonNumber);
  Serial.print(" touch-change(");
  Serial.print(self->_lastState);
  Serial.print("->");
  Serial.print(currentBtnState);
  Serial.println(")");
#endif

  if (currentBtnState != self->_defaultLevel) {
    os_timer_arm(&self->_buttonHoldTimer, self->_buttonHoldTimeOut, false); // Trigger hold timer once

#if TOUCH_MODULE_DEBUG
    Serial.print("SET BTN ");
    Serial.print(self->_buttonNumber);
    Serial.println(" TIMER");
#endif
  } else {
    os_timer_disarm(&self->_buttonHoldTimer);

#if TOUCH_MODULE_DEBUG
    Serial.print("UNSET BTN ");
    Serial.print(self->_buttonNumber);
    Serial.println(" TIMER");
#endif
  }
  
  self->_lastState = currentBtnState;
}

ICACHE_RAM_ATTR void TouchButtonModule::buttonTimerCallback(TouchButtonModule* self)
{
#if TOUCH_MODULE_DEBUG
  Serial.print("TIMER-BTN-");
  Serial.println(self->_buttonNumber);
#endif

  if (self->_enabled && digitalRead(self->_pin) != self->_defaultLevel) {
    self->_buttonHolded = true; // If button still holded after timeout, trigger this button is holded
    // yield();
    
    self->bounceFx->start();
    self->disable();
    if (self->_holdPeriod)
      os_timer_arm(&self->_buttonHoldTimer, self->_holdPeriod, false);
  } else if (self->_buttonHolded && self->_holdPeriod) {
    self->bounceFx->stop();
    self->enable();
  }
}

void TouchButtonModule::enable(void)
{
  _enabled = true;
  os_timer_disarm(&_buttonHoldTimer);
  attachInterruptArg(digitalPinToInterrupt(_pin), reinterpret_cast<void (*)(void*)>(&TouchButtonModule::buttonChangeCallback), this, CHANGE);
}

void TouchButtonModule::disable(void)
{
  detachInterrupt(_pin);
  os_timer_disarm(&_buttonHoldTimer);
  _enabled = false;
}
