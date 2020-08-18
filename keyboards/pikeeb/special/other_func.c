/* Copyright 2020 ILWrites
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

void matrix_scan_user(void) {
    if (timer_elapsed32(VBAT_TIMER) > 60000) { // initiate the RPi voltage measurement every 60 seconds
        vbat_measure();
        VBAT_TIMER = timer_read32();
    }
    if (timer_elapsed32(RPIV_TIMER) > 3000) { // initiate the RPi voltage measurement every 3 seconds
        rpiv_measure();
        RPIV_TIMER = timer_read32();
    }
}

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
      INACTIVE_TIMER = timer_read32();
    }
    return true;
};

bool led_update_kb(led_t led_state) {
    bool res = led_update_user(led_state);
    if(res) {
        writePin(NUM_LED_PIN, led_state.num_lock);
        writePin(CAPS_LED_PIN, led_state.caps_lock);
    }
    return res;
}
