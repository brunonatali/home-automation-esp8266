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

#include "FlashValues.h"

FlashValues::FlashValues()
{
  this->FlashMemmory = Flash::getFlashMemoryInstance();
}

/**
 * @brief Get Wifi Access Point or Client mode
 *
 * @return bit_t
 */
bit_t FlashValues::getWifiMode()
{
  SERIALPRINTLN("FlaVal->getWifiMode()");

  return ByteManipulation::getBitOnByte(
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_1),
      EEPROM_ADDRESS_CONFIG_AREA_1_WIFI_MODE);
}

/**
 * @brief Get Touch button hardware logic level
 *
 * @return bit_t
 */
bit_t FlashValues::getButtonLogicLevel()
{
  SERIALPRINTLN("FlaVal->getButtonLogicLevel()");

  return ByteManipulation::getBitOnByte(
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_1),
      EEPROM_ADDRESS_CONFIG_AREA_1_BUTTON_LOGIC_LEVEL);
}

/**
 * @brief Get time (miliseconds) to set button holded in miliseconds
 *
 * @return uint16_t
 */
uint16_t FlashValues::getButtonHoldTimeout()
{
  SERIALPRINTLN("FlaVal->getButtonHoldTimeout()");

  return NIBLE_STEP_SECONDS[ByteManipulation::getFirstNibbleOnByte(
      this->FlashMemmory->readByte(
          EEPROM_ADDRESS_BUTTON_HOLD_TIME))];
}

/**
 * @brief Get time (miliseconds) to let buttons in hold mode (after holded)
 *
 * @return uint16_t
 */
uint16_t FlashValues::getButtonHoldPeriod()
{
  SERIALPRINTLN("FlaVal->getButtonHoldPeriod()");

  return NIBLE_STEP_SECONDS[ByteManipulation::getSecondNibbleOnByte(
      this->FlashMemmory->readByte(
          EEPROM_ADDRESS_BUTTON_HOLD_TIME))];
}

/**
 * @brief Get button mode configuration
 *
 * @return button_mode
 */
button_mode FlashValues::getButtonMode(uint8_t buttonIndex)
{
  SERIALPRINT("FlaVal->getButtonMode(");
  SERIALPRINT(buttonIndex);
  SERIALPRINTLN(")");

  if (buttonIndex == 0)
  {
    return (button_mode)ByteManipulation::getFirstNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_BUTTON_1_MODE));
  }
  else if (buttonIndex == 1)
  {
    return (button_mode)ByteManipulation::getSecondNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_BUTTON_2_MODE));
  }
  else if (buttonIndex == 2)
  {
    return (button_mode)ByteManipulation::getFirstNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_BUTTON_3_MODE));
  }
  else if (buttonIndex == 3)
  {
    return (button_mode)ByteManipulation::getSecondNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_BUTTON_4_MODE));
  }
  else if (buttonIndex == 4)
  {
    return (button_mode)ByteManipulation::getFirstNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_BUTTON_5_MODE));
  }
  else if (buttonIndex == 5)
  {
    return (button_mode)ByteManipulation::getSecondNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_BUTTON_6_MODE));
  }

  /*  ERROR - Wrong button index  */
  return button_mode::DISABLED;
}

/**
 * @brief Get output configured dimmer in percentage (0-100)
 *
 * @return uint8_t
 */
uint8_t FlashValues::getOutputDimmer(uint8_t outputIndex)
{
  SERIALPRINT("FlaVal->getOutputDimmer(");
  SERIALPRINT(outputIndex);
  SERIALPRINTLN(")");

  if (outputIndex == 0)
  {
    return NIBLE_STEP_PERCENTAGE[(light_dimmer)ByteManipulation::getFirstNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_OUTPUT_1_DIMMER))];
  }
  else if (outputIndex == 1)
  {
    return NIBLE_STEP_PERCENTAGE[(light_dimmer)ByteManipulation::getSecondNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_OUTPUT_2_DIMMER))];
  }
  else if (outputIndex == 2)
  {
    return NIBLE_STEP_PERCENTAGE[(light_dimmer)ByteManipulation::getFirstNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_OUTPUT_3_DIMMER))];
  }
  else if (outputIndex == 3)
  {
    return NIBLE_STEP_PERCENTAGE[(light_dimmer)ByteManipulation::getSecondNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_OUTPUT_4_DIMMER))];
  }
  else if (outputIndex == 4)
  {
    return NIBLE_STEP_PERCENTAGE[(light_dimmer)ByteManipulation::getFirstNibbleOnByte(
        this->FlashMemmory->readByte(
            EEPROM_ADDRESS_OUTPUT_5_DIMMER))];
  }

  /*  ERROR - Wrong button index  */
  return NIBLE_STEP_PERCENTAGE[light_dimmer::PERCENT_0];
}

/**
 * @brief Get buttons led dimmer value in percentage (0-100)
 *
 * @return uint8_t
 */
uint8_t FlashValues::getButtonsDimmer()
{
  SERIALPRINTLN("FlaVal->getButtonsDimmer()");

  return NIBLE_STEP_PERCENTAGE[(light_dimmer)ByteManipulation::getSecondNibbleOnByte(
      this->FlashMemmory->readByte(
          EEPROM_ADDRESS_BUTTONS_LED_DIMMER))];
}

/**
 * @brief Get instance of IPAddress used into Arduino Wifi
 *
 * @return IPAddress
 */
IPAddress *FlashValues::getConnectionIp()
{
  SERIALPRINTLN("FlaVal->getConnectionIp()");

  return new IPAddress(
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONNECTION_IP_1),
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONNECTION_IP_2),
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONNECTION_IP_3),
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONNECTION_IP_4));
}

/**
 * @brief Get connection port
 *
 * @return uint16_t
 */
uint16_t FlashValues::getConnectionPort()
{
  SERIALPRINTLN("FlaVal->getConnectionPort()");

  return 0xFFFF & (this->FlashMemmory->readByte(EEPROM_ADDRESS_CONNECTION_PORT_1) << 8) &
         this->FlashMemmory->readByte(EEPROM_ADDRESS_CONNECTION_PORT_2);
}

/**
 * @brief Get Wifi SSID
 *
 * @return String
 */
String FlashValues::getWifiSSID()
{
  SERIALPRINTLN("FlaVal->getWifiSSID()");

  return this->FlashMemmory->readString(EEPROM_ADDRESS_WIFI_SSID, 50);
}

/**
 * @brief Get Wifi Password
 *
 * @return String
 */
String FlashValues::getWifiPassword()
{
  SERIALPRINTLN("FlaVal->getWifiPassword()");

  return this->FlashMemmory->readString(EEPROM_ADDRESS_WIFI_PASSWORD, 50);
}

/**
 * @brief Get button alias
 *
 * @return String
 */
String FlashValues::getButtonAlias(uint8_t buttonIndex)
{
  SERIALPRINT("FlaVal->getButtonAlias(");
  SERIALPRINT(buttonIndex);
  SERIALPRINTLN(")");

  if (buttonIndex == 0)
  {
    return this->FlashMemmory->readString(EEPROM_ADDRESS_BUTTON_1_ALIAS, 20);
  }
  else if (buttonIndex == 1)
  {
    return this->FlashMemmory->readString(EEPROM_ADDRESS_BUTTON_2_ALIAS, 20);
  }
  else if (buttonIndex == 2)
  {
    return this->FlashMemmory->readString(EEPROM_ADDRESS_BUTTON_3_ALIAS, 20);
  }
  else if (buttonIndex == 3)
  {
    return this->FlashMemmory->readString(EEPROM_ADDRESS_BUTTON_4_ALIAS, 20);
  }
  else if (buttonIndex == 4)
  {
    return this->FlashMemmory->readString(EEPROM_ADDRESS_BUTTON_5_ALIAS, 20);
  }
  else if (buttonIndex == 5)
  {
    return this->FlashMemmory->readString(EEPROM_ADDRESS_BUTTON_6_ALIAS, 20);
  }

  return String("unknown");
}

/**
 * @brief Get room name
 *
 * @return String
 */
String FlashValues::getRoomAlias()
{
  SERIALPRINTLN("FlaVal->getRoomAlias()");

  return this->FlashMemmory->readString(EEPROM_ADDRESS_ROOM_ALIAS, 20);
}

/**
 * @brief Get button alias
 *
 * @return uint8_t
 */
uint8_t FlashValues::getButtonRemoteAddress(uint8_t buttonIndex)
{
  SERIALPRINT("FlaVal->getButtonRemoteAddress(");
  SERIALPRINT(buttonIndex);
  SERIALPRINTLN(")");

  if (buttonIndex == 0)
  {
    return this->FlashMemmory->readByte(EEPROM_ADDRESS_REMOTE_ADDRESS_1);
  }
  else if (buttonIndex == 1)
  {
    return this->FlashMemmory->readByte(EEPROM_ADDRESS_REMOTE_ADDRESS_2);
  }
  else if (buttonIndex == 2)
  {
    return this->FlashMemmory->readByte(EEPROM_ADDRESS_REMOTE_ADDRESS_3);
  }
  else if (buttonIndex == 3)
  {
    return this->FlashMemmory->readByte(EEPROM_ADDRESS_REMOTE_ADDRESS_4);
  }
  else if (buttonIndex == 4)
  {
    return this->FlashMemmory->readByte(EEPROM_ADDRESS_REMOTE_ADDRESS_5);
  }
  else if (buttonIndex == 5)
  {
    return this->FlashMemmory->readByte(EEPROM_ADDRESS_REMOTE_ADDRESS_6);
  }

  return 0x00;
}

/**
 * @brief Get defined device address
 *
 * @return uint8_t
 */
uint8_t FlashValues::getMyAddress()
{
  SERIALPRINTLN("FlaVal->getMyAddress()");

  return this->FlashMemmory->readByte(EEPROM_ADDRESS_DEVICE_ADDRESS);
}

/**
 * @brief Get time (miliseconds) to set system locked after hold lock button(s)
 *
 * @return uint16_t
 */
uint16_t FlashValues::getSystemLockTimeout()
{
  SERIALPRINTLN("FlaVal->getSystemLockTimeout()");

  return NIBLE_STEP_SECONDS[(seconds_step)ByteManipulation::getFirstNibbleOnByte(
      this->FlashMemmory->readByte(
          EEPROM_ADDRESS_SYSTEM_LOCK_TIMEOUT))];
}

/**
 * @brief Get time (miliseconds) of inactivity to auto lock the system
 *
 * @return uint16_t
 */
uint16_t FlashValues::getSytemLockPeriod()
{
  SERIALPRINTLN("FlaVal->getSytemLockPeriod()");

  return NIBLE_STEP_SECONDS[(seconds_step)ByteManipulation::getSecondNibbleOnByte(
      this->FlashMemmory->readByte(
          EEPROM_ADDRESS_SYSTEM_LOCK_PERIOD))];
}

/**
 * @brief Get system auto lock configuration
 *
 * @return bit_t
 */
bit_t FlashValues::getSystemAutoLock()
{
  SERIALPRINTLN("FlaVal->getSystemAutoLock()");

  return ByteManipulation::getBitOnByte(
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_2),
      EEPROM_ADDRESS_CONFIG_AREA_2_AUTO_LOCK);
}

/**
 * @brief Get if button must be holded to lock / unlock the system
 *
 * @return bit_t
 */
bit_t FlashValues::getButtonHandleHold(uint8_t buttonIndex)
{
  SERIALPRINT("FlaVal->getButtonHandleHold(");
  SERIALPRINT(buttonIndex);
  SERIALPRINTLN(")");

  if (buttonIndex == 0)
  {
    return ByteManipulation::getBitOnByte(
        this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_2),
        EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_1);
  }
  else if (buttonIndex == 1)
  {
    return ByteManipulation::getBitOnByte(
        this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_2),
        EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_2);
  }
  else if (buttonIndex == 2)
  {
    return ByteManipulation::getBitOnByte(
        this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_2),
        EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_3);
  }
  else if (buttonIndex == 3)
  {
    return ByteManipulation::getBitOnByte(
        this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_2),
        EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_4);
  }
  else if (buttonIndex == 4)
  {
    return ByteManipulation::getBitOnByte(
        this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_2),
        EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_5);
  }
  else if (buttonIndex == 5)
  {
    return ByteManipulation::getBitOnByte(
        this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_2),
        EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_6);
  }

  return 0x00;
}

/**
 * @brief Get EEPROM erase flash value
 *
 * @note This work by setting this to a diferent value hater
 * than currently recorded
 *
 * @return bit_t
 */
bit_t FlashValues::getEraseFlashBit()
{
  SERIALPRINTLN("FlaVal->getEraseFlashBit()");

  return ByteManipulation::getBitOnByte(
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_3),
      EEPROM_ADDRESS_CONFIG_AREA_3_ERASE_FLASH);
}

/**
 * @brief Get EEPROM reset configuration value
 *
 * @note This work by setting this to a diferent value hater
 * than currently recorded
 *
 * @return bit_t
 */
bit_t FlashValues::getSetFlashDefaultsBit()
{
  SERIALPRINTLN("FlaVal->getSetFlashDefaultsBit()");

  return ByteManipulation::getBitOnByte(
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_3),
      EEPROM_ADDRESS_CONFIG_AREA_3_FLASH_DEFAULTS);
}