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


Lighter::Lighter(int pin, bool dimmable)
{
    this->_pin = pin;
    this->_onLevel = (dimmable ? HIGH : LOW);
    this->_dimmable = dimmable;
    

    pinMode(this->_pin, OUTPUT);

    this->off();
}

Lighter::~Lighter(void)
{
    this->off();
}

void Lighter::setOnLevel(bool level)
{
    this->_onLevel = level;
}

void Lighter::setLockDimm(bool lock)
{
    this->_lockDimm = lock;
}

bool Lighter::setDimmable(bool dimm)
{
    if (dimm && this->_lockDimm) // set && lock
        return false;

    this->_dimmable = dimm;
    return true;
}

bool Lighter::dimmer(int value)
{
    if (!this->_dimmable)
        return false;

    if (value < 2)
        value = 2;
    else if (value > 100)
        value = 100;

    this->_value = value;

    analogWrite(this->_pin, value * 10.23); // 1023 -> 100%

    return true;
}

bool Lighter::on(void)
{
    digitalWrite(this->_pin, this->_onLevel);

    if (digitalRead(this->_pin) != this->_onLevel)
        return false;
        
    this->_value = this->_onLevel;
    return true;
}

bool Lighter::off(void)
{
    digitalWrite(this->_pin, !this->_onLevel);

    if (digitalRead(this->_pin) != !this->_onLevel)
        return false;
        
    this->_value = !this->_onLevel;
    return true;
}

int Lighter::getValue(void)
{
    return this->_value;
}

bool Lighter::getDimmable(void)
{
    return this->_dimmable;
}

bool Lighter::getLockDimm(void)
{
    return this->_lockDimm;
}