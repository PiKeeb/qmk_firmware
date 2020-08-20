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

#include "eeprom_config.h"

// Default EEPROM config in case of the EEPROM reset
void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.usbswitch_on = true; // We want this switched on (PC mode)
    user_config.battery_fast_charge_on = false; // We want this switched off (Slow charging mode)
    user_config.touchscreen_on = false; // We want this switched off (Touch Screen is on)
    eeconfig_update_user(user_config.raw); // Write the default config to EEPROM
    keyboard_post_init_user(); // Call the keymap level init
};
