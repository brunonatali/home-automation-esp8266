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

#ifndef BounceEffect_h
#define BounceEffect_h

#include <Arduino.h>

#include "GenericCallbacks.h"
extern "C"{
#include "user_interface.h"
}

#ifndef BOUNCE_FX_DEBUG
#define BOUNCE_FX_DEBUG true
#endif

#define BOUNCE_EFFECT_SLOW 1

#define BOUNCE_EFFECT_MID 2

#define BOUNCE_EFFECT_FAST 4

#define BOUNCE_EFFECT_FASTEST 6

class BounceEffect
{
  public:
    BounceEffect(int pin, int mode, int speed, int unholdCount, bool buttonDefLevel = HIGH);
    ~BounceEffect(void);
    void start(void);
    void stop(void);
    void setUnholdFunction(unholdcallback *callback, void *arg);

  private:
    int _pin;
    int _speed;
    unsigned int _value;
    uint8_t _direction;
    int _originalValue;
    int _originalMode;
    os_timer_t _bounceTimer;
    
    bool _buttonDefLevel;
    bool _configuredAsOut;
    bool _unholdSelected;
    int _unholdCount;
    int _unholdCountTemp;

    static ICACHE_RAM_ATTR void bounceCallback(BounceEffect* self);

    unholdcallback unholdCallback;
    void *unholdCallbackArg;
};
#endif
