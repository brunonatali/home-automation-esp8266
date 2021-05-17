
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

/*
  Enable/disable serial debug
*/
#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 1
#endif

/*
  Set the amount of buttons you must use
  Default value is 6
  Obs. A number gratter than 6 takes no effect, will assime 6 
*/
#if SERIAL_DEBUG
#define BUTTON_COUNT 5
#else
#define BUTTON_COUNT 6
#endif

/*
  Stores button`s class handler
*/
TouchButtonModule *_touchButton[6];

/*
  Store output pin Lighter class
*/
Lighter *_outputPinController[5];

/**
 * 
*/
FlashMan *_flash;


/**
 * 
*/
MCommunication *_communication;


/*
  Store touch buttons pins
  Using 6 pins to handle 6 touch buttons
  Default value is {1, 4, 10, 12, 13, 14}
*/
#if SERIAL_DEBUG
int _buttonPin[5] = {4, 10, 14, 12, 13};
#else
int _buttonPin[6] = {1, 4, 10, 14, 12, 13};
#endif
/*
  Stores configured mode for each button
*/
int _buttonMode[6] = {0xFF}; // Disabled by default

/*
  Stores output pin config
*/
#if SERIAL_DEBUG
int _outputPin[5] = {0, 0, 5, 15, 16};
#else
int _outputPin[5] = {0, 3, 5, 15, 16};
#endif

/*
  Configure if output pin is dimmable or not
*/
bool _buttonPinDimmable[5] = {true, true, true, true, false};

/**
 * System used - Enable / disable dimmer
*/
bool dimmerEnabled = false;

/**
 * System used - Store dimmer button index
*/
uint8_t dimmerButtonIndex = 0xFF;

/**
 * System used - Store holded button number
*/
uint8_t dimmerHoldButton = 0xFF;

/*
  Enable ALL libs debug
*/
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



static ICACHE_RAM_ATTR bool buttonClicked(void* self, uint16 buttonNumber);
static ICACHE_RAM_ATTR bool buttonHolded(void* self, uint16 buttonNumber);
static ICACHE_RAM_ATTR bool buttonUnholded(void* self, uint16 buttonNumber);

static ICACHE_RAM_ATTR void dimmerButtonClicked(void);

void enableDimmerButton(void);

void disableDimmerButton(void);

void configureButton(uint8_t buttonIndex, uint8_t mode, bool setFlash = true);

String getButtonsJsonList(void);

void handleWebServerRequest(void);

/*
  Reboot system
  @var bool critical specify that reboot root cause is something critical
*/
void reboot(bool critical = false);