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

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _LVL0,
    _LVL1,
    _LVL2,
    _LVL3
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
       KC_1
    ),
    [_LVL1] = LAYOUT(
       KC_2
    ),
    [_LVL2] = LAYOUT(
       KC_3
    ),
    [_LVL3] = LAYOUT(
       KC_4
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

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0:
            if(active) {
               writePin(LED0_PIN, 1);
               rgblight_mode(6);
               layer_on(_LVL0);
            } else {
               writePin(LED0_PIN, 0);
               rgblight_mode(0);
               layer_off(_LVL0);
            }
            break;
        case 1:
            if(active) {
               writePin(LED1_PIN, 1);
               rgblight_mode(12);
               layer_on(_LVL1);
            } else {
               writePin(LED1_PIN, 0);
               rgblight_mode(0);
               layer_off(_LVL1);
            }
            break;
        case 2:
            if(active) {
               writePin(LED2_PIN, 1);
               rgblight_mode(15);
               layer_on(_LVL2);
            } else {
               writePin(LED2_PIN, 0);
               rgblight_mode(0);
               layer_off(_LVL2);
            }
            break;
        case 3:
            if(active) {
               writePin(LED3_PIN, 1);
               rgblight_mode(17);
               layer_on(_LVL3);
            } else {
               writePin(LED3_PIN, 0);
               rgblight_mode(0);
               layer_off(_LVL3);
            }
            break;
    }
    return true;
}
