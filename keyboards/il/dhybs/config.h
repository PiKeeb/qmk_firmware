/*
Copyright 2021 ILWrites

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
#define VENDOR_ID    0x494C // IL for ILWrites
#define PRODUCT_ID   0x4253 // BS for Big Switch
#define DEVICE_VER   0x0001 // First version
#define MANUFACTURER ILWrites
#define PRODUCT      DHYBS

/* key matrix size */
#define MATRIX_ROWS 1
#define MATRIX_COLS 1

/* Keyboard Matrix Assignments */
#define MATRIX_ROW_PINS { F1 }
#define MATRIX_COL_PINS { F0 }
#define UNUSED_PINS { B6, C7, D5, D6, D7 }

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* DIP Switch */
#ifdef DIP_SWITCH_ENABLE
#    define DIP_SWITCH_PINS { D3, D2, D1, D0 }
#endif

/* Bluetooth */
#ifdef BLUETOOTH
#    define AdafruitBleResetPin D4
#    define AdafruitBleCSPin B4
#    define AdafruitBleIRQPin E6
#endif

// VIA layer number
#define DYNAMIC_KEYMAP_LAYER_COUNT 16

/* Audio */
#ifdef AUDIO_ENABLE
#    define AUDIO_PIN B7
#endif

/* RGB */
/* Warning! When changing RGBLIGHT_LIMIT_VAL make sure that the fuse (PTC) is up to spec with the consumed current! */
#define RGB_DI_PIN C6
#ifdef RGB_DI_PIN
#   define RGBLED_NUM 20
#   define RGBLIGHT_ANIMATIONS
#   define RGBLIGHT_LIMIT_VAL 150
#endif

/* LEDs */
#define LED0_PIN F7
#define LED1_PIN F6
#define LED2_PIN F5
#define LED3_PIN F4

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5