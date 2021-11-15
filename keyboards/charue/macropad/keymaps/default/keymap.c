// Copyright 2021 ILWrites (@PiKeeb)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT_all(
        KC_NLCK, KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_P4,   KC_P5,   KC_P6,   KC_F1,
        KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        RGB_MOD, KC_P0,  KC_PDOT, MO(_FN)
    ),
    [_FN] = LAYOUT_all(
        RESET, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};

void keyboard_post_init_user(void) {
  debug_enable=true;
  debug_matrix=true;
  rgblight_mode(RGBLIGHT_MODE_RGB_TEST);
}
