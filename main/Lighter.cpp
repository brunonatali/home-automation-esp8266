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

#include "Lighter.h"


Lighter::Lighter(int pin, int onLevel, int offLevel, bool dimmable, bool lockDimm, int startValue)
{
    _pin = pin;
    _onLevel = onLevel;
    _offLevel = offLevel;
    _value = startValue;
    _dimmable = dimmable;
    _lockDimm = lockDimm;

    pinMode(_pin, OUTPUT);

    if (_value) {
        if (_value > 1)
            dimmer(_value);
        else
            on();
    } else {
        off();
    }
}

bool Lighter::dimmer(int value)
{
    if (!_dimmable)
        return false;

    if(value > 100)
        value = 100;

    _value = value;

    value = value * 10.23;
    if (value)
        analogWrite(_pin, value); 
    else   
        digitalWrite(_pin, _onLevel);

    return true;
}

void Lighter::on(void)
{
    _value = 1;
    digitalWrite(_pin, _onLevel);
}

void Lighter::off(void)
{
    _value = 0;
    digitalWrite(_pin, _offLevel);
}

int Lighter::getValue(void)
{
    return _value;
}

bool Lighter::getDimmable(void)
{
    return _dimmable;
}

bool Lighter::getLockDimm(void)
{
    return _lockDimm;
}

bool Lighter::setDimmable(bool dimm)
{
    if (_lockDimm && !_dimmable)
        return false;

    _dimmable = dimm;
    return true;
}