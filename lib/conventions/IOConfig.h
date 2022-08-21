/**
 * Nible (4 bit) step definition.
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

#ifndef IOConfig_h
#define IOConfig_h

#include <Arduino.h>

typedef enum button_number
{
  BUTTON_1 = 1,
  BUTTON_2 = 2,
  BUTTON_3 = 3,
  BUTTON_4 = 4,
  BUTTON_5 = 5,
  BUTTON_6 = 6,

} button_number;

typedef enum button_index
{
  BUTTON_1 = 0,
  BUTTON_2 = 1,
  BUTTON_3 = 2,
  BUTTON_4 = 3,
  BUTTON_5 = 4,
  BUTTON_6 = 5,

} button_index;

typedef enum output_number
{
  OUTPUT_1 = 1,
  OUTPUT_2 = 2,
  OUTPUT_3 = 3,
  OUTPUT_4 = 4,
  OUTPUT_5 = 5,
} output_number;

typedef enum output_index
{
  OUTPUT_1 = 0,
  OUTPUT_2 = 1,
  OUTPUT_3 = 2,
  OUTPUT_4 = 3,
  OUTPUT_5 = 4,
} output_index;

#endif