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
button_mode FlashValues::getButtonMode(button_index buttonIndex)
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
uint8_t FlashValues::getOutputDimmer(output_index outputIndex)
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
String FlashValues::getButtonAlias(button_index buttonIndex)
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
uint8_t FlashValues::getButtonRemoteAddress(button_index buttonIndex)
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
bit_t FlashValues::getButtonHandleHold(button_index buttonIndex)
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
bit_t FlashValues::getDefaultConfigFlashBit()
{
  SERIALPRINTLN("FlaVal->getDefaultConfigFlashBit()");

  return ByteManipulation::getBitOnByte(
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_3),
      EEPROM_ADDRESS_CONFIG_AREA_3_FLASH_DEFAULTS);
}

/**
 * @brief Set Wifi Access Point or Client mode
 *
 * @param mode
 * @return flash_value_result
 */
flash_value_result FlashValues::setWifiMode(wifi_mode mode)
{
  SERIALPRINT("FlaVal->setWifiMode(");
  SERIALPRINT(mode, BIN);
  SERIALPRINTLN(")");

  return this->FlashMemmory->writeBit(
             EEPROM_ADDRESS_CONFIG_AREA_1,
             EEPROM_ADDRESS_CONFIG_AREA_1_WIFI_MODE,
             mode)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set Touch button hardware logic level
 *
 * @param logicLevel
 * @return flash_value_result
 */
flash_value_result FlashValues::setButtonLogicLevel(button_logic_level logicLevel)
{
  SERIALPRINT("FlaVal->setButtonLogicLevel(");
  SERIALPRINT(logicLevel, BIN);
  SERIALPRINTLN(")");

  return this->FlashMemmory->writeBit(
             EEPROM_ADDRESS_CONFIG_AREA_1,
             EEPROM_ADDRESS_CONFIG_AREA_1_BUTTON_LOGIC_LEVEL,
             logicLevel)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set time seconds step to set button holded in miliseconds
 *
 * @param time
 * @return flash_value_result
 */
flash_value_result FlashValues::setButtonHoldTimeout(seconds_step time)
{
  SERIALPRINT("FlaVal->setButtonHoldTimeout(");
  SERIALPRINT(time, DEC);
  SERIALPRINTLN(")");

  uint8_t flashByte =
      this->FlashMemmory->readByte(EEPROM_ADDRESS_BUTTON_HOLD_TIME);
  ByteManipulation::setFirstNibbleOnByte(&flashByte, time);

  return this->FlashMemmory->writeByte(
             EEPROM_ADDRESS_BUTTON_HOLD_TIME,
             flashByte)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set time seconds step to let buttons in hold mode (after holded)
 *
 * @param time
 * @return flash_value_result
 */
flash_value_result FlashValues::setButtonHoldPeriod(seconds_step time)
{
  SERIALPRINT("FlaVal->setButtonHoldPeriod(");
  SERIALPRINT(time, DEC);
  SERIALPRINTLN(")");

  uint8_t flashByte =
      this->FlashMemmory->readByte(EEPROM_ADDRESS_BUTTON_HOLD_TIME);
  ByteManipulation::setSecondNibbleOnByte(&flashByte, time);

  return this->FlashMemmory->writeByte(
             EEPROM_ADDRESS_BUTTON_HOLD_TIME,
             flashByte)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set button mode configuration
 *
 * @param buttonIndex
 * @param mode
 * @return flash_value_result
 */
flash_value_result FlashValues::setButtonMode(button_index buttonIndex, button_mode mode)
{
  SERIALPRINT("FlaVal->setButtonMode(");
  SERIALPRINT(buttonIndex);
  SERIALPRINT("->");
  SERIALPRINT(mode);
  SERIALPRINTLN(")");

  bool writeResult = false;

  if (buttonIndex == 0)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_BUTTON_1_MODE);
    ByteManipulation::setFirstNibbleOnByte(&flashByte, mode);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_BUTTON_1_MODE,
        flashByte);
  }
  else if (buttonIndex == 1)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_BUTTON_2_MODE);
    ByteManipulation::setSecondNibbleOnByte(&flashByte, mode);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_BUTTON_2_MODE,
        flashByte);
  }
  else if (buttonIndex == 2)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_BUTTON_3_MODE);
    ByteManipulation::setFirstNibbleOnByte(&flashByte, mode);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_BUTTON_3_MODE,
        flashByte);
  }
  else if (buttonIndex == 3)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_BUTTON_4_MODE);
    ByteManipulation::setSecondNibbleOnByte(&flashByte, mode);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_BUTTON_4_MODE,
        flashByte);
  }
  else if (buttonIndex == 4)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_BUTTON_5_MODE);
    ByteManipulation::setFirstNibbleOnByte(&flashByte, mode);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_BUTTON_5_MODE,
        flashByte);
  }
  else if (buttonIndex == 5)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_BUTTON_6_MODE);
    ByteManipulation::setSecondNibbleOnByte(&flashByte, mode);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_BUTTON_6_MODE,
        flashByte);
  }

  return writeResult ? flash_value_result::OK : flash_value_result::ERROR;
}

/**
 * @brief Set output configured dimmer
 *
 * @param outputIndex
 * @param dimmer
 * @return flash_value_result
 */
flash_value_result FlashValues::setOutputDimmer(output_index outputIndex, light_dimmer dimmer)
{
  SERIALPRINT("FlaVal->setOutputDimmer(");
  SERIALPRINT(outputIndex);
  SERIALPRINT("->");
  SERIALPRINT(dimmer);
  SERIALPRINTLN(")");

  bool writeResult = false;

  if (outputIndex == 0)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_OUTPUT_1_DIMMER);
    ByteManipulation::setFirstNibbleOnByte(&flashByte, dimmer);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_OUTPUT_1_DIMMER,
        flashByte);
  }
  else if (outputIndex == 1)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_OUTPUT_2_DIMMER);
    ByteManipulation::setSecondNibbleOnByte(&flashByte, dimmer);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_OUTPUT_2_DIMMER,
        flashByte);
  }
  else if (outputIndex == 2)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_OUTPUT_3_DIMMER);
    ByteManipulation::setFirstNibbleOnByte(&flashByte, dimmer);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_OUTPUT_3_DIMMER,
        flashByte);
  }
  else if (outputIndex == 3)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_OUTPUT_4_DIMMER);
    ByteManipulation::setSecondNibbleOnByte(&flashByte, dimmer);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_OUTPUT_4_DIMMER,
        flashByte);
  }
  else if (outputIndex == 4)
  {
    uint8_t flashByte =
        this->FlashMemmory->readByte(EEPROM_ADDRESS_OUTPUT_5_DIMMER);
    ByteManipulation::setFirstNibbleOnByte(&flashByte, dimmer);

    writeResult = this->FlashMemmory->writeByte(
        EEPROM_ADDRESS_OUTPUT_5_DIMMER,
        flashByte);
  }
  return writeResult ? flash_value_result::OK : flash_value_result::ERROR;
}

/**
 * @brief Set buttons led dimmer value
 *
 * @param dimmer
 * @return flash_value_result
 */
flash_value_result FlashValues::setButtonsDimmer(light_dimmer dimmer)
{
  SERIALPRINT("FlaVal->setButtonsDimmer(");
  SERIALPRINT(dimmer, DEC);
  SERIALPRINTLN(")");

  uint8_t flashByte =
      this->FlashMemmory->readByte(EEPROM_ADDRESS_BUTTONS_LED_DIMMER);
  ByteManipulation::setSecondNibbleOnByte(&flashByte, dimmer);

  return this->FlashMemmory->writeByte(
             EEPROM_ADDRESS_BUTTONS_LED_DIMMER,
             flashByte)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set connection (mananger / own) IP
 *
 * @param ip4_addr1
 * @param ip4_addr2
 * @param ip4_addr3
 * @param ip4_addr4
 * @return flash_value_result
 */
flash_value_result FlashValues::setConnectionIp(uint8_t ip4_addr1, uint8_t ip4_addr2, uint8_t ip4_addr3, uint8_t ip4_addr4)
{
  SERIALPRINT("FlaVal->setConnectionIp(");
  SERIALPRINT(ip4_addr1, DEC);
  SERIALPRINT(".");
  SERIALPRINT(ip4_addr2, DEC);
  SERIALPRINT(".");
  SERIALPRINT(ip4_addr3, DEC);
  SERIALPRINT(".");
  SERIALPRINT(ip4_addr4, DEC);
  SERIALPRINTLN(")");

  this->FlashMemmory->pauseWrite();

  (void)this->FlashMemmory->writeByte(
      EEPROM_ADDRESS_CONNECTION_IP_1, ip4_addr1);
  (void)this->FlashMemmory->writeByte(
      EEPROM_ADDRESS_CONNECTION_IP_2, ip4_addr2);
  (void)this->FlashMemmory->writeByte(
      EEPROM_ADDRESS_CONNECTION_IP_3, ip4_addr3);
  (void)this->FlashMemmory->writeByte(
      EEPROM_ADDRESS_CONNECTION_IP_4, ip4_addr4);

  return this->FlashMemmory->resumeWrite()
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set connection (mananger / own) port
 *
 * @param port
 * @return flash_value_result
 */
flash_value_result FlashValues::setConnectionPort(uint16_t port)
{
  SERIALPRINT("FlaVal->setConnectionPort(");
  SERIALPRINT(port, DEC);
  SERIALPRINTLN(")");

  this->FlashMemmory->pauseWrite();

  (void)this->FlashMemmory->writeByte(
      EEPROM_ADDRESS_CONNECTION_PORT_1,
      (uint8_t)(port >> 8));
  (void)this->FlashMemmory->writeByte(
      EEPROM_ADDRESS_CONNECTION_PORT_2,
      (uint8_t)(port & 0x00FF));

  return this->FlashMemmory->resumeWrite()
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set Wifi SSID
 *
 * @param ssid
 * @return flash_value_result
 */
flash_value_result FlashValues::setWifiSSID(String ssid)
{
  SERIALPRINT("FlaVal->setWifiSSID(");
  SERIALPRINT(ssid);
  SERIALPRINTLN(")");

  uint8_t size = ssid.length();
  if (!size || size > 50)
  {
    return flash_value_result::WRONG_STRING_SIZE;
  }

  return this->FlashMemmory->writeString(
             EEPROM_ADDRESS_WIFI_SSID, ssid, size)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Get Wifi Password
 *
 * @param password
 * @return flash_value_result
 */
flash_value_result FlashValues::setWifiPassword(String password)
{
  SERIALPRINT("FlaVal->setWifiPassword(");
  SERIALPRINT(password);
  SERIALPRINTLN(")");

  uint8_t size = password.length();
  if (!size || size > 50)
  {
    return flash_value_result::WRONG_STRING_SIZE;
  }

  return this->FlashMemmory->writeString(
             EEPROM_ADDRESS_WIFI_PASSWORD, password, size)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set button alias (name)
 *
 * @param buttonIndex
 * @param name
 * @return flash_value_result
 */
flash_value_result FlashValues::setButtonAlias(button_index buttonIndex, String name)
{
  SERIALPRINT("FlaVal->setButtonAlias(");
  SERIALPRINT(buttonIndex);
  SERIALPRINT("->");
  SERIALPRINT(name);
  SERIALPRINTLN(")");

  uint8_t size = name.length();
  if (!size || size > 20)
  {
    return flash_value_result::WRONG_STRING_SIZE;
  }

  if (buttonIndex == 0)
  {
    return this->FlashMemmory->writeString(
               EEPROM_ADDRESS_BUTTON_1_ALIAS, name, size)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 1)
  {
    return this->FlashMemmory->writeString(
               EEPROM_ADDRESS_BUTTON_2_ALIAS, name, size)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 2)
  {
    return this->FlashMemmory->writeString(
               EEPROM_ADDRESS_BUTTON_3_ALIAS, name, size)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 3)
  {
    return this->FlashMemmory->writeString(
               EEPROM_ADDRESS_BUTTON_4_ALIAS, name, size)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 4)
  {
    return this->FlashMemmory->writeString(
               EEPROM_ADDRESS_BUTTON_5_ALIAS, name, size)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 5)
  {
    return this->FlashMemmory->writeString(
               EEPROM_ADDRESS_BUTTON_6_ALIAS, name, size)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }

  return flash_value_result::ERROR;
}

/**
 * @brief Set room alias (name)
 *
 * @param name
 * @return flash_value_result
 */
flash_value_result FlashValues::setRoomAlias(String name)
{
  SERIALPRINT("FlaVal->setRoomAlias(");
  SERIALPRINT(name);
  SERIALPRINTLN(")");

  uint8_t size = name.length();
  if (!size || size > 20)
  {
    return flash_value_result::WRONG_STRING_SIZE;
  }

  return this->FlashMemmory->writeString(
             EEPROM_ADDRESS_ROOM_ALIAS, name, size)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set button alias
 *
 * @param buttonIndex
 * @param address
 * @return flash_value_result
 */
flash_value_result FlashValues::setButtonRemoteAddress(button_index buttonIndex, uint8_t address)
{
  SERIALPRINT("FlaVal->setButtonRemoteAddress(");
  SERIALPRINT(buttonIndex);
  SERIALPRINT("->");
  SERIALPRINT(address, HEX);
  SERIALPRINTLN(")");

  if (buttonIndex == 0)
  {
    return this->FlashMemmory->writeByte(
               EEPROM_ADDRESS_REMOTE_ADDRESS_1, address)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 1)
  {
    return this->FlashMemmory->writeByte(
               EEPROM_ADDRESS_REMOTE_ADDRESS_2, address)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 2)
  {
    return this->FlashMemmory->writeByte(
               EEPROM_ADDRESS_REMOTE_ADDRESS_3, address)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 3)
  {
    return this->FlashMemmory->writeByte(
               EEPROM_ADDRESS_REMOTE_ADDRESS_4, address)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 4)
  {
    return this->FlashMemmory->writeByte(
               EEPROM_ADDRESS_REMOTE_ADDRESS_5, address)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 5)
  {
    return this->FlashMemmory->writeByte(
               EEPROM_ADDRESS_REMOTE_ADDRESS_6, address)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }

  return flash_value_result::ERROR;
}

/**
 * @brief Set defined device address
 *
 * @note Never set it by your own. Only manager can set the device address
 *
 * @param address
 * @return flash_value_result
 */
flash_value_result FlashValues::setMyAddress(uint8_t address)
{
  SERIALPRINT("FlaVal->setMyAddress(");
  SERIALPRINT(address, HEX);
  SERIALPRINTLN(")");

  return this->FlashMemmory->writeByte(
             EEPROM_ADDRESS_DEVICE_ADDRESS, address)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set time (step seconds) that system must be locked after hold lock button(s)
 *
 * @param time
 * @return flash_value_result
 */
flash_value_result FlashValues::setSystemLockTimeout(seconds_step time)
{
  SERIALPRINT("FlaVal->setSystemLockTimeout(");
  SERIALPRINT(time, DEC);
  SERIALPRINTLN(")");

  uint8_t flashByte =
      this->FlashMemmory->readByte(EEPROM_ADDRESS_SYSTEM_LOCK_TIMEOUT);
  ByteManipulation::setFirstNibbleOnByte(&flashByte, time);

  return this->FlashMemmory->writeByte(
             EEPROM_ADDRESS_SYSTEM_LOCK_TIMEOUT,
             flashByte)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set time (step seconds) of inactivity to auto lock the system
 *
 * @param time
 * @return flash_value_result
 */
flash_value_result FlashValues::setSytemLockPeriod(seconds_step time)
{
  SERIALPRINT("FlaVal->setSytemLockPeriod(");
  SERIALPRINT(time, DEC);
  SERIALPRINTLN(")");

  uint8_t flashByte =
      this->FlashMemmory->readByte(EEPROM_ADDRESS_SYSTEM_LOCK_PERIOD);
  ByteManipulation::setSecondNibbleOnByte(&flashByte, time);

  return this->FlashMemmory->writeByte(
             EEPROM_ADDRESS_BUTTON_HOLD_TIME,
             flashByte)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set system auto (buttons) lock configuration
 *
 * @param autoLock
 * @return flash_value_result
 */
flash_value_result FlashValues::setSystemAutoLock(system_auto_lock autoLock)
{
  SERIALPRINT("FlaVal->setSystemAutoLock(");
  SERIALPRINT(autoLock, BIN);
  SERIALPRINTLN(")");

  return this->FlashMemmory->writeBit(
             EEPROM_ADDRESS_CONFIG_AREA_2,
             EEPROM_ADDRESS_CONFIG_AREA_2_AUTO_LOCK,
             autoLock)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set the button (buttonIndex) to control lock/unlock system
 *
 * @param buttonIndex
 * @param hold
 * @return flash_value_result
 */
flash_value_result FlashValues::setButtonHandleHold(button_index buttonIndex, bit_t hold)
{
  SERIALPRINT("FlaVal->setButtonHandleHold(");
  SERIALPRINT(buttonIndex);
  SERIALPRINT("->");
  SERIALPRINT(hold, BIN);
  SERIALPRINTLN(")");

  bool writeResult = false;

  if (buttonIndex == 0)
  {
    return this->FlashMemmory->writeBit(
               EEPROM_ADDRESS_CONFIG_AREA_2,
               EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_1,
               hold)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 1)
  {
    return this->FlashMemmory->writeBit(
               EEPROM_ADDRESS_CONFIG_AREA_2,
               EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_2,
               hold)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 2)
  {
    return this->FlashMemmory->writeBit(
               EEPROM_ADDRESS_CONFIG_AREA_2,
               EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_3,
               hold)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 3)
  {
    return this->FlashMemmory->writeBit(
               EEPROM_ADDRESS_CONFIG_AREA_2,
               EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_4,
               hold)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 4)
  {
    return this->FlashMemmory->writeBit(
               EEPROM_ADDRESS_CONFIG_AREA_2,
               EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_5,
               hold)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }
  else if (buttonIndex == 5)
  {
    return this->FlashMemmory->writeBit(
               EEPROM_ADDRESS_CONFIG_AREA_2,
               EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_6,
               hold)
               ? flash_value_result::OK
               : flash_value_result::ERROR;
  }

  return flash_value_result::ERROR;
}

/**
 * @brief Set EEPROM erase flash value
 *
 * @note This work by setting this to a diferent value hater
 * than currently recorded
 *
 * @param erase
 * @return flash_value_result
 */
flash_value_result FlashValues::setEraseFlashBit(bit_t erase)
{
  SERIALPRINT("FlaVal->setEraseFlashBit(");
  SERIALPRINT(erase, BIN);
  SERIALPRINTLN(")");

  bit_t current = ByteManipulation::getBitOnByte(
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_3),
      EEPROM_ADDRESS_CONFIG_AREA_3_ERASE_FLASH);

  if (current == erase)
  {
    return flash_value_result::SAME_VALUE_NOT_ALLOWED;
  }

  return this->FlashMemmory->writeBit(
             EEPROM_ADDRESS_CONFIG_AREA_3,
             EEPROM_ADDRESS_CONFIG_AREA_3_ERASE_FLASH,
             erase)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Set EEPROM reset configuration value
 *
 * @note This work by setting this to a diferent value hater
 * than currently recorded
 *
 * @param config
 * @return flash_value_result
 */
flash_value_result FlashValues::setDefaultConfigFlashBit(bit_t config)
{
  SERIALPRINT("FlaVal->setDefaultConfigFlashBit(");
  SERIALPRINT(config, BIN);
  SERIALPRINTLN(")");

  bit_t current = ByteManipulation::getBitOnByte(
      this->FlashMemmory->readByte(EEPROM_ADDRESS_CONFIG_AREA_3),
      EEPROM_ADDRESS_CONFIG_AREA_3_FLASH_DEFAULTS);

  if (current == config)
  {
    return flash_value_result::SAME_VALUE_NOT_ALLOWED;
  }

  return this->FlashMemmory->writeBit(
             EEPROM_ADDRESS_CONFIG_AREA_3,
             EEPROM_ADDRESS_CONFIG_AREA_3_FLASH_DEFAULTS,
             config)
             ? flash_value_result::OK
             : flash_value_result::ERROR;
}

/**
 * @brief Restores all flash values to their defaults
 *
 * @return true
 * @return false
 */
bool FlashValues::restoreDefaults()
{
  SERIALPRINT("FlaVal->restoreDefaults()");

  bool flashWriteResult = false;

  this->FlashMemmory->pauseWrite();

  /**
   * Set all values by calling their setters
   */
  (void)this->setWifiMode(FLASH_DEFAULT_WIFI_MODE);

  (void)this->setButtonLogicLevel(FLASH_DEFAULT_BUTTON_LOGIC_LEVEL);

  (void)this->setButtonHoldTimeout((seconds_step)FLASH_DEFAULT_BUTTON_HOLD_TIME_OUT);

  (void)this->setButtonHoldPeriod((seconds_step)FLASH_DEFAULT_BUTTON_HOLD_PERIOD);

  (void)this->setButtonsDimmer((light_dimmer)FLASH_DEFAULT_BUTTONS_DIMMER);

  (void)this->setConnectionIp(
      FLASH_DEFAULT_IP_OCTET_1,
      FLASH_DEFAULT_IP_OCTET_2,
      FLASH_DEFAULT_IP_OCTET_3,
      FLASH_DEFAULT_IP_OCTET_4);

  (void)this->setConnectionPort(FLASH_DEFAULT_MANAGER_PORT);

  (void)this->setWifiSSID(FLASH_DEFAULT_WIFI_PRE_SSID + WiFi.macAddress());

  (void)this->setWifiPassword(FLASH_DEFAULT_WIFI_PASSWORD);

  (void)this->setRoomAlias(FLASH_DEFAULT_KEYPAD_ALIAS);

  (void)this->setMyAddress(FLASH_DEFAULT_DEVICE_ADDRESS);

  (void)this->setSystemLockTimeout((seconds_step)FLASH_DEFAULT_LOCK_TIME_OUT);

  (void)this->setSytemLockPeriod((seconds_step)FLASH_DEFAULT_AUTO_LOCK_PERIOD);

  (void)this->setSystemAutoLock(system_auto_lock::ENABLED);

  /**
   * Set outputs default values
   *
   * @note values are the same (0%) since we do not want to damage a hardware
   * that's incompatible with PWM
   */
  for (uint8_t i = 0; i < TOTAL_OUTPUTS_COUNT; i++)
  {
    if (i == 0)
    {
      (void)this->setOutputDimmer(
          output_index::OUTPUT_1,
          (light_dimmer)FLASH_DEFAULT_OUTPUT);
    }
    else if (i == 1)
    {
      (void)this->setOutputDimmer(
          output_index::OUTPUT_2,
          (light_dimmer)FLASH_DEFAULT_OUTPUT);
    }
    else if (i == 2)
    {
      (void)this->setOutputDimmer(
          output_index::OUTPUT_3,
          (light_dimmer)FLASH_DEFAULT_OUTPUT);
    }
    else if (i == 3)
    {
      (void)this->setOutputDimmer(
          output_index::OUTPUT_4,
          (light_dimmer)FLASH_DEFAULT_OUTPUT);
    }
    else if (i == 4)
    {
      (void)this->setOutputDimmer(
          output_index::OUTPUT_5,
          (light_dimmer)FLASH_DEFAULT_OUTPUT);
    }
  }

  ;

  /**
   * Set buttons default values
   */
  for (uint8_t i = 0; i < TOTAL_BUTTONS_COUNT; i++)
  {
    if (i == 0)
    {
      (void)this->setButtonMode(
          button_index::BUTTON_1,
          (button_mode)FLASH_DEFAULT_BUTTON_1_MODE);

      (void)this->setButtonAlias(
          button_index::BUTTON_1,
          FLASH_DEFAULT_BUTTON_1_ALIAS);

      (void)this->setButtonRemoteAddress(
          button_index::BUTTON_1,
          FLASH_DEFAULT_REMOTE_ADDRESS);

      (void)this->setButtonHandleHold(
          button_index::BUTTON_1,
          ByteManipulation::getBitOnByte(
              FLASH_DEFAULT_SYSTEM_LOCK_CONFIG,
              EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_1));
    }
    else if (i == 1)
    {
      (void)this->setButtonMode(
          button_index::BUTTON_2,
          (button_mode)FLASH_DEFAULT_BUTTON_2_MODE);

      (void)this->setButtonAlias(
          button_index::BUTTON_2,
          FLASH_DEFAULT_BUTTON_2_ALIAS);

      (void)this->setButtonRemoteAddress(
          button_index::BUTTON_2,
          FLASH_DEFAULT_REMOTE_ADDRESS);

      (void)this->setButtonHandleHold(
          button_index::BUTTON_2,
          ByteManipulation::getBitOnByte(
              FLASH_DEFAULT_SYSTEM_LOCK_CONFIG,
              EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_2));
    }
    else if (i == 2)
    {
      (void)this->setButtonMode(
          button_index::BUTTON_3,
          (button_mode)FLASH_DEFAULT_BUTTON_3_MODE);

      (void)this->setButtonAlias(
          button_index::BUTTON_3,
          FLASH_DEFAULT_BUTTON_3_ALIAS);

      (void)this->setButtonRemoteAddress(
          button_index::BUTTON_3,
          FLASH_DEFAULT_REMOTE_ADDRESS);

      (void)this->setButtonHandleHold(
          button_index::BUTTON_3,
          ByteManipulation::getBitOnByte(
              FLASH_DEFAULT_SYSTEM_LOCK_CONFIG,
              EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_3));
    }
    else if (i == 3)
    {
      (void)this->setButtonMode(
          button_index::BUTTON_4,
          (button_mode)FLASH_DEFAULT_BUTTON_4_MODE);

      (void)this->setButtonAlias(
          button_index::BUTTON_4,
          FLASH_DEFAULT_BUTTON_4_ALIAS);

      (void)this->setButtonRemoteAddress(
          button_index::BUTTON_4,
          FLASH_DEFAULT_REMOTE_ADDRESS);

      (void)this->setButtonHandleHold(
          button_index::BUTTON_4,
          ByteManipulation::getBitOnByte(
              FLASH_DEFAULT_SYSTEM_LOCK_CONFIG,
              EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_4));
    }
    else if (i == 4)
    {
      (void)this->setButtonMode(
          button_index::BUTTON_5,
          (button_mode)FLASH_DEFAULT_BUTTON_5_MODE);

      (void)this->setButtonAlias(
          button_index::BUTTON_5,
          FLASH_DEFAULT_BUTTON_5_ALIAS);

      (void)this->setButtonRemoteAddress(
          button_index::BUTTON_5,
          FLASH_DEFAULT_REMOTE_ADDRESS);

      (void)this->setButtonHandleHold(
          button_index::BUTTON_5,
          ByteManipulation::getBitOnByte(
              FLASH_DEFAULT_SYSTEM_LOCK_CONFIG,
              EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_5));
    }
    else if (i == 5)
    {
      (void)this->setButtonMode(
          button_index::BUTTON_6,
          (button_mode)FLASH_DEFAULT_BUTTON_6_MODE);

      (void)this->setButtonAlias(
          button_index::BUTTON_6,
          FLASH_DEFAULT_BUTTON_6_ALIAS);

      (void)this->setButtonRemoteAddress(
          button_index::BUTTON_6,
          FLASH_DEFAULT_REMOTE_ADDRESS);

      (void)this->setButtonHandleHold(
          button_index::BUTTON_6,
          ByteManipulation::getBitOnByte(
              FLASH_DEFAULT_SYSTEM_LOCK_CONFIG,
              EEPROM_ADDRESS_CONFIG_AREA_2_LOCK_BUTTON_6));
    }
  }

  flashWriteResult = this->FlashMemmory->resumeWrite();

  if (flashWriteResult)
    SERIALPRINTLN(" - OK");
  else
    SERIALPRINTLN(" - ERROR");

  return flashWriteResult ? flash_value_result::OK : flash_value_result::ERROR;
}

/**
 * @brief Validates flash CRC
 *
 * @note This function compares flash write controllers too in order
 * to perform flash erase / restore default configs during flash
 * validation process
 *
 * @return true if is valid
 * @return false otherwise
 */
bool FlashValues::validateFlash()
{
  if (!this->FlashMemmory->isCrcValid() ||
      this->getEraseFlashBit() != MANUAL_SET_BIT_TO_ERASE_FLASH)
  {
    this->FlashMemmory->erase();
    this->setFlashBitWriteControllers(false);
    this->restoreDefaults();
    return false;
  }
  else if (this->getDefaultConfigFlashBit() != MANUAL_SET_BIT_TO_RESTORE_FLASH_DEFAULTS)
  {
    this->setFlashBitWriteControllers(false);
    this->restoreDefaults();
    return false;
  }

  return true;
}

/**
 * @brief Use this to automaticaly set erase and defaults flash bit
 *
 * @param restoreWrite Remember to retore write if not done inside
 */
void FlashValues::setFlashBitWriteControllers(bool resumeWrite)
{
  this->FlashMemmory->pauseWrite();
  this->setEraseFlashBit(MANUAL_SET_BIT_TO_ERASE_FLASH);
  this->setDefaultConfigFlashBit(MANUAL_SET_BIT_TO_ERASE_FLASH);

  if (resumeWrite)
    (void)this->FlashMemmory->resumeWrite();
}