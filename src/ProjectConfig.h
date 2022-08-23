/**
 * @brief Enable/disable serial debug
 */
#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG 1
#endif

/**
 * @brief Hardware version
 * v0 - Buttons are mirrored
 * v1 - Buttons corrected
 */
#define HW_VERSION 1

/**
 * @brief Use this to ERASE FLASH at bootup
 *
 * @note You need to change this value (0/1) every time you want
 * to erase flash after record a new firmware. After firts boot
 * and memmory erase, this value will be recorded into flash
 */
#define MANUAL_SET_BIT_TO_ERASE_FLASH 0

/**
 * @brief Use this to set defsult values on flash
 *
 * @note You need to change this value (0/1) every time you want
 * to restore flash after record a new firmware. After firts boot
 * and memmory restore, this value will be recorded into flash
 */
#define MANUAL_SET_BIT_TO_RESTORE_FLASH_DEFAULTS 0

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
 * @brief Gran total of bottons we may have
 */
#define TOTAL_BUTTONS_COUNT 6

/**
 * @brief Set the amount of buttons you must use
 * Default value is 6
 *
 * @note A number gratter than 6 takes no effect, will assime 6
 */
#if SERIAL_DEBUG
#define AVAILABLE_BUTTONS_COUNT 5
#else
#define AVAILABLE_BUTTONS_COUNT 6
#endif

/**
 * @brief Gran total of outputs we may have
 */
#define TOTAL_OUTPUTS_COUNT 5