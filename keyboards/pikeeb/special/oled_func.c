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

// Define timers
static uint32_t INACTIVE_TIMER = 0; // Inactivity timer

// Define toggle conditions
static bool BLINK_TOG = false;
static bool OLED_TOG = false;

/********************/
/* OLED ACTION CODE */
/********************/

// PiKeeb Logo
void render_logo(void) {
    static const char PROGMEM pikeeb_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };
    oled_write_P(pikeeb_logo, false);
};

// Layer state monitor
void render_layer_state(void) {
    static const char PROGMEM layerlogo[] = {
        0xD5, 0xD6, 0x00
    };
    oled_write_P(layerlogo, false);
    oled_write_P(PSTR("Base"), layer_state_is(0));
    oled_write_P(PSTR("Func"), layer_state_is(1));
    oled_write_P(PSTR("Sett"), layer_state_is(2));
}

// Caps/Num Lock state monitor
void render_lock_state(led_t led_state) {
    static const char PROGMEM locklogo[] = {
        0xD7, 0xD8, 0x00
    };
    oled_write_P(locklogo, false);
    oled_write_P(locklogo, false);
    oled_write_P(led_state.num_lock ? PSTR("N") : PSTR("~"), false);
    oled_write_P(led_state.caps_lock ? PSTR("C") : PSTR("~"), false);
}

void render_touchscreen_state(void) {
    static const char PROGMEM TSlogo[] = {
        0xD9, 0xDA, 0x00
    };
    static const char PROGMEM offlogo[] = {
        0xDB, 0xDC, 0x00
    };
    static const char PROGMEM onlogo[] = {
        0xDD, 0xDE, 0x00
    };
    oled_write_P(TSlogo, false);
    if (user_config.touchscreen_on) {
        oled_write_P(onlogo, false);
    } else {
        oled_write_P(offlogo, false);
    };
}

void render_rpi_state(void) {
    static const char PROGMEM RPIlogo[] = {
        0x9F, 0xBF, 0x00
    };
    oled_write_P(RPIlogo, false);
    if (RPIV>= 5.3) {
        oled_write_P(PSTR("OV"), false);
    } else if (RPIV< 5.3 && RPIV>= 4.8) {
        oled_write_P(PSTR("ON"), false);
    } else if (RPIV< 4.8 && RPIV>= 3.3){
        oled_write_P(PSTR("UV"), false);
    } else {
        oled_write_P(PSTR("~~"), false);
    };
}

void render_batery_state(void) {
    static const char PROGMEM bat_100_logo[] = {
        0x01, 0x02, 0x00
    };
    static const char PROGMEM bat_75_logo[] = {
        0x01, 0x04, 0x00
    };
    static const char PROGMEM bat_50_logo[] = {
        0x01, 0x06, 0x00
    };
    static const char PROGMEM bat_25_logo[] = {
        0x03, 0x06, 0x00
    };
    static const char PROGMEM bat_0_logo[] = {
        0x05, 0x06, 0x00
    };
    static const char PROGMEM blink[] = {
        0x00, 0x00, 0x00
    };
    if (VBAT >= 3.9) {
        oled_write_P(bat_100_logo, false);
    } else if (VBAT < 3.9 && VBAT > 3.6) {
        oled_write_P(bat_75_logo, false);
    } else if (VBAT < 3.6 && VBAT > 3.5 ) {
        oled_write_P(bat_50_logo, false);
    } else if (VBAT < 3.5 && VBAT > 3.2 ) {
        oled_write_P(bat_25_logo, false);
    } else if (VBAT <= 3.2) {
        if (timer_elapsed(BLINK_TIMER) > 1500) { // 1.5s timer to flip the toggle
            BLINK_TIMER = timer_read();
            BLINK_TOG = !BLINK_TOG; // simple flip
        };
        if (BLINK_TOG) {
            oled_write_P(blink, false);
        } else {
            oled_write_P(bat_0_logo, false);
        };
    };
}

void render_status(void) {
    render_layer_state();
    render_lock_state(host_keyboard_led_state());
    render_touchscreen_state();
    render_rpi_state();
    render_batery_state();
}

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
