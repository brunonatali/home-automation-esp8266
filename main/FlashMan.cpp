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

  EEPROM.begin(512);

  bool isPartitionOk_1 = FlashMan::calcFlashCrc(1) == FlashMan::getFlashCrc(1);
  bool isPartitionOk_2 = FlashMan::calcFlashCrc(2) == FlashMan::getFlashCrc(2);
  
  if (!isPartitionOk_1 && !isPartitionOk_2) {

#if SERIAL_DEBUG
    Serial.println("Wrong Flash format, formatting...");
#endif
    uint8_t boolConfigs = 0;
    bitWrite(boolConfigs, 0, WIFI_OPERATION_MODE_AP);
    //bitWrite(boolConfigs, 1, SERIAL_DEBUG);
    EEPROM.write(0, boolConfigs);
#if SERIAL_DEBUG
    Serial.println("\t-Bool configs: OK");
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
#endif
    if (EEPROM.commit()) {
#if SERIAL_DEBUG
      Serial.println("Flash formatted successfully");
#endif
    } else {
#if SERIAL_DEBUG
      Serial.println("ERROR Formatting flash");
#endif
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


String FlashMan::getSsid(void)
{
  return this->ssid;
}

String FlashMan::getWifiPass(void)
{
  return this->wifiPass;
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






