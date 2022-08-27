/*
Creates instance to managa fisical outputs
Copyright (c) 2022 Bruno Natali - b010010010n@gmail.com

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

#include "FisicalOutput.h"

FisicalOutput::FisicalOutput(output_number outputNumber)
{
  this->flashValues = new FlashValues();

  this->number = outputNumber;
  this->index = FisicalOutput::getIndexByNumber(outputNumber);
  this->dimmerValue = this->flashValues->getOutputDimmer(this->index);
  this->mode = this->dimmerValue ? fisical_output_mode::DIMMER : fisical_output_mode::SWITCH;
  this->pin = OUTPUTS_PINOUT[this->index];

  // Last output (5-16) could not perform a PWM sign
  if (outputNumber = output_number::OUTPUT_5)
    this->dimmable = false;

  // Initialize pin
  pinMode(this->pin, OUTPUT);

  // Initialize output OFF
  (void)this->turnOff();
}

/**
 * @brief Set dimmer value on output
 *
 * @note If current output state is not currently configured as a Dimmer and value is
 * gratter than 0 we will configure output to dimmer.
 * If values is equal 0, than the output is decofigured as dimmable
 *
 * @param value Nibble percentage relative
 * @return true
 * @return false
 */
bool FisicalOutput::setDimmerValue(light_dimmer value)
{
  if (!this->dimmable || this->state == fisical_output_state::DISABLED)
    return false;

  SERIALPRINT("\tFisOut->setDimmerValue(");
  SERIALPRINT(value, DEC);
  SERIALPRINT(") ");

  if (this->mode == fisical_output_mode::DIMMER)
  {
    this->dimmerValue = value;
    this->flashValues->setOutputDimmer(this->index, value);

    // If value is PERCENTAGE_0, configures to non dimmable output & removes PWM
    if (value == light_dimmer::PERCENT_0)
    {
      SERIALPRINT("- SW ");
      this->mode = fisical_output_mode::SWITCH;
      return this->turnOn();
    }

    if (this->dimmerValue == light_dimmer::PERCENT_100)
      digitalWrite(this->pin, HIGH);
    else
      analogWrite(this->pin, NIBLE_STEP_PERCENTAGE[value] * 10.23);

    SERIALPRINTLN("- OK");
    return true;
  }

  // Mode = fisical_output_mode::SWITCH
  SERIALPRINT("- DM ");
  this->mode = fisical_output_mode::DIMMER;
  return this->setDimmerValue(value);
}

/**
 * @brief Turns the ligth on
 *
 * @return true
 * @return false
 */
bool FisicalOutput::turnOn()
{
  if (this->state == fisical_output_state::DISABLED)
    return false;

  SERIALPRINTLN("\tFisOut->turnOn()");

  if (this->mode == fisical_output_mode::DIMMER)
  {
    if (!this->setDimmerValue(this->dimmerValue))
    {
      // Dimmer could not be configured fallback to switch mode
      this->mode = fisical_output_mode::SWITCH;
      digitalWrite(this->pin, HIGH);
    }
  }
  else
    digitalWrite(this->pin, LOW);

  this->state = fisical_output_state::ON;

  return true;
};

/**
 * @brief Turns the ligth off
 *
 * @return true
 * @return false
 */
bool FisicalOutput::turnOff()
{
  if (this->state == fisical_output_state::DISABLED)
    return false;

  SERIALPRINTLN("\tFisOut->turnOff()");

  if (this->mode == fisical_output_mode::DIMMER)
    digitalWrite(this->pin, LOW);
  else
    digitalWrite(this->pin, HIGH);

  this->state = fisical_output_state::OFF;

  return true;
};

/**
 * @brief Get current output state
 *
 * @return fisical_output_state
 */
fisical_output_state FisicalOutput::getState()
{
  SERIALPRINTLN("\tFisOut->getState()");

  return this->state;
}

/**
 * @brief Get current output mode
 *
 * @return fisical_output_mode
 */
fisical_output_mode FisicalOutput::getMode()
{
  SERIALPRINTLN("\tFisOut->getMode()");

  return this->mode;
}

output_index FisicalOutput::getIndexByNumber(output_number outputNumber)
{
  if (outputNumber == output_number::OUTPUT_1)
    return output_index::OUTPUT_1;
  if (outputNumber == output_number::OUTPUT_2)
    return output_index::OUTPUT_2;
  if (outputNumber == output_number::OUTPUT_3)
    return output_index::OUTPUT_3;
  if (outputNumber == output_number::OUTPUT_4)
    return output_index::OUTPUT_4;
  if (outputNumber == output_number::OUTPUT_5)
    return output_index::OUTPUT_5;
}
