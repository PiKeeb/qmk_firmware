/*
Copyright 2022 KgOfHedgehogs & ILWrites

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID 0x616c // al for alexbarb
#define PRODUCT_ID 0x6c79 // ly for lyra
#define DEVICE_VER 0x0001 // first version
#define MANUFACTURER alexbarb
#define PRODUCT Lyra
#define DESCRIPTION A Candybar-inspired keyboard with a numpad and a MX Hot-swap PCB

/* key matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 9

/*
 * Keyboard Matrix Assignments
 */
#define MATRIX_ROW_PINS { F7, F6, F5, F4, F1, F0, E6, D7 }
#define MATRIX_COL_PINS { C6, C7, D0, D1, D2, D3, D4, D5, D6 }
#define UNUSED_PINS {B4, B5}

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

/* RGB LED

  ****************
  *** WARNING! ***
  ****************

  In case you've got the PCB from the original GB and not the OSH version,
  please, change the RGB_DI_PIN to B4 and change B4 in the UNUSED_PINS to B6.

 */
#define RGB_DI_PIN B6
#ifdef RGB_DI_PIN
  #define RGBLED_NUM 1
  #define RGBLIGHT_HUE_STEP 8
  #define RGBLIGHT_SAT_STEP 8
  #define RGBLIGHT_VAL_STEP 8
  #define RGBLIGHT_LIMIT_VAL 100
  #define RGBLIGHT_SLEEP  /* If defined, the RGB lighting will be switched off when the host goes to sleep */
  // #define RGBLIGHT_ANIMATIONS
  #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
  #define RGBLIGHT_EFFECT_RAINBOW_MOOD
  #define RGBLIGHT_EFFECT_BREATHING
  #define RGBLIGHT_LAYERS
  #define RGBLIGHT_LAYER_BLINK
#endif

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Depricated features */
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
