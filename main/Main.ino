/*
Main program for ESP8266 home automation
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
#include "Main.h"

const char *ssid[100] = {0};
const char *password = WIFI_AP_PASSWORD;

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
  #if SERIAL_DEBUG
    Serial.println("Client requested root");
  #endif
}

void setup()
{
  EEPROM.begin(512);
  
/*
  SYSTEM INFO
*/
#if SERIAL_DEBUG
  Serial.begin(115200); 
  Serial.println("");
  Serial.println("System setup...");
  Serial.print("SDK version: ");
  Serial.println(system_get_sdk_version());
  Serial.print("ChipID: ");
  Serial.println(system_get_chip_id()); 
  // Serial.print("MEM: ");
  // system_set_os_print(1);
  // system_print_meminfo();
  Serial.print("Power: ");
  Serial.println(system_get_vdd33());
#endif

  WiFi.macAddress(_myMac);
  
  if (!checkFlash())
    reboot(true);

  //String wifiMacString = ;
    
  WiFi.softAP(wifiMacString, password);
  IPAddress myIP = WiFi.softAPIP();

  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);  // Turn off light when plugedin 

/*
  BUTTONS
*/
#if SERIAL_DEBUG
  Serial.print("Initializing buttons: ");
  if (BUTTON_COUNT) {
    Serial.print(BUTTON_COUNT);
    Serial.print(": ");
  } else {
    Serial.println("None");
  }
#endif

  for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
#if SERIAL_DEBUG
    if (btnCnt)
      Serial.print(", ");
    Serial.print(btnCnt);
    Serial.print("->");
    Serial.print(_buttonPin[btnCnt]);
#endif
    _touchButton[btnCnt] = new TouchButtonModule(_buttonPin[btnCnt], btnCnt, BUTTON_DEFAULT_LEVEL, BUTTON_HOLD_TIMEOUT, BUTTON_HOLD_PERIOD);
  }
#if SERIAL_DEBUG
    Serial.println("");
#endif
}


// the loop function runs over and over again forever
void loop()
{
  server.handleClient();
}

/*
 * Flash default format
 * 
 * [MAC][MODE]
 * 
 * MAC[6] - WiFi MAC address
 * MODE[1] - Access Point / Client (default - WIFI_OPERATION_MODE_AP)
*/
bool checkFlash(void)
{
  bool isPartitionOk_1 = calcFlashCrc(1) == getFlashCrc(1);
  bool isPartitionOk_2 = calcFlashCrc(2) == getFlashCrc(2);
  
  if (!isPartitionOk_1 && !isPartitionOk_2) {
#if SERIAL_DEBUG
    Serial.println("Wrong flash format, formatting...");
#endif
    uint8_t boolConfigs = 0;
    bitWrite(boolConfigs, 0, WIFI_OPERATION_MODE_AP);
    bitWrite(boolConfigs, 1, SERIAL_DEBUG);
    EEPROM.write(0, boolConfigs);


    String defaultSsid = WIFI_AP_SSID + WiFi.macAddress();
    setSsid(&defaultSsid, false);

    String defaultPassword = WIFI_AP_PASSWORD;
    setPassword(&defaultPassword, false);
  
    for (int i = 7 ; i < 512 ; i++)
      EEPROM.write(i, 0x00);
    
    if (EEPROM.commit()) {
#if SERIAL_DEBUG
      Serial.println("Flash formatted successfully");
#endif
    } else {
#if SERIAL_DEBUG
      Serial.println("ERROR Formatting flash");
#endif
      return false;
    }
  }

  return true;
}

void reboot(bool critical)
{
#if SERIAL_DEBUG 
  if (critical)
    Serial.print("[CRITICAL] ");
  Serial.println("System reboot requested");
#endif
  ESP.restart(); 
}

String *getSsid()
{

}

/**
 * Primary SSID is stored in EEPROM on address bettwen 50 - 149
 * Backup SSID is stored from 250 to 349
*/
bool setSsid(String *ssid, bool setCrc)
{
  int ssidLen = ssid->length() + 50;
  for (int i = 50 ; i < 150 ; i++) {
    if (i < ssidLen)
      EEPROM.write(i, ssid->charAt(i));
    else
      EEPROM.write(i, 0x00);
  }

  if (!EEPROM.commit())
    return false;
  if (setCrc && !setFlashCrc(1))
    return false;
    
  for (int i = 250 ; i < 350 ; i++) {
    if (i < ssidLen)
      EEPROM.write(i, ssid->charAt(i));
    else
      EEPROM.write(i, 0x00);
  }

  if (setCrc) {
    if (!EEPROM.commit() || !setFlashCrc(2))
      return false;
    else
      return true;
  }

  return EEPROM.commit();
}

/**
 * Primary password is stored in EEPROM on address bettwen 150 - 249
 * Backup password is stored from 350 to 449
*/
bool setPassword(String *pass, bool setCrc)
{
  int passLen = pass->length() + 150;
  for (int i = 150 ; i < 250 ; i++) {
    if (i < passLen)
      EEPROM.write(i, pass->charAt(i));
    else
      EEPROM.write(i, 0x00);
  }

  if (!EEPROM.commit())
    return false;
  if (setCrc && !setFlashCrc(1))
    return false;

  for (int i = 350 ; i < 450 ; i++) {
    if (i < passLen)
      EEPROM.write(i, pass->charAt(i));
    else
      EEPROM.write(i, 0x00);
  }

  if (setCrc) {
    if (!EEPROM.commit() || !setFlashCrc(2))
      return false;
    else
      return true;
  }

  return EEPROM.commit();
}

bool calcCrcFlash(unsigned char partition)
{
  unsigned int crcLimit = 504;
  unsigned int flashCounter;
  unsigned int crcCounter;

  if (partition == 1) {
    flashCounter = 0;
    crcCounter = 504;
  }
  else if (partition == 2) {
    flashCounter = 250;
    crcCounter = 508;
  } else {
    return false;
  }

  unsigned short crc = getFlashCrc(partition);

  for (unsigned char i = 0 ; i < 4 ; i++) {
    EEPROM.write(i, 0x00);
    crcCounter ++;
  }
}

uint32_t calcFlashCrc(uint8_t partition)
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

uint32_t getFlashCrc(uint8_t partition)
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

void uInt2Char(uint8_t (&buf)[4], uint32_t value)
{
  buf[0] = value;
  buf[1] = value >> 8;
  buf[2] = value >> 16;
  buf[3] = value >> 24;
}

uint32_t char2UInt(const uint8_t (&buf)[4])
{
    uint32_t u0 = buf[0], u1 = buf[1], u2 = buf[2], u3 = buf[3];
    uint32_t uval = u0 | (u1 << 8) | (u2 << 16) | (u3 << 24);
    return uval;
}

bool setFlashCrc(uint8_t partition)
{
  uint16_t flashCounter;
  uint8_t flashCrc[4];

  if (partition == 1)
    flashCounter = 504;
  else if (partition == 2)
    flashCounter = 508;
  else
    return 0;

  uint32_t crc = calcFlashCrc(partition);
  if (!crc)
    return false;

  uInt2Char(flashCrc, crc);

  for (uint8_t i = 0 ; i < 4 ; i++, flashCounter++)
    EEPROM.write(flashCounter, flashCrc[i]);

  return EEPROM.commit();
}