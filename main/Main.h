
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
#include "TouchButtonModule.h"
#include "FlashMan.h"
#include "Lighter.h"

////////////////////////
//////////////////////// CONFIGURATION
////////////////////////

/*
  Set the amount of buttons you must use
  Default value is 6
  Obs. A number gratter than 6 takes no effect, will assime 6 
*/
#define BUTTON_COUNT 6

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

/*
  Store touch buttons pins
  Using 6 pins to handle 6 touch buttons
  Default value is {1, 4, 10, 12, 13, 14}
*/
int _buttonPin[6] = {1, 4, 10, 12, 13, 14};

/*
  Stores output pin config
*/
int _outputPin[5] = {0, 3, 5, 15, 16};

/*
  Configure if output pin is dimmable or not
*/
bool _buttonPinDimmable[5] = {true, true, true, true, false};


/*
  Enable/disable serial debug
*/
#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 1
#endif

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

bool setSsid(String ssid, bool setCrc = true);

String getSsid(void);

bool setWifiPassword(String *pass, bool setCrc = true);

String getWifiPassword(void);

/*
  Reboot system
  @var bool critical specify that reboot root cause is something critical
*/
void reboot(bool critical = false);

/*
  Check flash integrity & format if needed
*/
bool checkFlash(void);

/**
 * \brief Callculate fash contet CRC32
 * \param partition (1/2) 
 * \return crc32
*/
uint32_t calcFlashCrc(unsigned char partition);


void uInt2Char(uint8_t (&buf)[4], uint32_t value);


uint32_t char2UInt(const uint8_t (&buf)[4]);


bool setFlashCrc(uint8_t partition);