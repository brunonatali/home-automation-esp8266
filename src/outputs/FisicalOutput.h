/**
 * Creates instance to managa fisical outputs
 * Copyright (c) 2022 Bruno Natali - b010010010n@gmail.com
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

#ifndef FisicalOutput_h
#define FisicalOutput_h

#include <Arduino.h>

#include "ProjectConfig.h"
#include "IOConfig.h"
#include "FlashValues.h"
#include "NibleStep.h"

typedef enum fisical_output_state
{
  OFF,
  ON,
  NOT_INITIALIZED,
  DISABLED
} fisical_output_state;

typedef enum fisical_output_mode
{
  SWITCH,
  DIMMER
} fisical_output_mode;

class FisicalOutput
{

public:
  FisicalOutput(output_number outputNumber);

  bool turnOn();

  bool turnOff();

  bool setDimmerValue(light_dimmer value);

  fisical_output_mode getMode();

  fisical_output_state getState();

  output_index static getIndexByNumber(output_number buttonNumber);

private:
  uint8_t pin;
  bool dimmable = true;
  light_dimmer dimmerValue;
  fisical_output_state state = fisical_output_state::NOT_INITIALIZED;
  fisical_output_mode mode = fisical_output_mode::SWITCH;
  output_number number;
  output_index index;

  FlashValues *flashValues;
};

#endif