#ifndef Converters_h
#define Converters_h

#include <Arduino.h>
#include <string.h>

void uInt2Char(uint8_t (&buf)[4], uint32_t value);

uint32_t char2UInt(const uint8_t (&buf)[4]);

#endif