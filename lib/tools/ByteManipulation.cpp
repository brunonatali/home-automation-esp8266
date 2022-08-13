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

#include "ByteManipulation.h"

ByteManipulation::ByteManipulation(uint8_t *byte)
{
  this->_byte = byte;
}

/**
 * @brief Set the Bit on `this->_byte` by given bitIndex
 *
 * @param bitIndex
 * @param bitValue
 */
void ByteManipulation::setBit(uint8_t bitIndex, bit_t bitValue)
{
  (void)ByteManipulation::setBitOnByte(this->_byte, bitIndex, bitValue);
}

/**
 * @brief Get Bit on `this->_byte` by given bitIndex
 *
 * @param bitIndex
 * @return true
 * @return false
 */
bit_t ByteManipulation::getBit(uint8_t bitIndex)
{
  return ByteManipulation::getBitOnByte(*this->_byte, bitIndex);
}

/**
 * @brief Set value at first nibble on `this->_byte`
 *
 * @param value
 */
void ByteManipulation::setFirstNibble(uint8_t value)
{
  (void)ByteManipulation::setFirstNibbleOnByte(this->_byte, value);
}

/**
 * @brief Returns first nibble value
 *
 * @return uint8_t
 */
uint8_t ByteManipulation::getFirstNibble()
{
  return ByteManipulation::getFirstNibbleOnByte(*this->_byte);
}

/**
 * @brief Set value at second nibble on `this->_byte`
 *
 * @param value
 */
void ByteManipulation::setSecondNibble(uint8_t value)
{
  (void)ByteManipulation::setSecondNibbleOnByte(this->_byte, value);
}

/**
 * @brief Returns second nibble value
 *
 * @return uint8_t
 */
uint8_t ByteManipulation::getSecondNibble()
{
  return ByteManipulation::getSecondNibbleOnByte(*this->_byte);
}

/**
 * @brief Configure bit on bitIndex with given bitValue
 *
 * @param byte Byte to configure
 * @param bitIndex Which bit position on Byte need to be changed
 * @param bitValue 0 or 1
 * @return uint8_t Modified Byte
 */
void ByteManipulation::setBitOnByte(uint8_t *byte, uint8_t bitIndex, bit_t bitValue)
{
  *byte = bitValue ? bitSet(*byte, bitIndex) : bitClear(*byte, bitIndex);
}

/**
 * @brief Get bit value from given Byte
 *
 * @param byte
 * @param bitIndex
 * @return true
 * @return false
 */
bit_t ByteManipulation::getBitOnByte(uint8_t byte, uint8_t bitIndex)
{
  return ((byte) >> (bitIndex)) & 0x01;
}

/**
 * @brief Configure first (less significative) 4 bits on given byte
 *
 * @param byte
 * @param value 4 bits value (0x00 - 0x0F)
 */
void ByteManipulation::setFirstNibbleOnByte(uint8_t *byte, uint8_t value)
{
  *byte = (value & 0x0F) | (*byte & 0xF0);
}

/**
 * @brief Return firts nibble
 *
 * @param byte
 * @return uint8_t
 */
uint8_t ByteManipulation::getFirstNibbleOnByte(uint8_t byte)
{
  return byte & 0x0F;
}

/**
 * @brief Configure second (most significative) 4 bits on given byte
 *
 * @param byte
 * @param value 4 bits value (0x00 - 0x0F)
 */
void ByteManipulation::setSecondNibbleOnByte(uint8_t *byte, uint8_t value)
{
  *byte = ((value << 4) & 0xF0) | (*byte & 0x0F);
}

/**
 * @brief Return Second nibble
 *
 * @param byte
 * @return uint8_t
 */
uint8_t ByteManipulation::getSecondNibbleOnByte(uint8_t byte)
{
  return (byte >> 4) & 0x0F;
}