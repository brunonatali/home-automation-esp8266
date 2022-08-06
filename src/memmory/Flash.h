/**
 * EEPROM (Flash) manager.
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

#ifndef Flash_h
#define Flash_h

#include <Arduino.h>
#include <string.h>
#include <HardwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>

#include "CRC32.h"
#include "Converters.h"

#include "StandardFlashValues.h"

/**
 * ESP8266 Flash size
 */
#define EEPROM_FLASH_SIZE 512

class Flash : protected EEPROMClass
{
public:
  Flash();

  bool writeString(uint8_t memAddress, String str, bool setCrc = true);
  bool writeByte(uint8_t memAddress, uint8_t byte, bool setCrc = true);
  bool writeBit(uint8_t memAddress, uint8_t byteIndex, bool value, bool setCrc = true);

  String readString(uint8_t memAddress, uint8_t size);
  uint8_t readByte(uint8_t memAddress);

  void erase();

private:
  uint32_t calcFlashCrc();
  bool setFlashCrc();
  uint32_t getFlashCrc();
};
#endif