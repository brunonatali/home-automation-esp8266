/**
 * Deal with wifi and webserver / socket communication
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

/**
 * This class aims to manage the module's Wi-Fi stack, create and manage 
 * communication sockets and provide a web page for communication 
*/
class MCommunication
{
  public:
    /**
     * Instantiate class
     * 
     * @param ssid Wi-Fi SSID
     * @param password Wi-Fi Password
    */
    MCommunication(String ssid = "", String password = "");

    /**
     * Just turn off Wi-Fi on destructs
    */
    ~MCommunication(void);

    /**
     * Configure Wi-Fi mode
     * The Web page handler was designed on main.ino, reffer to ../main.ino->handleWebServerRequest()
     * 
     * @note Wi-Fi modes are Access Point and Client, but the last one was not implemented yet
     * 
     * @param mode Mode must be WIFI_OPERATION_MODE_AP | WIFI_OPERATION_MODE_CLIENT
     * @param ssid
     * @param password
     * 
     * @returns If could set mode
    */
    bool setWifiMode(uint8_t mode, String ssid = "", String password = "");

    /**
     * Configures Wi-Fi IP for AP or static IP if is on client mode
     * 
     * @param local 
     * @param gateway 
     * @param subnet
     * 
     * @returns If could configure IP
    */
    bool setWifiIp(IPAddress *local, IPAddress *gateway, IPAddress *subnet);

    /**
     * This will start Wi-Fi stack with previously configered settings 
     * 
     * @returns If it was started
    */
    bool startWifi(void);

    /**
     * This will stop Wi-Fi stack
     * 
     * @returns If it was stoped
    */
    bool stopWifi(void);

    /**
     * Restart will call stopWifi() and startWifi()
     * 
     * @returns if two was completed successfully
    */
    bool restartWifi(void);

    /**
     * This function is only for debug propouses
     * 
     * @note if SERIAL_DEBUG is not declared or not 1, will do nothing
    */
    void getWifiStatus(void);

    /**
     * Just configure the Wi-Fi debug interface to Serial
    */
    void setWifiSerialDebug(void);

    // Store ESP8266WebServer stack
    ESP8266WebServer *webServer;

    /**
     * This variable stores entire web page
     * Before make software reffer to ../web/page.html and to ../web/page-min.html
    */
    String localWebPage;

  private:
    IPAddress WifiIpLocal;
    IPAddress WifiIpGateway;
    IPAddress WifiIpSubnet;

    uint8_t WifiMode;
    String WifiSssid;
    String WifiPassword;

};

/**
 * This functions was declared outside class because ESP8266WebServer do not suport classes
 * stored in main.ino
 * This function is relative to main project
 * */ 
void handleWebServerRoot(void);
void handleWebServerSetOnOff(void);
void handleWebServerConfig(void);
#endif
