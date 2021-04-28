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

#ifndef TouchButtonModule_h
#define TouchButtonModule_h

#include <Arduino.h>

#include "GenericCallbacks.h"
#include "BounceEffect.h"

extern "C"{
#include "user_interface.h"
}


#ifndef TOUCH_MODULE_DEBUG
#define TOUCH_MODULE_DEBUG true
#endif

#if TOUCH_MODULE_DEBUG // Enable bounce debug
#define BOUNCE_FX_DEBUG true
#endif

class TouchButtonModule
{
  protected:
    bool _buttonHolded = false;
    
  public:
    TouchButtonModule(int pin, int buttonNumber, bool buttonDefLevel, int buttonHoldTimeOut, int buttonHoldPeriod);
    ~TouchButtonModule(void);
    void disable(void);
    void enable(void);
    void setClickFunction(clickcallback *callback, void *arg);
    void setHoldFunction(unholdcallback *callback, void *arg);
    void setUnholdFunction(unholdcallback *callback, void *arg);
    
  private:
    int _pin;
    int _buttonNumber;
    int _lastState;
    int _buttonHoldTimeOut;
    int _holdPeriod;
    bool _defaultLevel;
    bool _enabled = true;
    os_timer_t _buttonHoldTimer;
    BounceEffect *bounceFx;
    
    static ICACHE_RAM_ATTR bool buttonUnholdCallback(TouchButtonModule* self);

    static ICACHE_RAM_ATTR void buttonChangeCallback(TouchButtonModule* self);
    static ICACHE_RAM_ATTR void buttonTimerCallback(TouchButtonModule* self);

    clickcallback clickCallback;
    unholdcallback holdCallback;
    unholdcallback unholdCallback;
    void *clickCallbackArg;
    void *holdCallbackArg;
    void *unholdCallbackArg;
};
#endif
