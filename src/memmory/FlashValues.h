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

#include "IPAddress.h"

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
  DISABLED_BY_SOFTWARE = LIGHT_MODE_STEP_DISABLED_BY_SOFTWARE,
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

  button_mode getButtonMode(uint8_t buttonIndex);

  uint8_t getOutputDimmer(uint8_t outputIndex);

  uint8_t getButtonsDimmer();

  IPAddress *getConnectionIp();

  uint16_t getConnectionPort();

  String getWifiSSID();

  String getWifiPassword();

  String getButtonAlias(uint8_t buttonIndex);

  String getRoomAlias();

  uint8_t getButtonRemoteAddress(uint8_t buttonIndex);

  uint8_t getMyAddress();

  uint16_t getSystemLockTimeout();

  uint16_t getSytemLockPeriod();

  bit_t getSystemAutoLock();

  bit_t getButtonHandleHold(uint8_t buttonIndex);

  bit_t getEraseFlashBit();

  bit_t getSetFlashDefaultsBit();

private:
  Flash *FlashMemmory;
};

#endif