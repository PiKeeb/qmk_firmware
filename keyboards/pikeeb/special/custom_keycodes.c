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

 // Define the keycodes used by macros in process_record_user
 enum custom_keycodes {
     USB_SWITCH,
     BATTERYMODE,
     TS_SWITCH
 };

 /* Custom keycode action */

 bool process_record_user(uint16_t keycode, keyrecord_t *record) {
     switch (keycode) {

     case USB_SWITCH:
         if (record->event.pressed) {
             user_config.usbswitch_on = !user_config.usbswitch_on;
             eeconfig_update_user(user_config.raw);
             if (user_config.usbswitch_on) { // ON = PC MODE
                 writePinHigh(USBSW_PIN);
                 writePinHigh(USBSW_LED_PIN);
             } else {
                 writePinLow(USBSW_PIN);
                 writePinLow(USBSW_LED_PIN);
             }
         }
         break;

     case BATTERYMODE:
         if (record->event.pressed) {
             user_config.battery_fast_charge_on = !user_config.battery_fast_charge_on;
             eeconfig_update_user(user_config.raw);
             if (user_config.battery_fast_charge_on) { // ON = FAST CHARGING (1A MAX)
                 writePinHigh(BAT_SEL_PIN);
                 writePinHigh(BAT_PROG_PIN);
                 writePinHigh(FCH_LED_PIN);
             } else {
                 writePinLow(BAT_SEL_PIN);
                 writePinLow(BAT_PROG_PIN);
                 writePinLow(FCH_LED_PIN);
             }
         }
         break;

     case TS_SWITCH:
         if (record->event.pressed) {
             user_config.touchscreen_on = !user_config.touchscreen_on;
             eeconfig_update_user(user_config.raw);
             if (user_config.touchscreen_on) { // ON = Touch Screen is ON
                 writePinHigh(TS_EN_PIN);
             } else {
                 writePinLow(TS_EN_PIN);
             }
         }
         break;
     }
     return true;
 };
