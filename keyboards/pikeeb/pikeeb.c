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

#include "pikeeb.h"
#include "analog.h"
#include <stdio.h>

// Debug include
#include "debug.h"

#ifdef INACT_LOGO
    static uint32_t OLED_LOGO_TIME = INACT_LOGO;
#else
    static uint32_t OLED_LOGO_TIME = 60000;
#endif

#ifdef INACT_OFF
    static uint32_t OLED_OFF_TIME = INACT_OFF;
#else
    static uint32_t OLED_OFF_TIME = 120000;
#endif


#ifdef BAT_MEAS_TIME
    static uint32_t VBAT_TIME = BAT_MEAS_TIME;
#else
    static uint32_t VBAT_TIME = 60000;
#endif

#ifdef RPI_MEAS_TIME
    static uint32_t RPIV_TIME = RPI_MEAS_TIME;
#else
    static uint32_t RPIV_TIME = 3000;
#endif

// Define timers
static uint32_t INACTIVE_TIMER = 0; // Inactivity timer
static uint32_t RPIV_TIMER = 0; // Raspberyy Pi voltage measurement timer
static uint32_t VBAT_TIMER = 0; // Battery voltage measurement timer

// Define state booleans
static bool RPI_OFF_STATE = true;
static bool VBAT_CHARGING_STATE = false;

// Define toggle booleans
static bool OLED_TOG = false;
static bool LOGO_TOG = false;
static bool OLED_TO_CLEAR = false;
static bool OLED_FORCE_OFF = false;

// Define current OLED page
static int OLED_PAGE = 0;

// Define constants:
// Voltage division factor
static float VDF = 9.33;
// Voltage reference in mV
static uint32_t VREF = 5200;
// Analog-to-Digital Converter resolution
static uint32_t ADC_RES = 1024;
// Full battery voltage in mV
static int VBAT_full = 4200;

// Define values for voltage reading
static int VBAT;
static int RPIV;

// Define value for Backlignt level
static int BL_LVL;

// Define number of voltage samples to take each measurement cycle
static int NUM_SAMPLES = 10;

// Strings for sprintf
static char RPIV_String[9];
static char VBAT_String[9];
static char BL_LVL_String[9];

// Global OLED symbols
static const char PROGMEM pilogo[] = {
    0x9B, 0x9C, 0x00
};


//----------------------//
// EEPROM configuration //
//----------------------//

// Define EEPROM config
typedef union {
  uint32_t raw;
  struct {
    bool usbswitch_on :1;
    bool battery_fast_charge_on :1;
    bool touchscreen_on :1;
  };
} kb_config_t;

kb_config_t kb_config;

// Default EEPROM config in case of the EEPROM reset
void eeconfig_init_kb(void) {
    kb_config.raw = 0;
    kb_config.usbswitch_on = false; // We want this switched off (PC mode)
    kb_config.battery_fast_charge_on = false; // We want this switched off (Slow charging mode)
    kb_config.touchscreen_on = false; // We want this switched off (Touch Screen is off)
    eeconfig_update_kb(kb_config.raw); // Write the default KB config to EEPROM
    keyboard_post_init_kb(); // Call the KB level init
}

//-------------------------------//
// Voltage Measurement functions //
//-------------------------------//

void vbat_measure(void) {
    // Define values to get destroyed after the function ends
    int VBAT_RAW = 0; // RAW = no caclualtion
    int VBAT_DIV = 0; // DIV = divided value in mV
    int VBAT_REAL = 0; // REAL = calculated value in mV
    int NUM_VBAT_SCOUNT = 0; // Samples taken counter

    // Take the measurements
    writePinLow(VBAT_MON_EN_PIN); // Disable discharge protection MOSFET
    while (NUM_VBAT_SCOUNT < NUM_SAMPLES) {
        VBAT_RAW += analogReadPin(VBAT_MON_ADC_PIN); // Read the analog voltage and sum it up.
        NUM_VBAT_SCOUNT++; // Advance the counter
    };
    writePinHigh(VBAT_MON_EN_PIN); // Enable discharge protection MOSFET

    // Start the calculation
    VBAT_DIV = VBAT_RAW * VREF / ADC_RES / NUM_SAMPLES;
    VBAT_REAL = (VBAT_DIV * VDF);

    // Output the calculation
    VBAT = VBAT_REAL;

    // Set the state
    if (VBAT > VBAT_full) {
        VBAT_CHARGING_STATE = true;
    } else {
        VBAT_CHARGING_STATE = false;
    }

    // Convert int to string
    sprintf(VBAT_String, "%u mV", VBAT);

    // Output to debug console
    dprintf("VBAT_RAW: %u\n", VBAT_RAW);
    dprintf("VBAT_DIV: %u mV\n", VBAT_DIV);
    dprintf("VBAT: %u mV\n", VBAT);
}

void rpiv_measure(void) {
    // Define values to get destroyed after the function ends
    int RPIV_RAW = 0; // RAW = no caclualtion
    int RPIV_DIV = 0; // DIV = divided value in mV
    int RPIV_REAL = 0; // REAL = calculated value in mV
    int NUM_RPIV_SCOUNT = 0; // Samples taken counter

    // Take the measurements
    while (NUM_RPIV_SCOUNT < NUM_SAMPLES) {
        RPIV_RAW += analogReadPin(RPIV_MON_ADC_PIN);; // Read the analog voltage and sum it up.
        NUM_RPIV_SCOUNT++; // Advance the counter
    };

    // Start the calculation
    RPIV_DIV = RPIV_RAW * VREF / ADC_RES / NUM_SAMPLES;
    RPIV_REAL = (RPIV_DIV * VDF);

    // Check for noise & set the state
    if (RPIV_REAL < 1000) {
        RPIV_REAL = 0;
        RPI_OFF_STATE = true;
    } else {
        RPI_OFF_STATE = false;
    };

    // Output the calculation
    RPIV = RPIV_REAL;

    // Convert int to string
    sprintf(RPIV_String, "%u mV", RPIV);

    // Output to debug console
    dprintf("RPIV_RAW: %u\n", RPIV_RAW);
    dprintf("RPIV_DIV: %u mV\n", RPIV_DIV);
    dprintf("RPIV: %u mV\n", RPIV);
}

//----------------------------//
// Keyboard related functions //
//----------------------------//

void keyboard_post_init_kb(void) {

    // Debug enable
    debug_enable = true;
    debug_matrix = true;

    //Setting the pins to output
    setPinOutput(USBSW_PIN);
    setPinOutput(BAT_SEL_PIN);
    setPinOutput(BAT_SEL_PIN);
    setPinOutput(TS_EN_PIN);

    //Setting the LED pins to output
    setPinOutput(NUM_LED_PIN);
    setPinOutput(CAPS_LED_PIN);
    setPinOutput(F0_LED_PIN);
    setPinOutput(F1_LED_PIN);
    setPinOutput(USBSW_LED_PIN);
    setPinOutput(FCH_LED_PIN);

    //Setting the ADC pins
    setPinOutput(VBAT_MON_EN_PIN);
    setPinInput(VBAT_MON_ADC_PIN);
    setPinInput(RPIV_MON_ADC_PIN);

    // Take the voltage measurement
    vbat_measure();
    rpiv_measure();

    // EEPROM power-on init
    kb_config.raw = eeconfig_read_kb();

    // Switch pins according to the settings of the EEPROM config
    if (kb_config.usbswitch_on) {
        writePinHigh(USBSW_PIN);
    } else {
        writePinLow(USBSW_PIN);
    };
    if (kb_config.battery_fast_charge_on) {
        writePinHigh(BAT_SEL_PIN);
        writePinHigh(BAT_PROG_PIN);
    } else {
        writePinLow(BAT_SEL_PIN);
        writePinLow(BAT_PROG_PIN);
    };
/*  if (kb_config.backlight_on) {
        writePinHigh(BL_EN_PIN);
    } else {
        writePinLow(BL_EN_PIN);
    };*/
    if (kb_config.touchscreen_on) {
        writePinLow(TS_EN_PIN);
    } else {
        writePinHigh(TS_EN_PIN);
    };

    // Initialize timers for voltage measurements
    VBAT_TIMER = timer_read32();
    RPIV_TIMER = timer_read32();

    // Check the inactive time for errors
    if (OLED_OFF_TIME < OLED_LOGO_TIME) {
        OLED_LOGO_TIME = 12000;
        OLED_OFF_TIME = 24000;
    };
}

void matrix_scan_kb(void) {
    if (timer_elapsed32(VBAT_TIMER) > VBAT_TIME) {
        dprint("VBAT_TIMER elapsed!\n");
        vbat_measure();
        VBAT_TIMER = timer_read32();
    };
    if (timer_elapsed32(RPIV_TIMER) > RPIV_TIME) {
        dprint("RPIV_TIMER elapsed!\n");
        rpiv_measure();
        RPIV_TIMER = timer_read32();
    };
}

//------------------//
// LED control code //
//------------------//

bool led_update_kb(led_t led_state) {
    bool res = led_update_user(led_state);
    if(res) {
        writePin(NUM_LED_PIN, led_state.num_lock);
        writePin(CAPS_LED_PIN, led_state.caps_lock);
    }
    return res;
}

//-------------------------------//
//     Power Check functions     //
//-------------------------------//

bool ts_power_check(void) {
    if (RPI_OFF_STATE) {
        dprint("RPI is OFF. Can't switch TS!\n");
        return false;
    } else {
        dprint("RPI is ON. Switching TS:\n");
        return true;
    };
}

bool bl_power_check(void) {
    if (RPI_OFF_STATE) {
        dprint("RPI is OFF. Can't switch BL!\n");
        return false;
    } else {
        dprint("RPI is ON. Switching BL:\n");
        return true;
    };
}

//-----------------//
// Custom Keycodes //
//-----------------//

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        INACTIVE_TIMER = timer_read32();
        if (LOGO_TOG) {
          OLED_TO_CLEAR = true;
        };
    };

    //Debug matrix
    dprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);

    switch (keycode) {
        case USB_SW:
            if (record->event.pressed) {
                dprint("USBSW pressed. Switching USB...\n");
                kb_config.usbswitch_on = !kb_config.usbswitch_on;
                eeconfig_update_kb(kb_config.raw);
                if (kb_config.usbswitch_on) {
                    dprint("PI Mode\n");
                    writePinHigh(USBSW_PIN);
                } else {
                    dprint("PC Mode\n");
                    writePinLow(USBSW_PIN);
                }
            }
            return false;

        case BATMODE:
            if (record->event.pressed) {
                dprint("BATMODE pressed. Switching Fast Charging...\n");
                kb_config.battery_fast_charge_on = !kb_config.battery_fast_charge_on;
                eeconfig_update_kb(kb_config.raw);
                if (kb_config.battery_fast_charge_on) {
                    dprint("Fast Charging is ON\n");
                    writePinHigh(BAT_SEL_PIN);
                    writePinHigh(BAT_PROG_PIN);
                } else {
                    dprint("Fast Charging is OFF\n");
                    writePinLow(BAT_SEL_PIN);
                    writePinLow(BAT_PROG_PIN);
                }
            }
            return false;

        case TS_SW:
            if (record->event.pressed) {
                dprint("TS_SW pressed. Switching Touchscreen...\n");
                if (ts_power_check()) {
                    kb_config.touchscreen_on = !kb_config.touchscreen_on;
                    eeconfig_update_kb(kb_config.raw);
                };
                if (kb_config.touchscreen_on) {
                    dprint("Touchscreen is ON\n");
                    writePinLow(TS_EN_PIN);
                } else {
                    dprint("Touchscreen is OFF\n");
                    writePinHigh(TS_EN_PIN);
                }
            }
            return false;

        case OLED_SW:
            if (record->event.pressed) {
                dprint("OLED_SW pressed. Switching OLED...\n");
                OLED_FORCE_OFF = !OLED_FORCE_OFF;
                if (OLED_FORCE_OFF) {
                  dprint("OLED is OFF\n");
                } else {
                  dprint("OLED is ON\n");
                }
            }
            return false;

        case PG_CYCL:
            if (record->event.pressed) {
                OLED_PAGE ++;
                oled_clear();
            }
            return false;

        case PG_STAT:
            if (record->event.pressed) {
                OLED_PAGE = status_page;
                oled_clear();
            }
            return false;

        case BL_STEP:
            if (record->event.pressed) {
                dprintf("BL level is %u\n", get_backlight_level());
            }
            return true;


        default:
            return true;
    }
}

//------------------------//
// OLED Display functions //
//------------------------//

// PiKeeb Logo
void render_logo(void) {
    static const char PROGMEM pikeeb_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };
    oled_set_cursor(0, 0);
    oled_write_P(pikeeb_logo, false);
}

// Layer state monitor
void render_layer_state(void) {
    static const char PROGMEM layerlogo[] = {
        0xD5, 0xD6, 0x00
    };
    oled_write_P(layerlogo, false);
    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("DFT"), false);
            break;
        case _FN1:
            oled_write_P(PSTR("FN1"), false);
            break;
        case _FN2:
            oled_write_P(PSTR("FN2"), false);
            break;
        case _SET:
            oled_write_P(PSTR("SET"), false);
            break;
        default:
            oled_write_P(PSTR("UND"), false);
    };
    oled_set_cursor(7, 0);
}

// Caps/Num Lock state monitor
void render_lock_state(led_t led_state) {
    static const char PROGMEM locklogo[] = {
        0xD7, 0xD8, 0x00
    };
    oled_write_P(locklogo, false);
    oled_write_P(led_state.num_lock ? PSTR("N") : PSTR("-"), false);
    oled_write_P(led_state.caps_lock ? PSTR("C") : PSTR("-"), false);
    oled_set_cursor(13, 0);
}

void render_touchscreen_state(void) {
    static const char PROGMEM TSlogo[] = {
        0xD9, 0xDA, 0x00
    };
    oled_write_P(TSlogo, false);
    if (RPI_OFF_STATE) {
        oled_write_P(PSTR("---"), false);
    } else {
        oled_write_P(kb_config.touchscreen_on ? PSTR("ON ") : PSTR("OFF"), false);
    };
    oled_set_cursor(19, 0);
}

void render_batery_state(void) {
    static const char PROGMEM bat_charge_logo[] = {
        0x07, 0x08, 0x00
    };
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
    if (VBAT_CHARGING_STATE) {
        oled_write_P(bat_charge_logo, false);
    } if (VBAT <= 4200 && VBAT > 3825) {
        oled_write_P(bat_100_logo, false);
    } else if (VBAT <= 3825 && VBAT > 3700) {
        oled_write_P(bat_75_logo, false);
    } else if (VBAT <= 3700 && VBAT > 3650 ) {
        oled_write_P(bat_50_logo, false);
    } else if (VBAT <= 3650 && VBAT > 3450 ) {
        oled_write_P(bat_25_logo, false);
    } else if (VBAT <= 3450) {
        oled_write_P(bat_0_logo, false);
    };
    oled_set_cursor(0, 2);
}



void render_rpi_state(void) {
    oled_write_P(pilogo, false);
    if (RPIV >= 5300) {
        oled_write_P(PSTR(":OV!"), false);
    } else if (RPIV < 5300 && RPIV >= 4800) {
        oled_write_P(PSTR(":ON "), false);
    } else if (RPIV < 4800 && RPIV >= 3300){
        oled_write_P(PSTR(":UV!"), false);
    } else {
        oled_write_P(PSTR(":OFF"), false);
    };
    oled_set_cursor(7, 2);
}

void render_usb_state(void) {
    static const char PROGMEM pclogo[] = {
        0x99, 0x9A, 0x00
    };
    oled_write_P(PSTR("USB:"), false);
    oled_write_P(kb_config.usbswitch_on ? pilogo : pclogo, false);
    oled_set_cursor(16, 2);
}

void render_backlight_state(void) {
    static const char PROGMEM bl_off_logo[] = {
        0x9D, 0x9E, 0x00
    };
    static const char PROGMEM bl_on_logo[] = {
        0xBD, 0xBE, 0x00
    };
    static const char PROGMEM because_symbol[] = {
        0xBF, 0x00
    };
    if (RPI_OFF_STATE) {
        oled_write_P(bl_off_logo, false);
        oled_write_P(because_symbol, false);
        oled_write_P(pilogo, false);
    } else {
        BL_LVL = get_backlight_level();
        if (BL_LVL == 0) {
            oled_write_P(bl_off_logo, false);
            oled_write_ln_P(PSTR("OFF"), false);
        } else {
            sprintf(BL_LVL_String, "%u", BL_LVL);
            oled_write_P(bl_on_logo, false);
            oled_write_ln(BL_LVL_String, false);
        };
    };
}

void render_status(void) {
    render_layer_state();
    render_lock_state(host_keyboard_led_state());
    render_touchscreen_state();
    render_batery_state();
    render_rpi_state();
    render_usb_state();
    render_backlight_state();
}

void render_voltage(void) {
    oled_write_P(PSTR("VBAT: "), false);
    if (VBAT_CHARGING_STATE) {
        oled_write(VBAT_String, false);
        oled_write_ln_P(PSTR(" (CHRG)"), false);
    } else {
        oled_write_ln(VBAT_String, false);
    };
    oled_set_cursor(0, 1);
    oled_write_P(PSTR("PI: "), false);
    if (RPI_OFF_STATE) {
        oled_write_ln_P(PSTR("0 mV (OFF)"), false);
    } else {
        oled_write_ln(RPIV_String, false);
    };
}

void render_game(void) {
    oled_write_P(PSTR("GAME!!!!"), false);
}

void render_fun(void) {
    oled_write_P(PSTR("FUN!"), false);
}

void check_inactive_timer(void) {
    if (timer_elapsed32(INACTIVE_TIMER) > OLED_OFF_TIME) {
        OLED_TOG = true;
    } else {
        OLED_TOG = false;
    };
    if (timer_elapsed32(INACTIVE_TIMER) > OLED_LOGO_TIME) {
        LOGO_TOG = true;
    } else {
        LOGO_TOG = false;
    }
}

void oled_task_user(void) {
    if (OLED_FORCE_OFF) {
        oled_clear();
        oled_off();
        return;
    };

    check_inactive_timer();

    if (OLED_TOG) {
        oled_clear();
        oled_off();
        return;
    } else {
        oled_on();
    };
    if (OLED_TO_CLEAR) {
        oled_clear();
        OLED_TO_CLEAR = false;
    }
    if (LOGO_TOG) {
        render_logo();
    } else {
        switch (OLED_PAGE) {
            case voltage_page:
                render_voltage();
                break;
            case game_page:
                render_game();
                break;
            case fun_page:
                render_fun();
                break;
            default:
                OLED_PAGE = status_page;
                render_status();
        }
    }
}
