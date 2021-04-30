/*
Developed to deal with wifi and webserver / socket communication
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

#ifndef MCommunication_h
#define MCommunication_h

#include <Arduino.h>

#include "GenericCallbacks.h"
#include "FlashMan.h"
extern "C"{
#include "user_interface.h"
}

// WiFi
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Interactions
#include <ESP8266WebServer.h>

class MCommunication
{
  public:
    MCommunication(String ssid = "", String password = "");
    ~MCommunication(void);

    bool setWifiMode(uint8_t mode, String ssid = "", String password = "");
    bool setWifiIp(IPAddress *local, IPAddress *gateway, IPAddress *subnet);

    bool startWifi(void);
    bool stopWifi(void);
    bool restartWifi(void);
    void getWifiStatus(void);

    void setWifiSerialDebug(bool set = true);

    //unholdcallback unholdCallback;
    //void *unholdCallbackArg;

  private:
    IPAddress WifiIpLocal;
    IPAddress WifiIpGateway;
    IPAddress WifiIpSubnet;

    uint8_t WifiMode;
    String WifiSssid;
    String WifiPassword;

    //static ICACHE_RAM_ATTR void bounceCallback(MCommunication* self);
};
#endif
