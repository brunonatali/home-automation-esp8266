/**
 * EEPROM (Flash) manager.
 * Copyright (c) 2021 Bruno Natali - b010010010n@gmail.com
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
  Set button default logic level (untouched)
*/
#define BUTTON_DEFAULT_LEVEL HIGH

/*
  Button hold timeout
  Indicates time in miliseconds that will trigger button is holding
*/
#define BUTTON_HOLD_TIMEOUT 2 // 2 sec

/*
  Button hold period
  Indicates time in miliseconds that button still in holded state before return to default
*/
#define BUTTON_HOLD_PERIOD 15 // 15 sec

/*
  Enable/disable serial debug
*/
#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 0
#endif

class FlashMan
{
  public:
    FlashMan();
    bool setSsid(String ssid, bool setCrc = true);
    bool setWifiPassword(String pass, bool setCrc = true);
    bool setButtonHoldTO(uint8_t seconds, bool setCrc = true);
    bool setButtonHoldPeriod(uint8_t seconds, bool setCrc = true);
    bool setWifiMode(uint8_t mode, bool setCrc = true);
    bool setButtonLogicLevel(uint8_t level, bool setCrc = true);
    bool setButtonLightMode(uint8_t button, uint8_t mode, bool setCrc = true);
    bool setButtonDimmer(uint8_t button, uint8_t value, bool setCrc = true);

    String getSsid(void);
    String getWifiPass(void);
    uint8_t getButtonHoldTO(bool force = false);
    uint8_t getButtonHoldPeriod(bool force = false);
    uint8_t getWifiMode(bool force = false);
    uint8_t getButtonLogicLevel(bool force = false);
    uint8_t getButtonLightMode(uint8_t button, bool force = false);
    uint8_t getButtonDimmer(uint8_t button, bool force = false);

    static bool writeString(String str, int p1Index, int p2Index, const int size, bool setCrc = true);
    static bool writeByte(uint8_t byte, int p1Index, int p2Index, bool setCrc = true);
    static bool writeBit(uint8_t *boolConfigs, uint8_t bit, uint8_t byteIndex, int p1Index, int p2Index, bool setCrc = true);

    static String getString(int index, uint8_t size);
    static char getByte(int index);

    uint8_t boolConfigs;

    private:
    String ssid;
    String wifiPass;
    uint8_t wifiMode;
    uint8_t btnLogicLevel;
    uint8_t btnHoldTimeOut;
    uint8_t btnHoldPeriod;
    uint8_t btnMode[6];
    uint8_t btnDimmer[6];

    static bool copyFlashPartition(uint8_t from, bool setCrc = true);
    static uint32_t calcFlashCrc(uint8_t partition);
    static bool setFlashCrc(uint8_t partition);
    static uint32_t getFlashCrc(uint8_t partition);
};
#endif