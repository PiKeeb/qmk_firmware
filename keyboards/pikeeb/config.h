/*
Copyright 2020 ILWrites

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


#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID    0xFEED
#define PRODUCT_ID   0x0000
#define DEVICE_VER   0x0001
#define MANUFACTURER ILWrites
#define PRODUCT      PiKeeb

/* Key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 15

/* Keyboard Matrix Assignments */
#define MATRIX_ROW_PINS { C1, C2, C3, C4, C5 }
#define MATRIX_COL_PINS { C7, A7, A6, A5, A4, A3, A2, A1, A0, F7, F6, F5, F4, F3, F2 }
#define UNUSED_PINS

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* Setting up the OLED driver */
#ifdef OLED_DRIVER_ENABLE
#   define OLED_FONT_H "keyboards/pikeeb/lib/glcdfont_pikeeb.c"
#   define OLED_FONT_START 0
#   define OLED_FONT_END 223
#   define OLED_FONT_WIDTH 6
#   define OLED_FONT_HEIGHT 8
#endif

/* Encoder pins */
#ifdef ENCODER_ENABLE
#   define ENCODERS_PAD_A { E1, E4 }
#   define ENCODERS_PAD_B { C0, E5 }
#endif

/* Screen backlight settings */
#define BACKLIGHT_PIN B6
#define BACKLIGHT_LEVELS 5

/* Audio pin settings */
#ifdef AUDIO_ENABLE
#   define C6_AUDIO
#endif

/* Bluetooth pins settings */
#ifdef BLUETOOTH_ENABLE
#   define AdafruitBleResetPin D4
#   define AdafruitBleCSPin B4
#   define AdafruitBleIRQPin E6
#endif

/* Other pins settings */

// Switches:
#define USBSW_PIN B0 // Pin for USB switch
#define TS_EN_PIN E3 // Pin for turning touch screen IC on or off

// Charger settings:
#define BAT_SEL_PIN B7 // Pin for choosing the charger type (USB or charger)
#define BAT_PROG_PIN B5 // Pin for programmming the battery charger

// Voltage monitoring:
#define VBAT_MON_EN_PIN E7 // Pin for turning battery monitor on or off
#define VBAT_MON_ADC_PIN F0 // Pin for monitoring the battery voltage
#define RPIV_MON_ADC_PIN F1 // Pin for monitoring the raspberry voltage

// LED pins:
#define USBSW_LED_PIN D2
#define CAPS_LED_PIN D3
#define NUM_LED_PIN D5
#define FCH_LED_PIN D6
#define F0_LED_PIN D7
#define F1_LED_PIN E0

/* Other settings */

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* Force NKRO */
//#define FORCE_NKRO

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

/* disable these deprecated features by default */
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

/* Bootmagic Lite key configuration */
//#define BOOTMAGIC_LITE_ROW 0
//#define BOOTMAGIC_LITE_COLUMN 0
