// Copyright 2021 ILWrites (@PiKeeb)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID    0x4344 //CD for Charue Design
#define PRODUCT_ID   0x7470 // tp for Terrapin
#define DEVICE_VER   0x0001 // First version
#define MANUFACTURER Charue Design
#define PRODUCT      terrapin

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 4

/* Keyboard Matrix Assignments */
#define MATRIX_ROW_PINS { D1, D2, D3, D5, D0 }
#define MATRIX_COL_PINS { C7, B5, B6, C6 }
#define UNUSED_PINS { B0, B7, E6, F4, F5, F6, F7, }

/* Encoder pads */
#define ENCODERS_PAD_A { B4 }
#define ENCODERS_PAD_B { D7 }

/* Analog slider potentiometer pins */
#define POT1_PIN D6
#define POT2_PIN D4

/* Button pins */
#define ENC_BUTTON_PIN F1

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION COL2ROW

#define RGB_DI_PIN F0
#ifdef RGB_DI_PIN
#    define RGBLED_NUM 6
#    define RGBLIGHT_ANIMATIONS
#endif

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* disable these deprecated features by default */
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION