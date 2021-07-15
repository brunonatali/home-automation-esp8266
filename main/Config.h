
#ifndef Config_h
#define Config_h

/*
  Enable/disable serial debug
*/
#define SERIAL_DEBUG 0

// Stores configured mode for each button
int _buttonMode[6] = {0xFA}; // Not configured

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

#endif