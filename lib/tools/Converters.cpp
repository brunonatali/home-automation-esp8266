/*
Generic converters functions
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

#include "Converters.h"

void uInt2Char(uint8_t (&buf)[4], uint32_t value)
{
  buf[0] = value;
  buf[1] = value >> 8;
  buf[2] = value >> 16;
  buf[3] = value >> 24;
}

uint32_t char2UInt(const uint8_t (&buf)[4])
{
  uint32_t u0 = buf[0], u1 = buf[1], u2 = buf[2], u3 = buf[3];
  uint32_t uval = u0 | (u1 << 8) | (u2 << 16) | (u3 << 24);
  return uval;
}