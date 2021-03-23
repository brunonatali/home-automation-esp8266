
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
  Set button default logic level (untouched)
*/
#define BUTTON_DEFAULT_LEVEL HIGH

/*
  Store touch buttons pins
  Using 6 pins to handle 6 touch buttons
  Default value is {4, 5, 12, 13, 14, 16}
*/
int _buttonPin[6] = {4, 5, 12, 13, 14, 16};

/*
  Button hold timeout
  Indicates time in miliseconds that will trigger button is holding
*/
#define BUTTON_HOLD_TIMEOUT 3000 // 3 sec

/*
  Button hold period
  Indicates time in miliseconds that button still in holded state before return to default
*/
#define BUTTON_HOLD_PERIOD 10000 // 10 sec

/*
  Enable/disable serial debug
*/
#define SERIAL_DEBUG true

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
//////////////////////// INCLUDES & DECLARATIONS
////////////////////////

#include <Arduino.h>

#include "TouchButtonModule.h"

#ifndef ESP8266WiFi_h
#define ESP8266WiFi_h
#include "ESP8266WiFi.h"
#endif

#ifndef WifiCredentials_h
#define WifiCredentials_h
#include "WifiCredentials.h"
#endif

/*
  Stores button`s class handler
*/
TouchButtonModule *_touchButton[6];
