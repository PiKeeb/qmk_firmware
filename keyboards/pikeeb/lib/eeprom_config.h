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

 static uint32_t RPIV_TIMER = 0; // Raspberyy Pi voltage measurement timer
 static uint32_t VBAT_TIMER = 0; // Battery voltage measurement timer
 static uint16_t BLINK_TIMER = 0; // Blink timer for the battery indicator

 // Define EEPROM config
 typedef union {
   uint32_t raw;
   struct {
     bool usbswitch_on :1;
     bool battery_fast_charge_on :1;
     bool touchscreen_on :1;
   };
 } user_config_t;

 user_config_t user_config;
