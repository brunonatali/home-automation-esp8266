/**
 * @brief Enable/disable serial debug
 */
#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 1
#endif

/**
 * Hardware version
 * v0 - Buttons are mirrored
 * v1 - Buttons corrected
 */
#define HW_VERSION 1

/**
 * @brief Define serial print mock
 */
#if SERIAL_DEBUG
#define SERIALPRINT Serial.print
#define SERIALPRINTLN Serial.println
#else
#define SERIALPRINT
#define SERIALPRINTLN
#endif

/**
 * @brief Set the amount of buttons you must use
 * Default value is 6
 *
 * @note A number gratter than 6 takes no effect, will assime 6
 */
#if SERIAL_DEBUG
#define BUTTONS_COUNT 5
#else
#define BUTTONS_COUNT 6
#endif

/**
 * This value probably not change, but we set here in order to
 * help loops
 */
#define OUTPUTS_COUNT 5