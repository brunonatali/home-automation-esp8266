/*
Flash (EEPROM) manager
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

#include "FlashMan.h"

FlashMan::FlashMan()
{
#if SERIAL_DEBUG
    Serial.println("Starting Flash");
#endif

  boolConfigs = 0x00;

  EEPROM.begin(512);

  bool isPartitionOk_1 = FlashMan::calcFlashCrc(1) == FlashMan::getFlashCrc(1);
  bool isPartitionOk_2 = FlashMan::calcFlashCrc(2) == FlashMan::getFlashCrc(2);
  
  if (!isPartitionOk_1 && !isPartitionOk_2) {

#if SERIAL_DEBUG
    Serial.println("Wrong Flash format, formatting...");
    Serial.print("\t-WiFi mode: ");
#endif
    bool wifiModeResult = FlashMan::setWifiMode(WIFI_OPERATION_MODE_AP, false);
#if SERIAL_DEBUG
    if (wifiModeResult)
      Serial.println("OK");
    else
      Serial.println("ERROR");
    Serial.println("\t-Button logic level -> ");
#endif
    bool btnLogicLevelResult = FlashMan::setButtonLogicLevel(BUTTON_DEFAULT_LEVEL, false);
#if SERIAL_DEBUG
    if (btnLogicLevelResult)
      Serial.println("OK");
    else
      Serial.println("ERROR");
    Serial.println("\t-Button Hold TO -> ");
#endif
    bool btnHoldTOResult = FlashMan::setButtonHoldTO(BUTTON_HOLD_TIMEOUT, false);
#if SERIAL_DEBUG
    if (btnHoldTOResult)
      Serial.println("OK");
    else
      Serial.println("ERROR");
    Serial.println("\t-Button Hold Period -> ");
#endif
    bool btnHoldPeriodResult = FlashMan::setButtonHoldPeriod(BUTTON_HOLD_PERIOD, false);
#if SERIAL_DEBUG
    if (btnHoldPeriodResult)
      Serial.println("OK");
    else
      Serial.println("ERROR");
    Serial.println("\t-WiFi SSID -> ");
#endif
    bool wifiSsidResult = FlashMan::setSsid(WIFI_AP_SSID + WiFi.macAddress(), false);
#if SERIAL_DEBUG
    if (wifiSsidResult)
      Serial.println("OK");
    else
      Serial.println("ERROR");
    Serial.print("\t-WiFi Password ->");
#endif
    bool wifiPassResult = FlashMan::setWifiPassword(WIFI_AP_PASSWORD, false);
#if SERIAL_DEBUG
    if (wifiPassResult)
      Serial.println("OK");
    else
      Serial.println("ERROR");
    Serial.print("\t-Light Mode->");
#endif
    bool lightModeResult = true;
    for (uint8_t i = 1; i <= 6; i++)
      bool lightModeResult = lightModeResult & FlashMan::setButtonLightMode(i, (i == 6 ? 0xC8 : i - 1), false); 
#if SERIAL_DEBUG
    if (lightModeResult)
      Serial.println("OK");
    else
      Serial.println("ERROR");
    Serial.print("\t-Light Dimm->");
#endif
    bool lightDimmResult = true;
    for (uint8_t i = 1; i <= 6; i++)
      bool lightDimmResult = lightDimmResult & FlashMan::setButtonDimmer(i, 0, false); // OFF by default
#if SERIAL_DEBUG
    if (lightDimmResult)
      Serial.println("OK");
    else
      Serial.println("ERROR");
#endif


    /**
     * SAVE CHANGES TO FLASH
    */
    if (EEPROM.commit()) {
#if SERIAL_DEBUG
      if (FlashMan::setFlashCrc(1) && FlashMan::setFlashCrc(2))
        Serial.println("Flash formatted successfully");
#else
      FlashMan::setFlashCrc(1);
      FlashMan::setFlashCrc(2);
#endif
    } else {
#if SERIAL_DEBUG
      Serial.println("ERROR Formatting flash");
#endif
    }
  } else { // Perform a partition recovery
    if (!isPartitionOk_1) 
      FlashMan::copyFlashPartition(2);
    else if (!isPartitionOk_2)
      FlashMan::copyFlashPartition(1);

    // Load values to variables
    (void) this->getButtonHoldTO(true);
    (void) this->getButtonHoldPeriod(true);
    (void) this->getWifiMode(true);
    (void) this->getButtonLogicLevel(true);
    // Get buttons value
    for (uint8_t i = 1; i <= 6; i++) {
      (void) this->getButtonLightMode(i, true);
      (void) this->getButtonDimmer(i, true);
    }
  }
}



bool FlashMan::setSsid(String ssid, bool setCrc)
{
#if SERIAL_DEBUG
    Serial.print("setSsid: ");
#endif

  if (FlashMan::writeString(ssid, 50, 300, 100, setCrc)) {
    this->ssid = ssid;
    return true;
  }

  return false;
}

bool FlashMan::setWifiPassword(String pass, bool setCrc)
{
#if SERIAL_DEBUG
    Serial.print("setWifiPassword: ");
#endif

  if (FlashMan::writeString(pass, 150, 400, 100, setCrc)) {
    this->wifiPass = pass;
    return true;
  }

  return false;
}

bool FlashMan::setButtonHoldTO(uint8_t seconds, bool setCrc)
{
#if SERIAL_DEBUG
    Serial.print("setButtonHoldTO: ");
#endif

  if (FlashMan::writeByte(seconds, 1, 251, setCrc)) {
    this->btnHoldTimeOut = seconds;
    return true;
  }

  return false;
}

bool FlashMan::setButtonHoldPeriod(uint8_t seconds, bool setCrc)
{
#if SERIAL_DEBUG
    Serial.print("setButtonHoldPeriod: ");
#endif

  if (FlashMan::writeByte(seconds, 2, 252, setCrc)) {
    this->btnHoldPeriod = seconds;
    return true;
  }

  return false;
}

bool FlashMan::setWifiMode(uint8_t mode, bool setCrc)
{
#if SERIAL_DEBUG
    Serial.print("setWifiMode: ");
#endif

  if (mode != WIFI_OPERATION_MODE_AP && mode != WIFI_OPERATION_MODE_CLIENT)
    return false;

  if (FlashMan::writeBit(&this->boolConfigs, mode, 0, 0, 250, setCrc)) {
    this->wifiMode = mode;
    return true;
  }

  return false;
  
}

bool FlashMan::setButtonLogicLevel(uint8_t level, bool setCrc)
{
#if SERIAL_DEBUG
    Serial.print("setButtonLogicLevel: ");
#endif

  if (level != HIGH && level != LOW)
    return false;

  if (FlashMan::writeBit(&this->boolConfigs, level, 1, 0, 250, setCrc)) {
    this->btnLogicLevel = level;
    return true;
  }

  return false;
  
}

bool FlashMan::setButtonLightMode(uint8_t button, uint8_t mode, bool setCrc)
{
#if SERIAL_DEBUG
    Serial.print("setButtonLightMode: ");
#endif

  if (button > 6 || (mode > 6 && (mode != 0xC8 && mode != 0xFE && mode != 0xFF))) { // C8 - dimmer, FE - sw disabled, FF - hard disabled
#if SERIAL_DEBUG
    Serial.print("ERROR ");
    Serial.print(button);
    Serial.print("-");
    Serial.println(mode, HEX);
#endif
    return false;
  }

  if (FlashMan::writeByte(mode, 4 + button, 254 + button, setCrc)) {
    this->btnMode[button - 1] = mode;
#if SERIAL_DEBUG
    Serial.println("OK");
    return true;
#endif
  }
#if SERIAL_DEBUG
  Serial.println("flaERR");
#endif
  return false;
}

bool FlashMan::setButtonDimmer(uint8_t button, uint8_t value, bool setCrc)
{
#if SERIAL_DEBUG
    Serial.print("setButtonDimmer: ");
#endif

  if (button > 6) {
#if SERIAL_DEBUG
    Serial.print("ERROR ");
    Serial.print(button);
    Serial.print("-");
    Serial.println(value, HEX);
#endif
    return false;
  }

  if (FlashMan::writeByte(value, 10 + button, 260 + button, setCrc)) {
    this->btnDimmer[button - 1] = value;
#if SERIAL_DEBUG
    Serial.println("OK");
    return true;
#endif
  }
#if SERIAL_DEBUG
  Serial.println("flaERR");
#endif
  return false;
}



String FlashMan::getSsid(void)
{
  if (this->ssid.length() == 0)
    this->ssid = FlashMan::getString(50, 100);
  return this->ssid;
}

String FlashMan::getWifiPass(void)
{
  if (this->wifiPass.length() == 0)
    this->wifiPass = FlashMan::getString(150, 100);
  return this->wifiPass;
}

uint8_t FlashMan::getButtonHoldTO(bool force)
{
  if (force)
    this->btnHoldTimeOut = FlashMan::getByte(1);
  return this->btnHoldTimeOut;
}

uint8_t FlashMan::getButtonHoldPeriod(bool force)
{
  if (force)
    this->btnHoldPeriod = FlashMan::getByte(2);
  return this->btnHoldPeriod;
}

uint8_t FlashMan::getWifiMode(bool force)
{
  if (force) {
    char byte = FlashMan::getByte(0);
    this->wifiMode = bitRead(byte, 0);
  }
  return this->wifiMode;
}

uint8_t FlashMan::getButtonLogicLevel(bool force)
{
  if (force) {
    char byte = FlashMan::getByte(0);
    this->btnLogicLevel = bitRead(byte, 1);
  }
  return this->btnLogicLevel;
}

uint8_t FlashMan::getButtonLightMode(uint8_t button, bool force)
{
  if (button > 6)
    return 0;

  if (force) 
    this->btnMode[button - 1] = FlashMan::getByte(4 + button);
    
  return this->btnMode[button - 1];
}

uint8_t FlashMan::getButtonDimmer(uint8_t button, bool force)
{
  if (button > 6)
    return 0;

  if (force) 
    this->btnDimmer[button - 1] = FlashMan::getByte(10 + button);
    
  return this->btnDimmer[button - 1];
}



bool FlashMan::writeString(String str, int p1Index, int p2Index, const int size, bool setCrc)
{
  int i;
  char buffer[size];
  for (i = 0 ; i < size ; i++)
    buffer[i] = 0x00;
  str.toCharArray(buffer, size);

#if SERIAL_DEBUG
    Serial.print("\tP1=>");
#endif

  for (i = 0 ; i < size ; i++, p1Index++) {
      EEPROM.write(p1Index, buffer[i]);
#if SERIAL_DEBUG
      Serial.print(buffer[i]);
#endif
  }
  
#if SERIAL_DEBUG
    Serial.print(": ");
    Serial.println(p1Index);
#endif

  if (!EEPROM.commit())
    return false;
  if (setCrc && !FlashMan::setFlashCrc(1))
    return false;

#if SERIAL_DEBUG
    Serial.print("\tP2=>");
#endif    

  for (int i = 0 ; i < size ; i++, p2Index++) {
      EEPROM.write(p2Index, buffer[i]);
#if SERIAL_DEBUG
      Serial.print(buffer[i]);
#endif
  }
  
#if SERIAL_DEBUG
    Serial.print(": ");
    Serial.println(p2Index);
#endif

  if (setCrc) {
    if (!EEPROM.commit() || !FlashMan::setFlashCrc(2))
      return false;
    else
      return true;
  }

  return EEPROM.commit();
}

bool FlashMan::writeByte(uint8_t byte, int p1Index, int p2Index, bool setCrc)
{
#if SERIAL_DEBUG
    Serial.print("\tP1=>");
    Serial.println(byte, HEX);
    Serial.print("-");
    Serial.println(p1Index);
#endif

  EEPROM.write(p1Index, byte);

  if (!EEPROM.commit())
    return false;
  if (setCrc && !FlashMan::setFlashCrc(1))
    return false;

#if SERIAL_DEBUG
    Serial.print("\tP2=>");
    Serial.println(byte, HEX);
    Serial.print("-");
    Serial.println(p2Index);
#endif

  EEPROM.write(p2Index, byte);

  if (setCrc) {
    if (!EEPROM.commit() || !FlashMan::setFlashCrc(2))
      return false;
    else
      return true;
  }

  return EEPROM.commit();
}

bool FlashMan::writeBit(uint8_t *boolConfigs, uint8_t bit, uint8_t byteIndex, int p1Index, int p2Index, bool setCrc)
{

  bitWrite(*boolConfigs, byteIndex, bit);

#if SERIAL_DEBUG
    Serial.print("\tP1=>");
    Serial.println(bit);
    Serial.print("-");
    Serial.println(byteIndex);
    Serial.print("-");
    Serial.println(p1Index);
#endif

  EEPROM.write(p1Index, *boolConfigs);

  if (!EEPROM.commit())
    return false;
  if (setCrc && !FlashMan::setFlashCrc(1))
    return false;

#if SERIAL_DEBUG
    Serial.print("\tP2=>");
    Serial.println(bit);
    Serial.print("-");
    Serial.println(byteIndex);
    Serial.print("-");
    Serial.println(p2Index);
#endif

  EEPROM.write(p2Index, *boolConfigs);

  if (setCrc) {
    if (!EEPROM.commit() || !FlashMan::setFlashCrc(2))
      return false;
    else
      return true;
  }

  return EEPROM.commit();
}


String FlashMan::getString(int index, uint8_t size)
{
#if SERIAL_DEBUG
  Serial.print("getString(): ");
#endif
  uint8_t i;
  char buff[size];
  for (i = 0 ; i < size ; i++)
    buff[i] = 0x00;
  char flashByte;

  for (i = 0 ; i < size ; i++, index++) {
    flashByte = EEPROM.read(index);
    if (!flashByte)
      break;
#if SERIAL_DEBUG
    Serial.print(flashByte);
#endif
    buff[i] = flashByte;
  }

#if SERIAL_DEBUG
  Serial.print('-');
  Serial.println(i);
#endif

  return String(buff);
}

char FlashMan::getByte(int index)
{
#if SERIAL_DEBUG
  Serial.print("getByte(): ");
#endif

  char flashByte = EEPROM.read(index);

#if SERIAL_DEBUG
    Serial.print(flashByte);
#endif

  return flashByte;
}

bool FlashMan::copyFlashPartition(uint8_t from, bool setCrc)
{
  if (from == 2) {
    uint16_t n = 250;
    for (uint8_t i = 0 ; i < 250 ; i++, n++)
      EEPROM.write(i, EEPROM.read(n));
  } else if (from == 1) {
    uint8_t n = 0;
    for (uint16_t i = 250 ; i < 500 ; i++, n++)
      EEPROM.write(i, EEPROM.read(n));
  } else {
    return false;
  }

  if (setCrc) {
    if (!EEPROM.commit() || !FlashMan::setFlashCrc(from == 1 ? 2 : 1))
      return false;
    else
      return true;
  }

  return EEPROM.commit();
}

uint32_t FlashMan::calcFlashCrc(uint8_t partition)
{
  uint8_t content[250];
  uint16_t flashCounter;

  if (partition == 1)
    flashCounter = 0;
  else if (partition == 2)
    flashCounter = 250;
  else
    return 0;

  for (uint8_t i = 0 ; i < 250 ; i++, flashCounter++)
    content[i] = EEPROM.read(flashCounter);

  return CRC32::calculate(content, 250);
}

bool FlashMan::setFlashCrc(uint8_t partition)
{
  uint16_t flashCounter;
  uint8_t flashCrc[4];

  if (partition == 1)
    flashCounter = 504;
  else if (partition == 2)
    flashCounter = 508;
  else
    return 0;

  uint32_t crc = FlashMan::calcFlashCrc(partition);
  if (!crc)
    return false;

  uInt2Char(flashCrc, crc);

  for (uint8_t i = 0 ; i < 4 ; i++, flashCounter++)
    EEPROM.write(flashCounter, flashCrc[i]);

  return EEPROM.commit();
}

uint32_t FlashMan::getFlashCrc(uint8_t partition)
{
  uint8_t crcContent[4];
  uint16_t flashCounter;

  if (partition == 1)
    flashCounter = 504;
  else if (partition == 2)
    flashCounter = 508;
  else
    return 0;

  for (uint8_t i = 0 ; i < 4 ; i++, flashCounter++)
    crcContent[i] = EEPROM.read(flashCounter);

  return char2UInt(crcContent);
}







