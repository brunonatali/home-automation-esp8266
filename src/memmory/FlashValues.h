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

#ifndef FlashValues_h
#define FlashValues_h

#include "ProjectConfig.h"

#include "DefaultFlashValues.h"
#include "ByteManipulation.h"
#include "NibleStep.h"
#include "CustomTypes.h"
#include "Flash.h"
#include "IOConfig.h"

#include "IPAddress.h"

typedef enum flash_value_result
{
  OK,
  ERROR,
  WRONG_STRING_SIZE,
  SAME_VALUE_NOT_ALLOWED
} flash_value_result;

typedef enum button_mode
{
  NOT_CONFIGURED = LIGHT_MODE_STEP_NOT_CONFIGURED,
  OUTPUT_1 = LIGHT_MODE_STEP_OUTPUT_1,
  OUTPUT_2 = LIGHT_MODE_STEP_OUTPUT_2,
  OUTPUT_3 = LIGHT_MODE_STEP_OUTPUT_3,
  OUTPUT_4 = LIGHT_MODE_STEP_OUTPUT_4,
  OUTPUT_5 = LIGHT_MODE_STEP_OUTPUT_5,
  REMOTE_OUTPUT_1 = LIGHT_MODE_STEP_REMOTE_OUTPUT_1,
  REMOTE_OUTPUT_2 = LIGHT_MODE_STEP_REMOTE_OUTPUT_2,
  REMOTE_OUTPUT_3 = LIGHT_MODE_STEP_REMOTE_OUTPUT_3,
  REMOTE_OUTPUT_4 = LIGHT_MODE_STEP_REMOTE_OUTPUT_4,
  REMOTE_OUTPUT_5 = LIGHT_MODE_STEP_REMOTE_OUTPUT_5,
  DISABLED = LIGHT_MODE_STEP_DISABLED
} button_mode;

typedef enum light_dimmer
{
  PERCENT_0 = PERCENTAGE_0,
  PERCENT_6 = PERCENTAGE_6,
  PERCENT_13 = PERCENTAGE_13,
  PERCENT_19 = PERCENTAGE_19,
  PERCENT_26 = PERCENTAGE_26,
  PERCENT_32 = PERCENTAGE_32,
  PERCENT_39 = PERCENTAGE_39,
  PERCENT_45 = PERCENTAGE_45,
  PERCENT_53 = PERCENTAGE_53,
  PERCENT_60 = PERCENTAGE_60,
  PERCENT_66 = PERCENTAGE_66,
  PERCENT_73 = PERCENTAGE_73,
  PERCENT_79 = PERCENTAGE_79,
  PERCENT_86 = PERCENTAGE_86,
  PERCENT_93 = PERCENTAGE_93,
  PERCENT_100 = PERCENTAGE_100
} light_dimmer;

typedef enum seconds_step
{
  MILISECONDS_500 = SECONDS_0_5,
  SEC_1 = SECONDS_1,
  SEC_2 = SECONDS_2,
  SEC_3 = SECONDS_3,
  SEC_4 = SECONDS_4,
  SEC_5 = SECONDS_5,
  SEC_7 = SECONDS_7,
  SEC_8 = SECONDS_8,
  SEC_10 = SECONDS_10,
  SEC_15 = SECONDS_15,
  SEC_20 = SECONDS_20,
  SEC_25 = SECONDS_25,
  SEC_30 = SECONDS_30,
  SEC_40 = SECONDS_40,
  SEC_50 = SECONDS_50,
  SEC_60 = SECONDS_60,
} seconds_step;

/**
 * Wifi mode is stored into bit, so never add more possibilities
 */
typedef enum wifi_mode
{
  ACCESS_POINT = 0,
  WIFI_CLIENT = 1
} wifi_mode;

/**
 * Button logic level is stored into bit, so never add more possibilities
 */
typedef enum button_logic_level
{
  _LOW = 0,
  _HIGH = 1
} button_logic_level;

/**
 * Syatem auto lock is stored into bit, so never add more possibilities
 */
typedef enum system_auto_lock
{
  DISABLED = 0,
  ENABLED = 1
} system_auto_lock;

class FlashValues
{
public:
  FlashValues();

  /**
   * GETTERS
   */

  bit_t getWifiMode();

  bit_t getButtonLogicLevel();

  uint16_t getButtonHoldTimeout();

  uint16_t getButtonHoldPeriod();

  button_mode getButtonMode(button_index buttonIndex);

  light_dimmer getOutputDimmer(output_index outputIndex);

  uint8_t getButtonsDimmer();

  IPAddress *getConnectionIp();

  uint16_t getConnectionPort();

  String getWifiSSID();

  String getWifiPassword();

  String getButtonAlias(button_index buttonIndex);

  String getRoomAlias();

  uint8_t getButtonRemoteAddress(button_index buttonIndex);

  uint8_t getMyAddress();

  uint16_t getSystemLockTimeout();

  uint16_t getSytemLockPeriod();

  bit_t getSystemAutoLock();

  bit_t getButtonHandleLock(button_index buttonIndex);

  bit_t getEraseFlashBit();

  bit_t getDefaultConfigFlashBit();

  /**
   * SETTERS
   */

  flash_value_result setWifiMode(wifi_mode mode);

  flash_value_result setButtonLogicLevel(button_logic_level logicLevel);

  flash_value_result setButtonHoldTimeout(seconds_step time);

  flash_value_result setButtonHoldPeriod(seconds_step time);

  flash_value_result setButtonMode(button_index buttonIndex, button_mode mode);

  flash_value_result setOutputDimmer(output_index outputIndex, light_dimmer dimmer);

  flash_value_result setButtonsDimmer(light_dimmer dimmer);

  flash_value_result setConnectionIp(uint8_t ip4_addr1, uint8_t ip4_addr2, uint8_t ip4_addr3, uint8_t ip4_addr4);

  flash_value_result setConnectionPort(uint16_t port);

  flash_value_result setWifiSSID(String ssid);

  flash_value_result setWifiPassword(String password);

  flash_value_result setButtonAlias(button_index buttonIndex, String name);

  flash_value_result setRoomAlias(String name);

  flash_value_result setButtonRemoteAddress(button_index buttonIndex, uint8_t address);

  flash_value_result setMyAddress(uint8_t address);

  flash_value_result setSystemLockTimeout(seconds_step time);

  flash_value_result setSytemLockPeriod(seconds_step time);

  flash_value_result setSystemAutoLock(system_auto_lock autoLock);

  flash_value_result setButtonHandleLock(button_index buttonIndex, bit_t hold);

  flash_value_result setEraseFlashBit(bit_t erase);

  flash_value_result setDefaultConfigFlashBit(bit_t config);

  /**
   * COMMON
   */

  bool restoreDefaults();

  bool validateFlash();

private:
  Flash *FlashMemory;

  void setFlashBitWriteControllers(bool resumeWrite = true);
};

#endif