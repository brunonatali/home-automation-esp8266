
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

////////////////////////
//////////////////////// CONFIGURATION
////////////////////////


// Enable/disable serial debug
#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 1
#endif

#if SERIAL_DEBUG
#define SERIALPRINT Serial.print
#else
#define SERIALPRINT
#endif


/*
 *  Set the amount of buttons you must use
 *  Default value is 6
 *  Obs. A number gratter than 6 takes no effect, will assime 6 
*/
#if SERIAL_DEBUG
#define BUTTON_COUNT 5
#else
#define BUTTON_COUNT 6
#endif

/**
 * Hardware version
 * v0 - Buttons are mirrored
 * v1 - Buttons corrected
*/
#define HW_VERSION 1


// Stores button`s class handler
TouchButtonModule *_touchButton[6];


// Stores output pin Lighter class
Lighter *_outputPinController[5];


// Stores EEPROM handler class
FlashMan *_flash;



// Stores Wi-Fi stack handler class
MCommunication *_communication;


/*
 * Store touch buttons pins
 * Using 6 pins to handle 6 touch buttons
 * Default value is {1, 4, 10, 12, 13, 14}
*/
#if SERIAL_DEBUG
int _buttonPin[5] = {4, 10, 14, 12, 13};
#else
#if HW_VERSION == 0
int _buttonPin[6] = {1, 4, 10, 14, 12, 13};
#elif HW_VERSION == 1
int _buttonPin[6] = {14, 12, 13, 1, 4, 10};
#endif
#endif

// Stores configured mode for each button
int _buttonMode[6] = {0xFA}; // Not configured


// Stores output pin config
#if SERIAL_DEBUG
int _outputPin[5] = {0, 0, 5, 15, 16};
#else
int _outputPin[5] = {0, 3, 5, 15, 16};
#endif


// Configure if output pin is dimmable or not
bool _buttonPinDimmable[5] = {true, true, true, true, false};

/**
 * System used
 * Enable / disable dimmer
*/
bool dimmerEnabled = false;

/**
 * System used
 * Store dimmer button index
*/
uint8_t dimmerButtonIndex = 0xFF;

/**
 * System used
 * Store holded button number
*/
uint8_t dimmerHoldButton = 0xFF;


// Enable ALL libs debug
#if SERIAL_DEBUG
#ifndef TOUCH_MODULE_DEBUG
#define TOUCH_MODULE_DEBUG true
#endif
#ifndef BOUNCE_FX_DEBUG
#define BOUNCE_FX_DEBUG true
#endif
#endif


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