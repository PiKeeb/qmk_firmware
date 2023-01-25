#pragma message "pikeeb.c is included in the compile process"
#include "pikeeb.h"
#include "analog.h"
#include <stdio.h>
#include "debug.h"
#include "lib/bongocat.h"

//--------------------//
// Values & Variables //
//--------------------//

// Conditional values
#if defined OLED_INACTIVE_LOGO && defined OLED_INACTIVE_OFF
#   pragma message "Config's OLED_INACTIVE_* time values are used"
    static uint32_t OLED_LOGO_TIME = OLED_INACTIVE_LOGO;
    static uint32_t OLED_OFF_TIME = OLED_INACTIVE_OFF;
#else
#   pragma message "Default OLED_INACTIVE_* time values are used"
    static uint32_t OLED_LOGO_TIME = 60000;
    static uint32_t OLED_OFF_TIME = 120000;
#endif

#if defined VBAT_MEASURE_CYCLE_TIME && defined VRPI_MEASURE_CYCLE_TIME
#   pragma message "Config's voltage measurement cycle time values are used"
    static uint16_t VBAT_TIME = VBAT_MEASURE_CYCLE_TIME;
    static uint16_t RPIV_TIME = VRPI_MEASURE_CYCLE_TIME;
#else
#   pragma message "Default voltage measurement cycle time values are used"
    static uint16_t VBAT_TIME = 60000;
    static uint16_t RPIV_TIME = 3000;
#endif

// Timers
static uint32_t INACTIVE_TIMER = 0; // Inactivity timer
static uint16_t RPIV_TIMER = 0; // Raspberyy Pi voltage measurement timer
static uint16_t VBAT_TIMER = 0; // Battery voltage measurement timer

// State booleans
static bool RPI_OFF_STATE = true;
static bool VBAT_CHARGING_STATE = false;

// Toggle booleans
static bool OLED_TOG = false;
static bool LOGO_TOG = false;
static bool OLED_TO_CLEAR = false;
static bool OLED_FORCE_OFF = false;

// Current OLED page
static int OLED_PAGE = 0;

// Analog-to-Digital Converter resolution
static int ADC_RES = 1024;
static double VDF = 0.66666666666666; // equals to  2 / 3 because R2 / (R1 + R2), where R1 = 1K & R2 = 2K 

// Full battery voltage in mV
static int VBAT_FULL = 4200;

// Define values for voltage reading
static int VBAT;
static int RPIV;

// Define number of voltage samples to take each measurement cycle
static int NUM_SAMPLES = 10;

// Define delay in ms between each measurement cycle
static int MEASURE_DELAY = 5;

// Strings for sprintf
static char RPIV_String[9];
static char VBAT_String[9];
static char WPM_String[9];

// EEPROM values
typedef union {
  uint32_t raw;
  struct {
    bool usbswitch_high :1;
    uint8_t current_layer_kb :8;
    uint8_t current_layer_oled :8;
  };
} kb_config_t;

kb_config_t kb_config;

//----------------//
//  OLED symbols  //
//----------------//
static const char PROGMEM pilogo[] = {
    0xBB, 0xBC, 0x00
};

static const char PROGMEM pclogo[] = {
    0xB9, 0xBA, 0x00
};

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

static const char PROGMEM pikeeb_logo[] = {
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
};

static const char PROGMEM layerlogo[] = {
    0xD5, 0xD6, 0x00
};

static const char PROGMEM locklogo[] = {
    0xD7, 0xD8, 0x00
};

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

static const char PROGMEM approx_equal_to[] = {
    0x9D, 0x9E, 0x00
};
//----------------------//
// EEPROM configuration //
//----------------------//

// Default EEPROM config in case of the EEPROM reset
void eeconfig_init_kb(void) {
    kb_config.raw = 0;
    kb_config.usbswitch_high = true;    // Default USB = Type C
    kb_config.current_layer_kb = _BASE;     // Default Keyboard layer = _BASE
    kb_config.current_layer_oled = _OLED_STATUS;   // Default OLED layer = _OLED_STATUS
    eeconfig_update_kb(kb_config.raw);  // Write the default KB config to EEPROM
    keyboard_post_init_kb();            // Call the KB level init
}

//-------------------------------//
// Voltage measurement functions //
//-------------------------------//

void vbat_measure(void) {

    // Define values to get destroyed after the function ends
    long RAW_val = 0;
    int mV_DIV = 0;
    int mV_REAL = 0;
    int SAMPLES_TAKEN = 0;  // Samples taken counter
    uint16_t MEASURE_DELAY_TIMER;

    // Start the timer
    MEASURE_DELAY_TIMER = timer_read();

    // Take the measurements
    writePinLow(ADC_EN_PIN); // Disable discharge protection MOSFET
    while (SAMPLES_TAKEN < NUM_SAMPLES) {
        if (timer_elapsed(MEASURE_DELAY_TIMER) > MEASURE_DELAY) {
            RAW_val += analogReadPin(VBAT_ADC_PIN);   // Read the analog voltage and sum it up.
            SAMPLES_TAKEN++;                            // Advance the counter
            MEASURE_DELAY_TIMER = timer_read();         // Restart the timer
        };
    };
    writePinHigh(ADC_EN_PIN); // Enable discharge protection MOSFET

    // Stop the timer
    MEASURE_DELAY_TIMER = 0;

    // Start the calculation
    mV_DIV = RAW_val / ADC_RES / NUM_SAMPLES;
    mV_REAL = mV_DIV / VDF;

    // Output the calculation
    VBAT = mV_REAL;

    // Set the state
    if (VBAT > VBAT_FULL) {
        VBAT_CHARGING_STATE = true;
    } else {
        VBAT_CHARGING_STATE = false;
    }

    // Convert int to string
    sprintf(VBAT_String, "%u mV", mV_REAL);

    // Output to debug console
    dprintf("VBAT_RAW: %u\n", RAW_val);
    dprintf("VBAT_DIV: %u mV\n", mV_DIV);
    dprintf("VBAT_REAL: %u mV\n", mV_REAL);
}

void rpiv_measure(void) {
    // WARNING!!!
    // There is no protection MOSFET or voltage devider on the RPIV line on the first prototype. Modify this code for the second proto.

    // Define values to get destroyed after the function ends
    long RAW_val = 0;
//  int mV_DIV = 0;
    int mV_REAL = 0;
    int SAMPLES_TAKEN = 0;
    uint16_t MEASURE_DELAY_TIMER;

    // Start the timer
    MEASURE_DELAY_TIMER = timer_read();

    // Take the measurements
//  writePinLow(ADC_ENABLE_PIN);
    while (SAMPLES_TAKEN < NUM_SAMPLES) {
        if (timer_elapsed(MEASURE_DELAY_TIMER) > MEASURE_DELAY) {
            RAW_val += analogReadPin(RPIV_ADC_PIN);    // Read the analog voltage and sum it up.
            SAMPLES_TAKEN++;                              // Advance the counter
            MEASURE_DELAY_TIMER = timer_read();             // Restart the timer
        };
    };
//  writePinHigh(ADC_ENABLE_PIN);

    // Stop the timer
    MEASURE_DELAY_TIMER = 0;

    // Start the calculation
//  V_DIV = V_RAW / ADC_RES / NUM_SAMPLES;
//  V_REAL = V_DIV / ( 2 / 3 );                   // here 2 / 3 = R2 / (R1 + R2), where R1 = 1K & R2 = 2K 
    mV_REAL = RAW_val / ADC_RES / NUM_SAMPLES;       

    // Check for noise & set the state
    if (mV_REAL < 1000) {         // Value in mV
        mV_REAL = 0;
        RPI_OFF_STATE = true;
    };

    // Output the calculation
    RPIV = mV_REAL;

    // Convert int to string
    sprintf(RPIV_String, "%u mV", RPIV);

    // Output to debug console
    dprintf("RPIV_RAW: %u\n", RAW_val);
//  dprintf("RPIV_DIV: %u mV\n", mV_DIV);
    dprintf("RPIV: %u mV\n", mV_REAL);
}

//----------------------------//
// Keyboard related functions //
//----------------------------//

// Code that runs after the keyboard is turned on
void keyboard_post_init_kb(void) {

    // Debug enable
    debug_enable = true;
    debug_matrix = true;

    // Setting the pins to output
    setPinOutput(RGB_SW_PIN);
    setPinOutput(USB_SW_PIN);

    // Setting the LED pins to output
    setPinOutput(ACT_LED_PIN);

    // Setting the ADC Voltage reference
    analogReference(ADC_REF_POWER);

    // Setting the ADC pins
    setPinOutput(ADC_EN_PIN);
    setPinInput(VBAT_ADC_PIN);
    setPinInput(RPIV_ADC_PIN);

    // Take the voltage measurements
    vbat_measure();
    rpiv_measure();

    // EEPROM power-on init
    kb_config.raw = eeconfig_read_kb();

    // Switch pins according to the settings of the EEPROM config
    if (kb_config.usbswitch_high) {
        writePinHigh(USB_SW_PIN);
    } else {
        writePinLow(USB_SW_PIN);
    };

    // Read current OLED_PAGE from EEPROM
    OLED_PAGE = kb_config.current_layer_oled;

    // Initialize timers for voltage measurements
    VBAT_TIMER = timer_read();
    RPIV_TIMER = timer_read();

    // Check the inactive time for errors
    if (OLED_OFF_TIME < OLED_LOGO_TIME) {
        OLED_LOGO_TIME = 12000;
        OLED_OFF_TIME = 24000;
    };

    // Check RGB Light state and change the pin.
    if (!rgblight_is_enabled()) {
        writePinLow(RGB_SW_PIN);
    } else {
        writePinHigh(RGB_SW_PIN);
    }
}

//--------------------//
// Matrix scan checks //
//--------------------//

void matrix_scan_kb(void) {
    if (timer_elapsed(VBAT_TIMER) > VBAT_TIME) {
        dprint("VBAT_TIMER elapsed!\n");
        vbat_measure();
        VBAT_TIMER = timer_read();
    };
    if (timer_elapsed(RPIV_TIMER) > RPIV_TIME) {
        dprint("RPIV_TIMER elapsed!\n");
        rpiv_measure();
        RPIV_TIMER = timer_read();
    };
    if (!rgblight_is_enabled()) {
        writePinLow(RGB_SW_PIN);
    } else {
        writePinHigh(RGB_SW_PIN);
    }
}

//-----------------//
// Custom Keycodes //
//-----------------//
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        INACTIVE_TIMER = 0;
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
                dprint("USB_SW is pressed. Switching USB...\n");
                kb_config.usbswitch_high = !kb_config.usbswitch_high;
                eeconfig_update_kb(kb_config.raw);
                if (kb_config.usbswitch_high) {
                    dprint("CM Mode\n");
                    writePinHigh(USB_SW_PIN);
                } else {
                    dprint("CM Mode\n");
                    writePinLow(USB_SW_PIN);
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

        case OLED_PG_CYCL:
            if (record->event.pressed) {
                OLED_PAGE ++;
                kb_config.current_layer_oled = OLED_PAGE;
                eeconfig_update_kb(kb_config.raw);
                oled_clear();
            }
            return false;

        case OLED_PG_STAT:
            if (record->event.pressed) {
                OLED_PAGE = _OLED_STATUS;
                kb_config.current_layer_oled = OLED_PAGE;
                eeconfig_update_kb(kb_config.raw);
                oled_clear();
            }
            return false;

        default:
            return true;
    }
}

//------------------------//
// OLED Display functions //
//------------------------//

// PiKeeb Logo
void render_logo(void) {
    oled_set_cursor(0, 0);
    oled_write_P(pikeeb_logo, false);
}

// Layer state monitor
void render_layer_state(void) {
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
    oled_write_P(locklogo, false);
    oled_write_P(led_state.num_lock ? PSTR("N") : PSTR("-"), false);
    oled_write_P(led_state.caps_lock ? PSTR("C") : PSTR("-"), false);
    oled_set_cursor(13, 0);
}

void render_batery_state(void) {
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
    oled_write_P(PSTR("USB:"), false);
    oled_write_P(kb_config.usbswitch_high ? pilogo : pclogo, false);
    oled_set_cursor(16, 2);
}

void render_status(void) {
    render_layer_state();
    render_lock_state(host_keyboard_led_state());
    render_batery_state();
    render_rpi_state();
    render_usb_state();
}

void render_voltage(void) {
    oled_write_P(PSTR("VBAT"), false);
    oled_write_P(approx_equal_to, false);
    if (VBAT_CHARGING_STATE) {
        oled_write(VBAT_String, false);
        oled_write_ln_P(PSTR(" (CHRG)"), false);
    } else {
        oled_write_ln(VBAT_String, false);
    };
    oled_set_cursor(0, 1);
    oled_write_P(PSTR("RPIV"), false);
    oled_write_P(approx_equal_to, false);
    if (RPI_OFF_STATE) {
        oled_write_ln_P(PSTR("0 mV (OFF)"), false);
    } else {
        oled_write_ln(RPIV_String, false);
    };
}

void render_wpm(void) {
    render_anim();                                          // renders Bongocat
    oled_set_cursor(0, 0);                                  // sets cursor to (row, column) using charactar spacing (5 rows on 128x32 screen, anything more will overflow back to the top)
    sprintf(WPM_String, "WPM:%03d", get_current_wpm());     // edit the string to change wwhat shows up, edit %03d to change how many digits show up
    oled_write(WPM_String, false);                          // writes wpm on top left corner of string
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

//--------------------//
// Main OLED function //
//--------------------//

bool oled_task_kb(void) {
    // Defer to the keymap if they want to override
    if(!oled_task_user()) {
        return false;
    }

    // Check for force OFF state
    if (OLED_FORCE_OFF) {
        oled_clear();
        oled_off();
        return false;
    };

    // Check inactivity timer
    check_inactive_timer();

    if (OLED_TOG) {
        oled_clear();
        oled_off();
        return false;
    } else {
        oled_on();
    };

    if (OLED_TO_CLEAR) {
        oled_clear();
        OLED_TO_CLEAR = false;
    };

    if (LOGO_TOG) {
        render_logo();
    } else {
        switch (OLED_PAGE) {
            case _OLED_VOLTAGE:
                render_voltage();
                break;
            case _OLED_WPM:
                render_wpm();
                break;
            case _OLED_FUN:
                render_fun();
                break;
            default:
                OLED_PAGE = _OLED_STATUS;
                render_status();
        };
    };
    return false;
}