/**
 * EEPROM (Flash) Default values.
 * Copyright (c) 2022 Bruno Natali - b010010010n@gmail.com
 *
 * License (MIT license):
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef DefaultFlashValues_h
#define DefaultFlashValues_h

#include "NibleStep.h"
#include "FlashValues.h"

/**
 * This file defines standard values for each flash position
 */

/**
 * @brief Define Flash mode (AP / Client)
 *
 * @note Possible values are 0 / 1
 */
#define FLASH_DEFAULT_WIFI_MODE wifi_mode::ACCESS_POINT

/**
 * @brief Define Button untouched logic level state
 *
 * @note Possible values are 0 / 1 [LOW / HIGH]
 */
#define FLASH_DEFAULT_BUTTON_LOGIC_LEVEL button_logic_level::_HIGH

/**
 * @brief Time (seconds) to set that button is holded
 *
 */
#define FLASH_DEFAULT_BUTTON_HOLD_TIME_OUT SECONDS_3

/**
 * @brief Time (seconds) to let button in hold mode.
 *
 * @note This time is calculated by interaction inactivity
 *
 */
#define FLASH_DEFAULT_BUTTON_HOLD_PERIOD SECONDS_30

/**
 * @brief Light mode for button 1.
 *
 */
#define FLASH_DEFAULT_BUTTON_1_MODE LIGHT_MODE_STEP_OUTPUT_1

/**
 * @brief Light mode for button 2.
 *
 */
#define FLASH_DEFAULT_BUTTON_2_MODE LIGHT_MODE_STEP_OUTPUT_2

/**
 * @brief Light mode for button 3.
 *
 */
#define FLASH_DEFAULT_BUTTON_3_MODE LIGHT_MODE_STEP_OUTPUT_3

/**
 * @brief Light mode for button 4.
 *
 */
#define FLASH_DEFAULT_BUTTON_4_MODE LIGHT_MODE_STEP_OUTPUT_4

/**
 * @brief Light mode for button 5.
 *
 */
#define FLASH_DEFAULT_BUTTON_5_MODE LIGHT_MODE_STEP_OUTPUT_5

/**
 * @brief Light mode for button 6.
 *
 * @note We have just 5 outputs, because of it, last button is disabled
 * by default
 */
#define FLASH_DEFAULT_BUTTON_6_MODE LIGHT_MODE_STEP_DISABLED

/**
 * @brief Set bright for touch buttons LED
 */
#define FLASH_DEFAULT_BUTTONS_DIMMER PERCENTAGE_100

/**
 * @brief Dimmer percentage for all outputs.
 *
 * @note To avoid switch damage all outputs are non dimmabe as default
 */
#define FLASH_DEFAULT_OUTPUT PERCENTAGE_0

/**
 * @brief Device IP first octet.
 *
 */
#define FLASH_DEFAULT_IP_OCTET_1 192

/**
 * @brief Device IP second octet.
 *
 */
#define FLASH_DEFAULT_IP_OCTET_2 168

/**
 * @brief Device IP third octet.
 *
 */
#define FLASH_DEFAULT_IP_OCTET_3 4

/**
 * @brief Device IP fourth octet.
 *
 */
#define FLASH_DEFAULT_IP_OCTET_4 1

/**
 * @brief Socket port number that device will use to communicate with manager.
 *
 */
#define FLASH_DEFAULT_MANAGER_PORT 8897

/**
 * @brief WiFi SSID will be default configured with device MAC address, so
 * default value for SSID may not a static one.
 *
 */
#define FLASH_DEFAULT_WIFI_PRE_SSID "BN_"

/**
 * @brief WiFi password.
 *
 */
#define FLASH_DEFAULT_WIFI_PASSWORD "semsenha"

/**
 * @brief Name / alias for button 1.
 *
 * @note Alias will make easier to recognise for which light (lamp)
 * this button is associated for.
 */
#define FLASH_DEFAULT_BUTTON_1_ALIAS "Botao 1"

/**
 * @brief Name / alias for button 2.
 *
 * @note Alias will make easier to recognise for which light (lamp)
 * this button is associated for.
 */
#define FLASH_DEFAULT_BUTTON_2_ALIAS "Botao 2"

/**
 * @brief Name / alias for button 3.
 *
 * @note Alias will make easier to recognise for which light (lamp)
 * this button is associated for.
 */
#define FLASH_DEFAULT_BUTTON_3_ALIAS "Botao 3"

/**
 * @brief Name / alias for button 4.
 *
 * @note Alias will make easier to recognise for which light (lamp)
 * this button is associated for.
 */
#define FLASH_DEFAULT_BUTTON_4_ALIAS "Botao 4"

/**
 * @brief Name / alias for button 5.
 *
 * @note Alias will make easier to recognise for which light (lamp)
 * this button is associated for.
 */
#define FLASH_DEFAULT_BUTTON_5_ALIAS "Botao 5"

/**
 * @brief Name / alias for button 6.
 *
 * @note Alias will make easier to recognise for which light (lamp)
 * this button is associated for.
 */
#define FLASH_DEFAULT_BUTTON_6_ALIAS "Botao 6"

/**
 * @brief Name / alias room.
 *
 * @note Alias will make easier to recognise the room that keypad is
 * installed.
 */
#define FLASH_DEFAULT_KEYPAD_ALIAS "Lugar desconhecido"

/**
 * @brief Remote device address that may be controlled by buttons.
 * New or erased devices do not have a configuration that allows them to
 * access a manager, so a default value is defined here for all buttons.
 *
 * @note Buttons can control outputs on other devices, to make this happen,
 * configure address for remote device and `FLASH_DEFAULT_BUTTON_MODE_N`
 * as `LIGHT_MODE_STEP_REMOTE_OUTPUT_N` .
 */
#define FLASH_DEFAULT_REMOTE_ADDRESS 0x00

/**
 * @brief Device address.
 *
 * @note Own address is configured by manager, turning less possible keypads
 * connected to the same network with the same address. So, we define default
 * value here just in case we need that devices starts with diferent
 * address value.
 */
#define FLASH_DEFAULT_DEVICE_ADDRESS 0x00

/**
 * @brief Time holding lock buttons to perform a system lock / unlock.
 *
 */
#define FLASH_DEFAULT_LOCK_TIME_OUT SECONDS_2

/**
 * @brief Time of inactivity to auto lock the system.
 *
 */
#define FLASH_DEFAULT_AUTO_LOCK_PERIOD SECONDS_20

/**
 * @brief This configures system auto lock and buttons to lock / unlock
 * the system.
 *
 * @note This default will be set as 8 bit, because we do not need
 * configurations in a separated define at this moment.
 *
 * Bit 1 - System auto lock enabled
 *
 * Will use the upper 2 buttons to perform system lock / unlock
 * Bit 2 - Button 1
 * Bit 4 - Button 4
 */
#define FLASH_DEFAULT_SYSTEM_LOCK_CONFIG 0x13

#endif