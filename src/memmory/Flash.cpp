/*
Flash (EEPROM) manager
Copyright (c) 2022 Bruno Natali - b010010010n@gmail.com

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

#include "ProjectConfig.h"

#include "Flash.h"

/**
 * @brief This class will extends original EEPROM class, bypassing some
 * verifications and increasing performance on buffer copying
 */
Flash::Flash()
{
  this->begin(EEPROM_FLASH_SIZE);
}

/**
 * @brief Write string to Flash
 *
 * @note String will ever end with \0 char, so never pass string that will
 * spill the memory space
 *
 * @param memAddress One of the 512 mem positions
 * @param str String to write on flash
 * @param setCrc
 * @return true
 * @return false
 */
bool Flash::writeString(uint8_t memAddress, String str, bool setCrc)
{
  if (memAddress >= EEPROM_FLASH_SIZE)
    return false;

  uint8_t i;
  int size = str.length();

  SERIALPRINT("\tFlaWS[");
  SERIALPRINT(memAddress);
  SERIALPRINT("] ");
  SERIALPRINT(str);

  str.toCharArray(reinterpret_cast<char *>(this->_data[memAddress]), size);
  this->_data[memAddress + size] = 0x00; // Add zeroe to end the string
  this->_dirty = true;

  if (!this->commit() || (setCrc && !Flash::setFlashCrc()))
  {
    SERIALPRINTLN(" - ERR");
    return false;
  }

  SERIALPRINTLN(" - OK");
  return true;
}

/**
 * @brief Writes a single byte into flash
 *
 * @param memAddress One of the 512 mem positions
 * @param byte yte to write
 * @param setCrc
 * @return true
 * @return false
 */
bool Flash::writeByte(uint8_t memAddress, uint8_t byte, bool setCrc)
{
  if (memAddress > EEPROM_FLASH_SIZE)
    return false;

  SERIALPRINT("\tFlaWBy[");
  SERIALPRINT(memAddress);
  SERIALPRINT("] ");
  SERIALPRINT(byte, HEX);

  this->_data[memAddress] = byte;
  this->_dirty = true;

  if (!this->commit() || (setCrc && !Flash::setFlashCrc()))
  {
    SERIALPRINTLN(" - ERR");
    return false;
  }

  SERIALPRINTLN(" - OK");
  return true;
}

/**
 * @brief Write a single bit into given byte
 *
 * @param memAddress One of the 512 mem positions
 * @param byteIndex One of the 8 {0-7} positions of an byte
 * @param value Bit value (0-1)
 * @param setCrc
 * @return true
 * @return false
 */
bool Flash::writeBit(uint8_t memAddress, uint8_t byteIndex, bool value, bool setCrc)
{
  if (memAddress > EEPROM_FLASH_SIZE)
    return false;

  SERIALPRINT("\tFlaWBi[");
  SERIALPRINT(memAddress);
  SERIALPRINT("] ");
  SERIALPRINT(byteIndex);
  SERIALPRINT(" = ");
  SERIALPRINT(value);

  bitWrite(this->_data[memAddress], byteIndex, value);
  this->_dirty = true;

  if (!this->commit() || (setCrc && !Flash::setFlashCrc()))
  {
    SERIALPRINTLN(" - ERR");
    return false;
  }

  SERIALPRINT(" > ");
  SERIALPRINT(this->_data[memAddress]);
  SERIALPRINTLN(" - OK");
  return true;
}

/**
 * @brief Reads string (till /0)
 *
 * @param memAddress
 * @return String
 */
String Flash::readString(uint8_t memAddress, uint8_t size)
{
  char buff[size] = {0x00};

  // Set final read
  size += memAddress;

  SERIALPRINT("\tFlaRS[");
  SERIALPRINT(memAddress);
  SERIALPRINT("] ");

  for (uint8_t i = 0; memAddress < size; memAddress++, i++)
  {
    // If /0 is found
    if (!this->_data[memAddress])
      break;

    SERIALPRINT(this->_data[memAddress]);
    buff[i] = this->_data[memAddress];
  }

  String readed = String(buff);

  SERIALPRINTLN(readed);
  return readed;
}

/**
 * @brief Reads a single byte from Flash on given address
 *
 * @param memAddress
 * @return uint8_t
 */
uint8_t Flash::readByte(uint8_t memAddress)
{
  SERIALPRINT("\tFlaRB[");
  SERIALPRINT(memAddress);
  SERIALPRINT("] ");

  SERIALPRINTLN(this->_data[memAddress], HEX);
  return this->_data[memAddress];
}

/**
 * @brief Calculates CRC32 for entire Flash
 *
 * @return uint32_t CRC result
 */
uint32_t Flash::calcFlashCrc()
{
  return CRC32::calculate(this->_data, EEPROM_FLASH_SIZE - 4);
}

/**
 * @brief Set Flash CRC32 based on it's entire size at last 4
 * char on Flash
 *
 * @return true
 * @return false
 */
bool Flash::setFlashCrc()
{
  uint8_t crcBuff[4];

  uint32_t calculatedCrc = Flash::calcFlashCrc();

  SERIALPRINT(" {S-");
  SERIALPRINT(calculatedCrc);
  SERIALPRINT("}");

  if (!calculatedCrc)
    return false;

  uInt2Char(crcBuff, calculatedCrc);
  for (uint8_t i = 0; i < 4; i++)
    this->_data[508 + i] = crcBuff[i];

  this->_dirty = true;

  return this->commit();
}

/**
 * @brief Get last 4 char on Flash as CRC value
 *
 * @return uint32_t
 */
uint32_t Flash::getFlashCrc()
{
  uint8_t crcBuff[4];
  for (uint8_t i = 0; i < 4; i++)
    crcBuff[i] = this->_data[508 + i];

  uint32_t parsedCrc = char2UInt(crcBuff);

  SERIALPRINT(" {G-");
  SERIALPRINT(parsedCrc);
  SERIALPRINT("}");

  return parsedCrc;
}

/**
 * @brief Write zeroes on entire flash
 */
void Flash::erase()
{
  for (uint8_t i = 0; i < EEPROM_FLASH_SIZE; i++)
    this->_data[i] = 0x00;

  this->_dirty = true;

  (void)this->commit();
}