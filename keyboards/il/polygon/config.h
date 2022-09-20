// Copyright 2022 ILWrites (@PiKeeb)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Copyright 2022 ILWrites (@PiKeeb)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "config_common.h"

/* Key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 16

/* Keyboard Matrix Assignment s*/
#define MATRIX_ROW_PINS { B0, B1, B2, B3, D1 }
#define MATRIX_COL_PINS { D4, D6, D7, B4, B5, B6, C6, C7, F7, F6, F5, F4, F1, F0, E6, D2 }

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* Backlight */
#define BACKLIGHT_PIN B7

/* CAPS LED */
#define LED_CAPS_LOCK_PIN D0

/* Encoder pins */
#define ENCODERS_PAD_A { D3 }
#define ENCODERS_PAD_B { D5 }

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE
