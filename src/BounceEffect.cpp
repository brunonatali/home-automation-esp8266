#include <Arduino.h>
#include "BounceEffect.h"

BounceEffect::BounceEffect(int pin, int mode, int speed, int unholdCount, bool buttonDefLevel)
{
  _pin = pin;
  _originalMode = mode;
  _speed = speed;
  _unholdCount = unholdCount;
  _buttonDefLevel = buttonDefLevel;
  _bouncePeriod = 4; // quanto maior mais rápido (valores grandes podem causar efeito "escada")
  _bounceUnholdLimit = 1023 - ((_bouncePeriod * 8) + 5);
  

  unholdCallback = &GenericCallbacks::defaultCallbackReturnFalseArgSelf;
  unholdCallbackArg = this;

  os_timer_setfn(&_bounceTimer, reinterpret_cast<ETSTimerFunc*>(&BounceEffect::bounceCallback), reinterpret_cast<void*>(this));
}

BounceEffect::~BounceEffect()
{
  this->stop();
}

void BounceEffect::start(void)
{
  _value = 0;
  _direction = _bouncePeriod * 8;
  _unholdSelected = false;
  _unholdCountTemp = 0;

  if (_originalMode == OUTPUT)
    _originalValue = analogRead(_pin);

  pinMode(_pin, OUTPUT);
  _configuredAsOut = true;
  os_timer_arm(&_bounceTimer, 30, true);
}

void BounceEffect::stop(void)
{
  os_timer_disarm(&_bounceTimer);
  pinMode(_pin, _originalMode);

  if (_originalMode == OUTPUT)
    analogWrite(_pin, _originalValue);
}

void BounceEffect::setUnholdFunction(unholdcallback *callback, void *arg)
{
  unholdCallback = *callback;
  unholdCallbackArg = arg;
}

ICACHE_RAM_ATTR void BounceEffect::bounceCallback(BounceEffect* self)
{
  self->_value += self->_direction; 

  if (self->_value > 1023 || self->_value < 1) {
    self->_direction = self->_direction * (-1);
    self->_value += self->_direction;
  }

  if (self->_originalMode == INPUT) {
    if (self->_value < self->_bounceUnholdLimit) {
      
      if (!self->_configuredAsOut) {
        pinMode(self->_pin, OUTPUT);
        self->_configuredAsOut = true;
      }

      analogWrite(self->_pin, self->_value);
    } else {
      if (self->_configuredAsOut) {
        pinMode(self->_pin, INPUT);
        self->_configuredAsOut = false;
      }  
      
      if (digitalRead(self->_pin) != self->_buttonDefLevel && ++self->_unholdCountTemp >= self->_unholdCount) {
        // ---->> trigger unhold
        if (self->unholdCallback(self->unholdCallbackArg, self->_pin)) {
          self->stop();
          return;
        }
      } 
    }
  } else {
    analogWrite(self->_pin, self->_value);
  }

  #if BOUNCE_FX_DEBUG
  //Serial.println(self->_value);
  #endif
}
