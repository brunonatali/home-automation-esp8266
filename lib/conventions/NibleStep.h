/**
 * Nible (4 bit) step definition.
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

#ifndef NibleStep_h
#define NibleStep_h

#include <Arduino.h>

/**
 * @brief Steps that represents seconds.
 * This was designed to be saved under 4 bit variable
 *
 * @note Insted values are ginven in miliseconds, this enum represents an
 * seconds request interpretation.
 *
 */
const uint16_t NIBLE_STEP_SECONDS[16] = {
    500,
    1000,
    2000,
    3000,
    4000,
    5000,
    7000,
    8000,
    10000,
    15000,
    20000,
    25000,
    30000,
    40000,
    50000,
    60000};

/**
 * @brief Seconds keywords to make the code easier to read.
 *
 * @note Use array above with this keywords
 *
 */
#define SECONDS_0_5 0
#define SECONDS_1 1
#define SECONDS_2 2
#define SECONDS_3 3
#define SECONDS_4 4
#define SECONDS_5 5
#define SECONDS_7 6
#define SECONDS_8 7
#define SECONDS_10 8
#define SECONDS_15 9
#define SECONDS_20 10
#define SECONDS_25 11
#define SECONDS_30 12
#define SECONDS_40 13
#define SECONDS_50 14
#define SECONDS_60 15

/**
 * @brief Steps that represents percentage.
 * This was designed to be saved under 4 bit variable
 *
 */
const uint8_t NIBLE_STEP_PERCENTAGE[16] = {
    0,
    6,
    13,
    19,
    26,
    32,
    39,
    45,
    53,
    60,
    66,
    73,
    79,
    86,
    93,
    100};

/**
 * @brief Percentage keywords to make the code easier to read.
 *
 * @note Use array above with this keywords
 *
 */
#define PERCENTAGE_0 0
#define PERCENTAGE_6 1
#define PERCENTAGE_13 2
#define PERCENTAGE_19 3
#define PERCENTAGE_26 4
#define PERCENTAGE_32 5
#define PERCENTAGE_39 6
#define PERCENTAGE_45 7
#define PERCENTAGE_53 8
#define PERCENTAGE_60 9
#define PERCENTAGE_66 10
#define PERCENTAGE_73 11
#define PERCENTAGE_79 12
#define PERCENTAGE_86 13
#define PERCENTAGE_93 14
#define PERCENTAGE_100 15

/**
 * @brief Steps that represents light mode, to be configured on buttons.
 * This was designed to be saved under 4 bit variable
 *
 */
#define LIGHT_MODE_STEP_NOT_CONFIGURED 0
#define LIGHT_MODE_STEP_OUTPUT_1 1
#define LIGHT_MODE_STEP_OUTPUT_2 2
#define LIGHT_MODE_STEP_OUTPUT_3 3
#define LIGHT_MODE_STEP_OUTPUT_4 4
#define LIGHT_MODE_STEP_OUTPUT_5 5
#define LIGHT_MODE_STEP_REMOTE_OUTPUT_1 6
#define LIGHT_MODE_STEP_REMOTE_OUTPUT_2 7
#define LIGHT_MODE_STEP_REMOTE_OUTPUT_3 8
#define LIGHT_MODE_STEP_REMOTE_OUTPUT_4 9
#define LIGHT_MODE_STEP_REMOTE_OUTPUT_5 10
#define LIGHT_MODE_STEP_UNASIGNED_1 11
#define LIGHT_MODE_STEP_UNASIGNED_2 12
#define LIGHT_MODE_STEP_UNASIGNED_3 13
#define LIGHT_MODE_STEP_DISABLED_BY_SOFTWARE 14
#define LIGHT_MODE_STEP_DISABLED 15

#endif