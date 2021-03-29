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

#ifndef FlashMan_h
#define FlashMan_h

#include <Arduino.h>
#include <string.h>
#include <HardwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>

#include "CRC32.h"
#include "Converters.h"

/*
  Access Point default password
*/
#define WIFI_AP_PASSWORD "semsenha"

/*
  Access Point default SSID prepend
*/
#define WIFI_AP_SSID "BN_"

/*
  Access Point mode
*/
#define WIFI_OPERATION_MODE_AP 0

/*
  WiFi client mode
*/
#define WIFI_OPERATION_MODE_CLIENT 1

/*
  Enable/disable serial debug
*/
#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 1
#endif

class FlashMan
{
  public:
    FlashMan();
    bool setSsid(String ssid, bool setCrc = true);
    bool setWifiPassword(String pass, bool setCrc);
    String getSsid(void);
    String getWifiPass(void);

    static bool writeString(String str, int p1Index, int p2Index, const int size, bool setCrc = true);
    static uint32_t calcFlashCrc(uint8_t partition);
    static bool setFlashCrc(uint8_t partition);
    static uint32_t getFlashCrc(uint8_t partition);
    static String getString(int index, uint8_t size);

    private:
    String ssid;
    String wifiPass;
};
#endif