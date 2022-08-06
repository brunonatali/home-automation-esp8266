/**
 * General bounce effect for IO.
 * This class will handle timers and output PWM to reproduce a bounce effect into LED
 * Copyright (c) 2021 Bruno Natali - b010010010n@gmail.com
 * 
 * License (MIT license):
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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

/**
 * BounceEffect is an non-block class to deal with bounce effect and helps trigger some 
 * untouch events.
 * 
 * BounceEffect basically changes PWM value according configuration of time and the
 * increase value per cycle. Every change are called by timer, and not from while loop
 * freeing the main routine
*/
class BounceEffect
{
  public:
  
    /**
     * BounceEffect Constructor
     * 
     * @param pin IO number
     * @param mode Current IO configuration (INPUT/OUTPUT)
     * @param speed Desired bounce speed. (BOUNCE_EFFECT_SLOW | BOUNCE_EFFECT_MID | BOUNCE_EFFECT_FAST | BOUNCE_EFFECT_FASTEST)
     * 
     * @returns BounceEffect
    */
    BounceEffect(int pin, int mode, int speed, int unholdCount, bool buttonDefLevel = HIGH);

    /**
     * Calls stop()
    */
    ~BounceEffect(void);

    /**
     * Start bounce
     * Configure as output and initialize timers 
    */
    void start(void);

    /**
     * Stop bounce
     * Reconfigure to default (IN/OUT) and stop timers
    */
    void stop(void);

    /**
     * Configures callback to be triggered when user unhold button
    */
    void setUnholdFunction(unholdcallback *callback, void *arg);

    // Stores callback to unhold 
    unholdcallback unholdCallback;

    // Stores args that will be trasmited to unholdCallback
    void *unholdCallbackArg;

  private:
    // Holds declared pin
    int _pin;

    // Bouce speed
    int _speed;

    // Current PWM value
    int _value;

    // Tell if bounce is increasing or decreasing  
    int _direction;

    // Stores original value before start()
    int _originalValue;

    // Stores if is OUT | IN before start()
    int _originalMode;

    // Time between LOW - HIGH in seconds
    uint8_t _bouncePeriod; 

    // Pratically PWM limit
    uint16_t _bounceUnholdLimit;

    // Main timer
    os_timer_t _bounceTimer;
    
    // Stores if button`s default state is HIGH or LOW
    bool _buttonDefLevel;

    // Tell if is started
    bool _configuredAsOut;

    // Just for testing [REMOVE]
    bool _unholdSelected;

    // Tracks how much cycles user waiting for unhold
    int _unholdCount;

    // Temp unhold counter
    int _unholdCountTemp;

    /**
     * Change PWM value
     * This function is called by timer  
     * 
     * @param self This class BounceEffect
    */
    static ICACHE_RAM_ATTR void bounceCallback(BounceEffect* self);
};
#endif
