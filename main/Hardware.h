#ifndef Hardware_h
#define Hardware_h


#include "Config.h"

/**
 * Hardware version
 * v0 - Buttons are mirrored
 * v1 - Buttons corrected
*/
#define HW_VERSION 1

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


// Stores output pin config
#if SERIAL_DEBUG
int _outputPin[5] = {0, 0, 5, 15, 16};
#else
int _outputPin[5] = {0, 3, 5, 15, 16};
#endif


// Configure if output pin is dimmable or not
bool _buttonPinDimmable[5] = {true, true, true, true, false};

#endif