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

#include "oled_func.h"

void oled_task_user(void) {
    // Show logo when the inactivity timer is more than 60 seconds (60000 ms) long
    if (timer_elapsed32(INACTIVE_TIMER) > 60000) {
        render_logo();
    // Toggle the bit when the inactivity timer is more than 120 seconds (120000 ms) long
    } else if (timer_elapsed32(INACTIVE_TIMER) > 120000) {
        OLED_TOG = true;
    } else {
        OLED_TOG = false;
    };
    // When the bit is on (true), turn off the display
    if (OLED_TOG) {
        oled_off();
    } else {
        oled_on();
    };
    render_status();
}
