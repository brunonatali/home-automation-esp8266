/*
Light on - off manager
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

#ifndef Lighter_h
#define Lighter_h

#include <Arduino.h>

class Lighter
{
  protected:
    bool _buttonHolded = false;
    
  public:
    Lighter(int pin, int onLevel, int offLevel, bool dimmable, bool lockDimm = true, int startValue = 0);
    void on(void);
    void off(void);
    bool dimmer(int value);
    bool setDimmable(bool dimm);
    int getValue(void);
    bool getDimmable(void);
    
  private:
    int _pin;
    int _onLevel;
    int _offLevel;
    int _value;
    bool _dimmable;
    bool _lockDimm; /* lock dimmable state, prevent non dimmable IO to be configured as dimmable */
};
#endif
