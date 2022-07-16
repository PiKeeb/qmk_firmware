// Copyright 2022 Charue Design
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
// 0x4344 = CD for Charue Design
#define VENDOR_ID    0x4344
// 0x4231 = B2 for Board 2 (uniPCB board 2)
#define PRODUCT_ID   0x4232
// 0x0001 = 1 for Base version
#define DEVICE_VER   0x0001
#define MANUFACTURER Charue Design
#define PRODUCT      uniPCB 60

/* Key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 15

/* Keyboard Matrix Assignments*/
#define MATRIX_ROW_PINS { F1, F0, B0, C7, D5 }
#define MATRIX_COL_PINS { B6, B5, B4, D7, D6, D4, D1, D0, F7, F6, F5, C6, D2, B3, B2 }
#define UNUSED_PINS { B7, E6, F4 }

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* RGB */
#define RGB_DI_PIN D3
#ifdef RGB_DI_PIN
#    define RGBLED_NUM 10
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_CHRISTMAS
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
#define RGBLIGHT_EFFECT_RGB_TEST
#define RGBLIGHT_EFFECT_ALTERNATING
#define RGBLIGHT_EFFECT_TWINKLE
#endif

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE
