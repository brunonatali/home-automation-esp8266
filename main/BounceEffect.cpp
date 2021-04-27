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

#include <Arduino.h>
#include "BounceEffect.h"

/*
  @var int pin IO number 
  @var int mode Current IO configuration (INPUT/OUTPUT)
  @var int speed Desired bounce speed. Use difined: BOUNCE_EFFECT_SLOW, BOUNCE_EFFECT_MID, BOUNCE_EFFECT_FAST or BOUNCE_EFFECT_FASTEST
*/
BounceEffect::BounceEffect(int pin, int mode, int speed, int unholdCount, bool buttonDefLevel)
{
  _pin = pin;
  _originalMode = mode;
  _speed = speed;
  _unholdCount = unholdCount;
  _buttonDefLevel = buttonDefLevel;

  os_timer_setfn(&_bounceTimer, reinterpret_cast<ETSTimerFunc*>(&BounceEffect::bounceCallback), reinterpret_cast<void*>(this));
}

void BounceEffect::start()
{
  _value = 0;
  _direction = 2;
  _unholdSelected = false;
  _unholdCountTemp = 0;

  if (_originalMode == OUTPUT)
    _originalValue = analogRead(_pin);

  pinMode(_pin, OUTPUT);
  _configuredAsOut = true;
  os_timer_arm(&_bounceTimer, 10, true);
}

void BounceEffect::stop()
{
  os_timer_disarm(&_bounceTimer);
  pinMode(_pin, _originalMode);

  if (_originalMode == OUTPUT)
    analogWrite(_pin, _originalValue);
}

ICACHE_RAM_ATTR void BounceEffect::bounceCallback(BounceEffect* self)
{
  self->_value += self->_direction;

  if (self->_value > 1023 || self->_value < 1) {
    self->_direction = self->_direction * (-1);
    self->_value += self->_direction;
  }

  if (self->_originalMode == INPUT) {
    if (self->_value < 1000) {
      if (!self->_configuredAsOut) {
        if (self->_unholdSelected) {
          self->_unholdCountTemp ++;
          if (self->_unholdCountTemp >= self->_unholdCount) {
            self->stop();
            // ---->> trigger unhold
          }
        }

        pinMode(self->_pin, OUTPUT);
        self->_configuredAsOut = true;
      }
      analogWrite(self->_pin, self->_value);
    } else {
      if (self->_configuredAsOut) {
        pinMode(self->_pin, INPUT);
        self->_configuredAsOut = false;
      }
      if (!self->_unholdSelected && digitalRead(self->_pin) != self->_buttonDefLevel)
        self->_unholdSelected = true;
    }
  } else {
    analogWrite(self->_pin, self->_value);
  }

  #if BOUNCE_FX_DEBUG
  Serial.println(self->_value);
  #endif
}
