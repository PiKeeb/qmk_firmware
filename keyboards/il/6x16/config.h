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
#define VENDOR_ID    0x494c // IL for ILWrites
#define PRODUCT_ID   0x0616 // 0616 for 6x16
#define DEVICE_VER   0x0001 // First version
#define MANUFACTURER ILWrites
#define PRODUCT      6x16

/* Key Matrix Size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 16

/* Keyboard Matrix Assignments */
#define MATRIX_ROW_PINS { D1, D0, D2, D3, D5, D4 }
#define MATRIX_COL_PINS { E6, F1, B2, B1, B3, F4, F5, F6, F7, C7, C6, B6, B5, B4, D7, D6 }
#define UNUSED_PINS { B0, B7, F0 }

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION COL2ROW