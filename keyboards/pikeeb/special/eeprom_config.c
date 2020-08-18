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

 // Default EEPROM config in case of the EEPROM reset
 void eeconfig_init_user(void) {
     user_config.raw = 0;
     user_config.usbswitch_on = true; // We want this switched on (PC mode)
     user_config.battery_fast_charge_on = false; // We want this switched off (Slow charging mode)
     user_config.touchscreen_on = false; // We want this switched off (Touch Screen is on)
     eeconfig_update_user(user_config.raw); // Write the default config to EEPROM
     keyboard_post_init_user(); // Call the keymap level init
 };

 // Enable EEPROM and initialize the timers
 void keyboard_post_init_user(void) { // Call the keymap level matrix init.
     user_config.raw = eeconfig_read_user(); // Read the user config from EEPROM
     // Switch pins according to the settings of the EEPROM config
     if (user_config.usbswitch_on) {
         writePinHigh(USBSW_PIN);
         writePinHigh(USBSW_LED_PIN);
     } else {
         writePinLow(USBSW_PIN);
         writePinLow(USBSW_LED_PIN);
     }
     if (user_config.battery_fast_charge_on) {
         writePinHigh(BAT_SEL_PIN);
         writePinHigh(BAT_PROG_PIN);
         writePinHigh(FCH_LED_PIN);
     } else {
         writePinLow(BAT_SEL_PIN);
         writePinLow(BAT_PROG_PIN);
         writePinLow(FCH_LED_PIN);
     }
     if (user_config.touchscreen_on) {
         writePinHigh(TS_EN_PIN);
     } else {
         writePinLow(TS_EN_PIN);
     };

     // Initialize timers for blink & voltage measurements
     BLINK_TIMER = timer_read();
     VBAT_TIMER = timer_read32();
     RPIV_TIMER = timer_read32();

     // Take the voltage measurement
     vbat_measure();
     rpiv_measure();
 };
