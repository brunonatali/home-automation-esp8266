// Enable/disable serial debug
#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 0
#endif

/**
 * Hardware version
 * v0 - Buttons are mirrored
 * v1 - Buttons corrected
 */
#define HW_VERSION 1

/**
 * Define serial print mock
 */
#if SERIAL_DEBUG
#define SERIALPRINT Serial.print
#define SERIALPRINTLN Serial.println
#else
#define SERIALPRINT
#define SERIALPRINTLN
#endif