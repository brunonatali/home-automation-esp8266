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

//uint8_t *generalBuffer[100] = {0x00};


String ssid;

String wifiPass;

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
  #if SERIAL_DEBUG
    Serial.println("Client requested root");
  #endif
}

void setup()
{
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

  _flash = new FlashMan();

  WiFi.macAddress(_myMac);
  
  //if (!checkFlash())
  //  reboot(true);
    
  WiFi.softAP(_flash->getSsid(), _flash->getWifiPass());
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


void reboot(bool critical)
{
#if SERIAL_DEBUG 
  if (critical)
    Serial.print("[CRITICAL] ");
  Serial.println("System reboot requested");
#endif
  ESP.restart(); 
}

/*
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
*/





