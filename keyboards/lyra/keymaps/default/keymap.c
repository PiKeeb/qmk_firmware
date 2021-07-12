/* Copyright 2020 KgOfHedgehogs & ILWrites
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
    _FN1,
    _FN2,
    _FN3,
    _FN4
};

      /*
      PLEASE READ THIS BEFORE MODIFYING THE LAYOUT!

      Lyra keyboard has got two wariants of the PCB: ALPHA and BETA.
      Alpha's got keypad on the right.
      Beta's got keypad on the left.

      For your convinience, we've devided the keyboard zone and the keypad zone visually in the code.
      */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_1UBKSPC_SPLSPC_1UNUM(
    //-----------------------------------------------Keyboard Zone----------------------------------------------------------------//          //------Keypad Zone---------//
        KC_GESC, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,           KC_P7,   KC_P8,   KC_P9,
          KC_TAB,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,    KC_ENT,     KC_PGUP,           KC_P4,   KC_P5,   KC_P6,
             KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,   KC_UP,   KC_PGDN,           KC_P1,   KC_P2,   KC_P3,
        KC_LCTL,  KC_LGUI,  KC_LALT,         KC_BSPC,        MO(_FN1),         KC_SPC,          KC_RALT, KC_LEFT,   KC_DOWN, KC_RGHT,           KC_P0,   KC_P0,   KC_PDOT
    ),
    [_FN1] = LAYOUT_1UBKSPC_SPLSPC_1UNUM(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_PLUS,           S(KC_P7),S(KC_P8),S(KC_P9),
          KC_CLCK,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,    KC_INS,               S(KC_P4),S(KC_P5),S(KC_P6),
             _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,            S(KC_P1),S(KC_P2),S(KC_P3),
        _______,  _______,  _______,         KC_DEL,         _______,          MO(_FN2),        RESET,   KC_PSCR, KC_SLCK, KC_PAUS,             KC_NLCK, S(KC_P0),S(KC_PDOT)
   ),
    [_FN2] = LAYOUT_1UBKSPC_SPLSPC_1UNUM(
        KC_GRV,  TO(_FN3),    TO(_FN4),    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_PLUS,   S(KC_P7),S(KC_P8),S(KC_P9),
          KC_CLCK,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,    KC_INS,               S(KC_P4),S(KC_P5),S(KC_P6),
             _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,            S(KC_P1),S(KC_P2),S(KC_P3),
        _______,  _______,  _______,         KC_DEL,         _______,          _______,         RESET,     KC_PSCR, KC_SLCK, KC_PAUS,           KC_NLCK, S(KC_P0),S(KC_PDOT)
   ),
   [_FN3] = LAYOUT_1UBKSPC_SPLSPC_1UNUM(
       TO(_BASE),  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_PLUS,         S(KC_P7),S(KC_P8),S(KC_P9),
         KC_CLCK,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,    KC_INS,                S(KC_P4),S(KC_P5),S(KC_P6),
            _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,             S(KC_P1),S(KC_P2),S(KC_P3),
       _______,  _______,  _______,         KC_DEL,         _______,          _______,         RESET,     KC_PSCR, KC_SLCK, KC_PAUS,            KC_NLCK, S(KC_P0),S(KC_PDOT)
  ),
  [_FN4] = LAYOUT_1UBKSPC_SPLSPC_1UNUM(
      TO(_BASE),  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_PLUS,          S(KC_P7),S(KC_P8),S(KC_P9),
        KC_CLCK,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,    KC_INS,                 S(KC_P4),S(KC_P5),S(KC_P6),
           _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,              S(KC_P1),S(KC_P2),S(KC_P3),
      _______,  _______,  _______,         KC_DEL,         _______,          _______,         RESET,     KC_PSCR, KC_SLCK, KC_PAUS,             KC_NLCK, S(KC_P0),S(KC_PDOT)
 ),
};

// Setting the indicator layers

// To change the color, change the HSV_XXX value to whatever color you want
// Link to the commonly used colors in QMK: https://beta.docs.qmk.fm/using-qmk/hardware-features/lighting/feature_rgblight#colors

// Color of the RGB LED for Caps Lock
const rgblight_segment_t PROGMEM capslock_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, HSV_RED}
);
// Color of the RGB LED for Num Lock
const rgblight_segment_t PROGMEM numlock_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, HSV_BLUE}
);
// Color of the RGB LED for FN1
const rgblight_segment_t PROGMEM fn1_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, HSV_CYAN}
);
// Color of the RGB LED for FN2
const rgblight_segment_t PROGMEM fn2_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, HSV_PURPLE}
);
// Color of the RGB LED for FN3
const rgblight_segment_t PROGMEM fn3_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, HSV_SPRINGGREEN}
);
// Color of the RGB LED for FN4
const rgblight_segment_t PROGMEM fn4_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, HSV_GOLD} // Change the HSV_GOLD value to whatever HSV color value you want
);

// Setting up the lighting array

const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    capslock_rgb_layer,
    numlock_rgb_layer,
    fn1_rgb_layer,
    fn2_rgb_layer,
    fn3_rgb_layer,
    fn4_rgb_layer
);

// Define names for use in the programming section
enum rgb_layers {
    RGB_CAPS,
    RGB_NUM,
    RGB_FN1,
    RGB_FN2,
    RGB_FN3,
    RGB_FN4
};


void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = rgb_layers;
};


// Program the RGB layer triggers based on keyboard layer state

layer_state_t layer_state_set_user(layer_state_t state) {
    static layer_state_t old_state;

    // Blinks RGB_FN1 layer for 500 ms when layer_state_cmp changes to state 1 (aka layer _FN1)
    if (layer_state_cmp(state, 1) && !layer_state_cmp(old_state, 1)) {
        rgblight_blink_layer(RGB_FN1, 500);
    };

    // Blinks RGB_FN2 layer for 500 ms when layer_state_cmp changes to state 2 (aka layer _FN2)
    if (layer_state_cmp(state, 2) && !layer_state_cmp(old_state, 2)) {
        rgblight_blink_layer(RGB_FN2, 500);
    };

    // Lights up RGB_FN3 when layer_state_cmp changes to state 3 (aka layer _FN3)
    rgblight_set_layer_state(RGB_FN3, layer_state_cmp(state, 3));

    // Lights up RGB_FN3 when layer_state_cmp changes to state 4 (aka layer _FN4)
    rgblight_set_layer_state(RGB_FN4, layer_state_cmp(state, 4));

    old_state = state;
    return state;
};

// Program the RGB layer triggers based on keyboard led_state.

bool led_update_user(led_t led_state) {

    // Lights up RGB_CAPS when led_state.caps_lock is ON
    rgblight_set_layer_state(RGB_CAPS, led_state.caps_lock);

    // Lights up RGB_CAPS when led_state.num_lock is OFF
    rgblight_set_layer_state(RGB_NUM, !led_state.num_lock);

    return true;
}
