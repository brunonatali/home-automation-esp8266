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

#include "ProjectConfig.h"

/**
 * ESP8266 Flash size
 */
#define EEPROM_FLASH_SIZE 512

/**
 * This address meant to be store system configs
 *
 * bit 0 - Wifi Mode
 * bit 1 - Button logic level
 */
#define EEPROM_ADDRESS_CONFIG_AREA_1 0
#define EEPROM_ADDRESS_CONFIG_AREA_1_WIFI_MODE 0
#define EEPROM_ADDRESS_CONFIG_AREA_1_BUTTON_LOGIC_LEVEL 1

/**
 * Stores button hold timeout and period
 */
#define EEPROM_ADDRESS_BUTTON_HOLD_TIME 1

/**
 * Stores mode for button 1 nibble 1
 */
#define EEPROM_ADDRESS_BUTTON_1_MODE 2
/**
 * Stores mode for button 2 nibble 2
 */
#define EEPROM_ADDRESS_BUTTON_2_MODE 2
/**
 * Stores mode for button 3 nibble 1
 */
#define EEPROM_ADDRESS_BUTTON_3_MODE 3
/**
 * Stores mode for button 4 nibble 2
 */
#define EEPROM_ADDRESS_BUTTON_4_MODE 3
/**
 * Stores mode for button 5 nibble 1
 */
#define EEPROM_ADDRESS_BUTTON_5_MODE 4
/**
 * Stores mode for button 6 nibble 2
 */
#define EEPROM_ADDRESS_BUTTON_6_MODE 4

/**
 * Stores dimmer percent value for output 1
 */
#define EEPROM_ADDRESS_OUTPUT_1_DIMMER 5

/**
 * Stores dimmer percent value for output 2
 */
#define EEPROM_ADDRESS_OUTPUT_2_DIMMER 5

/**
 * Stores dimmer percent value for output 3
 */
#define EEPROM_ADDRESS_OUTPUT_3_DIMMER 6

/**
 * Stores dimmer percent value for output 4
 */
#define EEPROM_ADDRESS_OUTPUT_4_DIMMER 6

/**
 * Stores dimmer percent value for output 5
 */
#define EEPROM_ADDRESS_OUTPUT_5_DIMMER 7

/**
 * Stores dimmer percent value for buttons led
 */
#define EEPROM_ADDRESS_BUTTONS_LED_DIMMER 7

/**
 * Stores 4 bytes manager IP address
 */
#define EEPROM_ADDRESS_CONNECTION_IP_1 8
#define EEPROM_ADDRESS_CONNECTION_IP_2 9
#define EEPROM_ADDRESS_CONNECTION_IP_3 10
#define EEPROM_ADDRESS_CONNECTION_IP_4 11

/**
 * Stores 2 bytes manager socket port
 */
#define EEPROM_ADDRESS_CONNECTION_PORT_1 12
#define EEPROM_ADDRESS_CONNECTION_PORT_2 13

/**
 * Start address for Wifi SSID with 50 bytes long
 */
#define EEPROM_ADDRESS_WIFI_SSID 14

/**
 * Start address for Wifi password with 50 bytes long
 */
#define EEPROM_ADDRESS_WIFI_PASSWORD 64

/**
 * Start address for button 1 alias with 20 bytes long
 */
#define EEPROM_ADDRESS_BUTTON_1_ALIAS 114

/**
 * Start address for button 2 alias with 20 bytes long
 */
#define EEPROM_ADDRESS_BUTTON_2_ALIAS 134

/**
 * Start address for button 3 alias with 20 bytes long
 */
#define EEPROM_ADDRESS_BUTTON_3_ALIAS 154

/**
 * Start address for button 4 alias with 20 bytes long
 */
#define EEPROM_ADDRESS_BUTTON_4_ALIAS 174

/**
 * Start address for button 5 alias with 20 bytes long
 */
#define EEPROM_ADDRESS_BUTTON_5_ALIAS 194

/**
 * Start address for button 6 alias with 20 bytes long
 */
#define EEPROM_ADDRESS_BUTTON_6_ALIAS 214

/**
 * Start address for room alias with 20 bytes long
 */
#define EEPROM_ADDRESS_ROOM_ALIAS 234

/**
 * Stores remote device address for button 1
 */
#define EEPROM_ADDRESS_REMOTE_ADDRESS_1 254

/**
 * Stores remote device address for button 2
 */
#define EEPROM_ADDRESS_REMOTE_ADDRESS_2 255

/**
 * Stores remote device address for button 3
 */
#define EEPROM_ADDRESS_REMOTE_ADDRESS_3 256

/**
 * Stores remote device address for button 4
 */
#define EEPROM_ADDRESS_REMOTE_ADDRESS_4 257

/**
 * Stores remote device address for button 5
 */
#define EEPROM_ADDRESS_REMOTE_ADDRESS_5 258

/**
 * Stores remote device address for button 6
 */
#define EEPROM_ADDRESS_REMOTE_ADDRESS_6 259

/**
 * Stores own device address
 *
 * @note Our device address will be configured by manager only
 */
#define EEPROM_ADDRESS_DEVICE_ADDRESS 260

/**
 * Stores time to set system locked after hold lock button(s) nibble 1
 */
#define EEPROM_ADDRESS_SYSTEM_LOCK_TIMEOUT 261
/**
 * Stores time of inactivity to auto lock the system nibble 2
 */
#define EEPROM_ADDRESS_SYSTEM_LOCK_PERIOD 261

/**
 * This address meant to be store system (lock) configs
 *
 * bit 0 - Auto lock
 * bit 1 - Button 1 operates hold function
 * bit 2 - Button 2 operates hold function
 * bit 3 - Button 3 operates hold function
 * bit 4 - Button 4 operates hold function
 * bit 5 - Button 5 operates hold function
 * bit 6 - Button 6 operates hold function
 */
#define EEPROM_ADDRESS_CONFIG_AREA_2 262
#define EEPROM_ADDRESS_CONFIG_AREA_2_AUTO_LOCK 0
#define EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_1 1
#define EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_2 2
#define EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_3 3
#define EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_4 4
#define EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_5 5
#define EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_6 6

/**
 * This address meant to be store EEPROM configs
 *
 * bit 0 - Erase flash
 * bit 1 - Flash defaults
 */
#define EEPROM_ADDRESS_CONFIG_AREA_3 507
#define EEPROM_ADDRESS_CONFIG_AREA_3_ERASE_FLASH 0
#define EEPROM_ADDRESS_CONFIG_AREA_3_FLASH_DEFAULTS 1

Flash *FlashMemoryInstance = nullptr;

class Flash : protected EEPROMClass
{
public:
  Flash();

  static Flash *getFlashMemoryInstance();

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