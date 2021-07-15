
////////////////////////
//////////////////////// INCLUDES & DECLARATIONS
////////////////////////

#include <Arduino.h>
#include <string.h>

// WiFi
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Interactions
#include <ESP8266WebServer.h>


// Personal LIBs
#include "MCommunication.h"
#include "GenericCallbacks.h"
#include "TouchButtonModule.h"
#include "FlashMan.h"
#include "Lighter.h"

// Stores button`s class handler
TouchButtonModule *_touchButton[6];

// Stores output pin Lighter class
Lighter *_outputPinController[5];

// Stores EEPROM handler class
FlashMan *_flash;

// Stores Wi-Fi stack handler class
MCommunication *_communication;

////////////////////////
//////////////////////// FUNCTIONS
////////////////////////

/**
 * This function must be called when touch button is touched
 * Basically used by TouchButtonModule
 * [CALLBACK-ONLY]
 *  
 * @param self is not used for now - use nullptr instead
 * @param buttonNumber Button number relative to index number [0 - 5]
*/
static ICACHE_RAM_ATTR bool buttonClicked(void* self, uint16 buttonNumber);

/**
 * This function must be called when touch button is holded
 * Basically used by TouchButtonModule
 * [CALLBACK-ONLY]
 * 
 * @param self is not used for now - use nullptr instead
 * @param buttonNumber Button number relative to index number [0 - 5]
*/
static ICACHE_RAM_ATTR bool buttonHolded(void* self, uint16 buttonNumber);

/**
 * This function must be called when touch button is un-holded triggered by
 * untouch event
 * Basically used by TouchButtonModule
 * [CALLBACK-ONLY]
 * 
 * @param self is not used for now - use nullptr instead
 * @param buttonNumber Button number relative to index number [0 - 5]
*/
static ICACHE_RAM_ATTR bool buttonUnholded(void* self, uint16 buttonNumber);

/**
 * Handle only the dimmer button click events
 * [CALLBACK-ONLY]
 * 
 * @note dimmer button must be driven just for edge triggers (FALL | RISE)
 * 
 * @param self is not used for now - use nullptr instead
 * @param buttonNumber Button number relative to index number [0 - 5]
*/
static ICACHE_RAM_ATTR void dimmerButtonClicked(void);

/**
 * Enables dimmer button 
 * This function will attach interrupts relative to configured dimmer pin
 * 
 * @note Ensure to configureButton() before call this
*/
void enableDimmerButton(void);

/**
 * Disables any dimmer button attached or not
*/
void disableDimmerButton(void);

/**
 * Configures button
 * Modes table
 * - 1 => Events will be redirected to output 1
 * - 2 => Events will be redirected to output 2
 * - 3 => Events will be redirected to output 3
 * - 4 => Events will be redirected to output 4
 * - 5 => Events will be redirected to output 5
 * - 0xC8 => Dimmer
 * - 0xFE => Button Disabled by software. Not available for config, here just for refference
 * - 0xFF => Button disabled
 * 
 * @param buttonIndex 
 * @param mode Mode must be [1 - 5 | 0xC8 or 0xFF]
 * @param setFlash If set to true, will store current configuration into EEPROM
*/
void configureButton(uint8_t buttonIndex, uint8_t mode, bool setFlash = true);

/**
 * Returns buttons status ON or OFF
 * 
 * @param buttonIndex 
 * @param dimmable [1 | 2] If dimmable already calculated 
 * 
 * @returns 1 | 0 
*/
bool getSimpleOnOffButtonValue(uint8_t buttonIndex, uint8_t dimmable = 2);

/**
 * Return object with configured info of provided buttonindex
 * 
 * @param buttonindex
 * 
 * @returns Object {
 *    "d": <int> 0|1 [dimmerable],
 *    "dv": <int> 2 - 100 [dimmer value],
 *    "f": <int> 1 - 5 | C8 | FE | FF[mode],
 *    "s": <int> 0|1 [off | on]
 * }
*/
String getButtonJsonConfig(uint8_t buttonindex);

/**
 * Builds a list of buttons coonfig
 * 
 * @returns JSON formatted like: 
 * "1": {
 *    "d": <int> 0|1 [dimmerable],
 *    "dv": <int> 2 - 100 [dimmer value],
 *    "f": <int> 1 - 5 [mode],
 *    "s": <int> 0|1 [off | on]
 * },
 * "2" { ... }
*/
String getButtonsJsonList(void);

/**
 * This function will be called on each webserver request
 * used by MCommunication and configured at webServer->on("/", handleWebServerRequest)
 * 
 * @note requests must be done by root route
*/
void handleWebServerRoot(void);
void handleWebServerSetOnOff(void);
void handleWebServerConfig(void);


/**
 * Reset hardware
 * @param critical specify that reboot root cause is something critical
*/
void reboot(bool critical = false);