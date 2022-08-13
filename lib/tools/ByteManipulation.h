/**
 * Methods to manipulate a single byte
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
#ifndef ByteManipulation_h
#define ByteManipulation_h

#include "CustomTypes.h"

#include <Arduino.h>
#include <string.h>

class ByteManipulation
{
private:
  uint8_t *_byte;

public:
  ByteManipulation(uint8_t *byte);

  void setBit(uint8_t bitIndex, bit_t bitValue);

  bit_t getBit(uint8_t bitIndex);

  void setFirstNibble(uint8_t value);

  uint8_t getFirstNibble();

  void setSecondNibble(uint8_t value);

  uint8_t getSecondNibble();

  static void setBitOnByte(uint8_t *byte, uint8_t bitIndex, bit_t bitValue);

  static bit_t getBitOnByte(uint8_t byte, uint8_t bitIndex);

  static void setFirstNibbleOnByte(uint8_t *byte, uint8_t value);

  static uint8_t getFirstNibbleOnByte(uint8_t byte);

  static void setSecondNibbleOnByte(uint8_t *byte, uint8_t value);

  static uint8_t getSecondNibbleOnByte(uint8_t byte);
};

#endif