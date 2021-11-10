/* Copyright 2021 ILWrites
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "print.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _LVL0,
    _LVL1,
    _LVL2,
    _LVL3,
    _LVL4,
    _LVL5,
    _LVL6,
    _LVL7,
    _LVL8,
    _LVL9,
    _LVL10,
    _LVL11,
    _LVL12,
    _LVL13,
    _LVL14,
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    DHYBS = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
       DHYBS
    ),
    [_LVL0] = LAYOUT(
       KC_0
    ),
    [_LVL1] = LAYOUT(
       KC_1
    ),
    [_LVL2] = LAYOUT(
       KC_2
    ),
    [_LVL3] = LAYOUT(
       KC_3
    ),
    [_LVL4] = LAYOUT(
       KC_4
    ),
    [_LVL5] = LAYOUT(
       KC_5
    ),
    [_LVL6] = LAYOUT(
       KC_6
    ),
    [_LVL7] = LAYOUT(
       KC_7
    ),
    [_LVL8] = LAYOUT(
       KC_8
    ),
    [_LVL9] = LAYOUT(
       KC_9
    ),
    [_LVL10] = LAYOUT(
       KC_Q
    ),
    [_LVL11] = LAYOUT(
       KC_W
    ),
    [_LVL12] = LAYOUT(
       KC_E
    ),
    [_LVL13] = LAYOUT(
       KC_R
    ),
    [_LVL14] = LAYOUT(
       KC_T
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DHYBS:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("DHYBS is the best thing ever!");
            } else {
                // when keycode QMKBEST is released
            }
            break;
    }
    return true;
}

int DIP_VAL = 0;

void keyboard_post_init_user(void) {
    debug_enable=true;
}

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0:
            if(active) {
               writePin(LED0_PIN, 1);
               DIP_VAL |= 1 << 0;
               dprintf("DIP value: %d\n", DIP_VAL);
            } else {
               writePin(LED0_PIN, 0);
               DIP_VAL &= ~(1 << 0);
               dprintf("DIP value: %d\n", DIP_VAL);
            }
            break;
        case 1:
            if(active) {
               writePin(LED1_PIN, 1);
               DIP_VAL |= 1 << 1;
               dprintf("DIP value: %d\n", DIP_VAL);
            } else {
               writePin(LED1_PIN, 0);
               DIP_VAL &= ~(1 << 1);
               dprintf("DIP value: %d\n", DIP_VAL);
            }
            break;
        case 2:
            if(active) {
               writePin(LED2_PIN, 1);
               DIP_VAL |= 1 << 2;
               dprintf("DIP value: %d\n", DIP_VAL);
            } else {
               writePin(LED2_PIN, 0);
               DIP_VAL &= ~(1 << 2);
               dprintf("DIP value: %d\n", DIP_VAL);
            }
            break;
        case 3:
            if(active) {
               writePin(LED3_PIN, 1);
               DIP_VAL |= 1 << 3;
               dprintf("DIP value: %d\n", DIP_VAL);
            } else {
               writePin(LED3_PIN, 0);
               DIP_VAL &= ~(1 << 3);
               dprintf("DIP value: %d\n", DIP_VAL);
            }
            break;
    }
    layer_clear();
    switch (DIP_VAL) {
        case 0:
            layer_on(_BASE);
            rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
            break;
        case 1:
            layer_on(_LVL0);
            rgblight_mode(RGBLIGHT_MODE_BREATHING);
            break;
        case 2:
            layer_on(_LVL1);
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_MOOD);
            break;
        case 3:
            layer_on(_LVL2);
            rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL);
            break;
        case 4:
            layer_on(_LVL3);
            rgblight_mode(RGBLIGHT_MODE_SNAKE);
            break;
        case 5:
            layer_on(_LVL4);
            rgblight_mode(RGBLIGHT_MODE_KNIGHT);
            break;
        case 6:
            layer_on(_LVL5);
            rgblight_mode(RGBLIGHT_MODE_CHRISTMAS);
            break;
        case 7:
            layer_on(_LVL6);
            rgblight_mode(RGBLIGHT_MODE_STATIC_GRADIENT);
            break;
        case 8:
            layer_on(_LVL7);
            rgblight_mode(RGBLIGHT_MODE_RGB_TEST);
            break;
        case 9:
            layer_on(_LVL8);
            rgblight_mode(RGBLIGHT_MODE_ALTERNATING);
            break;
        case 10:
            layer_on(_LVL9);
            break;
        case 11:
            layer_on(_LVL10);
            break;
        case 12:
            layer_on(_LVL11);
            break;
        case 13:
            layer_on(_LVL12);
            break;
        case 14:
            layer_on(_LVL13);
            break;
        case 15:
            layer_on(_LVL14);
            break;
    }
    return true;
}
