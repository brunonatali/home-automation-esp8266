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

//WiFiServer wifiServer(80);


void setup()
{
  EEPROM.begin(512);
  WiFi.macAddress(_myMac);
  
  if (!checkFlash())
    reboot(true);
    
  WiFi.softAP("1234", WIFI_AP_PASSWORD);
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
  /*
    Chech 6 first bytes of flash, if is different than expected means it is a brand new module, than 
    a default envirolnment will be writed.
  */
  if (EEPROM.read(0) != _myMac[0] || EEPROM.read(1) != _myMac[1] || EEPROM.read(2) != _myMac[2] || 
      EEPROM.read(3) != _myMac[3] || EEPROM.read(4) != _myMac[4] || EEPROM.read(5) != _myMac[5]) {
        
#if SERIAL_DEBUG
    Serial.println("Wrong flash format, formatting...");
#endif
    EEPROM.write(0, _myMac[0]);
    EEPROM.write(1, _myMac[1]);
    EEPROM.write(2, _myMac[2]);
    EEPROM.write(3, _myMac[3]);
    EEPROM.write(4, _myMac[4]);
    EEPROM.write(5, _myMac[5]);
    EEPROM.write(6, WIFI_OPERATION_MODE_AP);
  
    for (int i = 0 ; i < 512; i++)
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

void reboot(bool critical = false)
{
#if SERIAL_DEBUG 
  if (critical)
    Serial.print("[CRITICAL] ");
  Serial.println("System reboot requested");
#endif
  ESP.restart(); 
}
