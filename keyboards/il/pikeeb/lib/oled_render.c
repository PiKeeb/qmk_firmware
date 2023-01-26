#include "oled_render.h"
#include QMK_KEYBOARD_H
#include <stdio.h>
#include "measure.h"
#include "bongocat.h"

//---------------------//
// Unused OLED symbols //
//--------------------//

/*
static const char PROGMEM offlogo[] = {
    0xD9, 0xDA, 0x00
};

static const char PROGMEM onlogo[] = {
    0xDB, 0xDC, 0x00
};


static const char PROGMEM almost_equal_to[] = {
    0xDB, 0xDC, 0x00
};
*/

//------------------------//
// OLED Render functions //
//------------------------//

void render_logo(void) {
    static const char PROGMEM pikeeb_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };
    oled_set_cursor(0, 0);
    oled_write_P(pikeeb_logo, false);
}

void render_layer_state(void) {
    static const char PROGMEM layerlogo[] = {
        0xD5, 0xD6, 0x00
    };
    oled_write_P(layerlogo, false);
    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("DFT"), false);
            break;
        case 1:
            oled_write_P(PSTR("FN1"), false);
            break;
        case 2:
            oled_write_P(PSTR("FN2"), false);
            break;
        case 3:
            oled_write_P(PSTR("SET"), false);
            break;
        default:
            oled_write_P(PSTR("UND"), false);
    };
    oled_set_cursor(7, 0);
}

void render_lock_state(led_t led_state) {
    static const char PROGMEM locklogo[] = {
        0xD7, 0xD8, 0x00
    };
    oled_write_P(locklogo, false);
    oled_write_P(led_state.num_lock ? PSTR("N") : PSTR("-"), false);
    oled_write_P(led_state.caps_lock ? PSTR("C") : PSTR("-"), false);
    oled_set_cursor(13, 0);
}

void render_batery_state(void) {
    static const char PROGMEM bat_charge_logo[] = {
        0x9B, 0x9C, 0x00
    };
    static const char PROGMEM bat_100_logo[] = {
        0x95, 0x96, 0x00
    };
    static const char PROGMEM bat_75_logo[] = {
        0x95, 0x98, 0x00
    };
    static const char PROGMEM bat_50_logo[] = {
        0x95, 0x9A, 0x00
    };

    static const char PROGMEM bat_25_logo[] = {
        0x97, 0x9A, 0x00
    };
    static const char PROGMEM bat_0_logo[] = {
        0x99, 0x9A, 0x00
    };
    if (!VBAT_CHARGING_STATE()) {
        if ( VBAT >= 4000 ) {
            oled_write_P(bat_100_logo, false);
        } else if ( VBAT > 3800) {
            oled_write_P(bat_75_logo, false);
        } else if ( VBAT > 3650 ) {
            oled_write_P(bat_50_logo, false);
        } else if ( VBAT > 3450 ) {
            oled_write_P(bat_25_logo, false);
        } else if ( VBAT > 3350 ) {
            oled_write_P(bat_0_logo, false);
        } else { 
            oled_write_P(PSTR("FU"), false); 
        }
    } else {
        oled_write_P(bat_charge_logo, false);
    } 
    oled_set_cursor(0, 2);
}

static const char PROGMEM pilogo[] = {
    0xBB, 0xBC, 0x00
};
static const char PROGMEM pclogo[] = {
    0xB9, 0xBA, 0x00
};

void render_rpi_state(void) {
    oled_write_P(pilogo, false);
    if (!RPI_OFF_STATE()) {
        if ( VRPI >= 5300 ) {
            oled_write_P(PSTR(":OV!"), false);
        } else if ( VRPI > 4800 ) {
            oled_write_P(PSTR(":ON "), false);
        } else {
            oled_write_P(PSTR(":UV!"), false);
        }
    } else {
        oled_write_P(PSTR(":OFF"), false);
    }
    oled_set_cursor(7, 2);
}

void render_usbsw_state(void) {
    oled_write_P(PSTR("USB:"), false);
    oled_write_P(kb_config.usbswitch_high ? pilogo : pclogo, false);
    oled_set_cursor(16, 2);
}

void render_status_page(void) {
    render_layer_state();
    render_lock_state(host_keyboard_led_state());
    render_batery_state();
    render_rpi_state();
    render_usbsw_state();
}

void render_volt_page(void) {
    static const char PROGMEM approx_equal_to[] = {
        0x9D, 0x9E, 0x00
    };
    oled_write_P(PSTR("VBAT"), false);
    oled_write_P(approx_equal_to, false);
    if (!VBAT_CHARGING_STATE()) {
        oled_write_ln(VBAT_String, false);
        oled_write_ln_P(PSTR(" mV"), false);
    } else {
        oled_write(VBAT_String, false);
        oled_write_ln_P(PSTR(" mV (CHRG)"), false);
    };
    oled_set_cursor(0, 1);
    oled_write_P(PSTR("VRPI"), false);
    oled_write_P(approx_equal_to, false);
    if (!RPI_OFF_STATE()) {
        oled_write_ln(VRPI_String, false);
        oled_write_ln_P(PSTR(" mV"), false);
    } else {
        oled_write_ln(VRPI_String, false);
        oled_write_ln_P(PSTR(" mV (OFF)"), false);
    }
}

void render_wpm_page(void) {
    render_anim();
    oled_set_cursor(0, 0);
    sprintf(WPM_String, "WPM:%03d", get_current_wpm());
    oled_write(WPM_String, false);
}

void render_fun_page(void) {
    oled_write_P(PSTR("FUN!"), false);
}

void check_inactive_timer(void) {
    if (timer_elapsed32(INACTIVE_TIMER) > OLED_INACTIVE_TIME_OFF) {
        OLED_TOG = true;
    } else {
        OLED_TOG = false;
    };
    if (timer_elapsed32(INACTIVE_TIMER) > OLED_INACTIVE_TIME_LOGO) {
        LOGO_TOG = true;
    } else {
        LOGO_TOG = false;
    }
}