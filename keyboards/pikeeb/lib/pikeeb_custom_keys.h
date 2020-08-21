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

#pragma once

enum custom_keys {
  USB_SW,
  BATMODE,
  TS_SW
};

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {

        case USB_SW:
            if (record->event.pressed) {
                switch_usb();
            }
            break;

        case BATMODE:
            if (record->event.pressed) {
                switch_batmode();
            }
            break;

        case TS_SW:
            if (record->event.pressed) {
                switch_ts();
            }
            break;
        }
        return true;
};
