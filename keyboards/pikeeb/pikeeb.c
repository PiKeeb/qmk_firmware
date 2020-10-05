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

#ifdef INACT_LOGO
    static uint32_t INACTIVE_TIME_LOGO = INACT_LOGO;
#else
    static uint32_t INACTIVE_TIME_LOGO = 60000;
#endif

#ifdef INACT_OFF
    static uint32_t INACTIVE_TIME_OFF = INACT_OFF;
#else
    static uint32_t INACTIVE_TIME_OFF = 120000;
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
static uint16_t BLINK_TIMER = 0; // Blink timer for the battery indicator

// Define toggle booleans
static bool BLINK_TOG = false;
static bool OLED_TOG = false;
static bool OLED_FORCE_OFF = false;

// Voltage measurement defines //

// Define number of samples to take voltage measurements
static int NUM_VSAMPLES = 10;

// Define voltage devision factor
static float VDF = 9.33;

// Define voltage reference
static float VREF = 5.00;

// Define ADC resolution
static int ADC_RES = 1024;

// Define values for voltage reading
static float VBAT;
static float RPIV;

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
    kb_config.touchscreen_on = false; // We want this switched off (Touch Screen is on)
    eeconfig_update_kb(kb_config.raw); // Write the default KB config to EEPROM
    keyboard_post_init_kb(); // Call the KB level init

    // Hand control to *_user
    eeconfig_init_user();
}

//-------------------------------//
// Voltage Measurement functions //
//-------------------------------//

void vbat_measure(void) {

    // Define values to get destroyed after the function ends
    uint32_t VBAT_RAW = 0;                                  // The RAW value = no caclualtion
    float VBAT_OUT = 0.0;                                   // The OUT value = devided calculated volage
    float VBAT_REAL = 0.0;                                  // The REAL value = calculated voltage
    int VBAT_COUNT = 0;                                     // The counter of number of samples taken
    uint8_t VBAT_10MS_TIMER = 0;                            // The 10ms timer
    bool VBAT_10MS_TIMER_TOG = false;                       // The toggle to get the measurement

    // Take the measurements
    writePinHigh(VBAT_MON_EN_PIN);                          // Disable discharge protection MOSFET
    VBAT_10MS_TIMER = timer_read();                         // Enable 10ms timer
    while (VBAT_COUNT < NUM_VSAMPLES) {
      if (timer_elapsed(VBAT_10MS_TIMER) > 10) {            // 10ms timer to flip the toggle
          VBAT_10MS_TIMER = timer_read();                   // Reset the timer
          VBAT_10MS_TIMER_TOG = !VBAT_10MS_TIMER_TOG;       // Flip the bit
      };
      if (VBAT_10MS_TIMER_TOG) {
          VBAT_RAW += analogReadPin(VBAT_MON_ADC_PIN);      // Read the analog voltage and sum it up.
          VBAT_COUNT++;                                     // Increment the counter by 1
      };
    };
    writePinLow(VBAT_MON_EN_PIN);                           // Enable discharge protection MOSFET

    // Start the calculation
    VBAT_OUT = (VBAT_RAW / NUM_VSAMPLES * VREF) / ADC_RES;  // Make the calculation for the OUT voltage
    VBAT_REAL = VBAT_OUT * VDF;                             // Make the calculation for the REAL voltage

    // Output the calculation
    VBAT = VBAT_REAL;                                       // Output the REAL value into the static
}

void rpiv_measure(void) {

    // Define values to get destroyed after the function ends
    uint32_t RPIV_RAW = 0;                                  // The RAW value = no caclualtion
    float RPIV_OUT = 0.2;                                   // The OUT value = devided calculated volage
    float RPIV_REAL = 0.0;                                  // The REAL value = calculated voltage
    int RPIV_COUNT = 0;                                     // The counter of number of samples taken
    uint8_t RPIV_10MS_TIMER = 0;                            // The 10ms timer
    bool RPIV_10MS_TIMER_TOG = false;                       // The toggle to get the measurement

    // Take the measurements
    RPIV_10MS_TIMER = timer_read();                         // Enable 10ms timer
    while (RPIV_COUNT < NUM_VSAMPLES) {
        if (timer_elapsed(RPIV_10MS_TIMER) > 10) {          // 10ms timer to flip the toggle
            RPIV_10MS_TIMER = timer_read();                 // Reset the timer
            RPIV_10MS_TIMER_TOG = !RPIV_10MS_TIMER_TOG;     // Flip the bit
        };
        if (RPIV_10MS_TIMER_TOG) {
            RPIV_RAW += analogReadPin(RPIV_MON_ADC_PIN);    // Read the analog voltage and sum it up.
            RPIV_COUNT++;                                   // Increment the counter by 1
        };
    };

    // Start the calculation
    RPIV_OUT = (RPIV_RAW / NUM_VSAMPLES * VREF) / ADC_RES;  // Make the calculation for the OUT voltage
    RPIV_REAL = RPIV_OUT * VDF;                             // Make the calculation for the REAL voltage

    // Output the calculation
    if (RPIV_REAL < 0.5) {
        RPIV_REAL = 0;                                      // Account for noise
    };
    RPIV = RPIV_REAL;                                       // Output the REAL value to the static
}

//-----------------//
// Check functions //
//-----------------//

void time_check(void) {
    if (INACTIVE_TIME_OFF<INACTIVE_TIME_LOGO) {
        INACTIVE_TIME_LOGO = 60000;
        INACTIVE_TIME_OFF = 120000;
    };
}


//----------------------------//
// Keyboard related functions //
//----------------------------//

// Enable EEPROM and initialize the timers
void keyboard_post_init_kb(void) { // Call the keymap level matrix init.
    kb_config.raw = eeconfig_read_kb(); // Read the user config from EEPROM
    // Switch pins according to the settings of the EEPROM config
    if (kb_config.usbswitch_on) {
        writePinHigh(USBSW_PIN);
        writePinHigh(USBSW_LED_PIN);
    } else {
        writePinLow(USBSW_PIN);
        writePinLow(USBSW_LED_PIN);
    }
    if (kb_config.battery_fast_charge_on) {
        writePinHigh(BAT_SEL_PIN);
        writePinHigh(BAT_PROG_PIN);
        writePinHigh(FCH_LED_PIN);
    } else {
        writePinLow(BAT_SEL_PIN);
        writePinLow(BAT_PROG_PIN);
        writePinLow(FCH_LED_PIN);
    }
    if (kb_config.touchscreen_on) {
        writePinHigh(TS_EN_PIN);
    } else {
        writePinLow(TS_EN_PIN);
    };

    // Initialize timers for blink & voltage measurements
    BLINK_TIMER = timer_read();
    VBAT_TIMER = timer_read32();
    RPIV_TIMER = timer_read32();

    // Check the inactive time for errors
    time_check();

    // Take the voltage measurement
    vbat_measure();
    rpiv_measure();

    // Hand control to *_user
    keyboard_post_init_user();
}

void matrix_scan_kb(void) {
   if (timer_elapsed32(VBAT_TIMER) > VBAT_TIME) { // initiate the RPi voltage measurement every 60 seconds
       vbat_measure();
       VBAT_TIMER = timer_read32();
   }
   if (timer_elapsed32(RPIV_TIMER) > RPIV_TIME) {
       rpiv_measure();
       RPIV_TIMER = timer_read32();
   };

   // Hand control to *_user
   matrix_scan_user();
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

//---------------------//
// Switching functions //
//---------------------//

void switch_usb(void) {
    kb_config.usbswitch_on = !kb_config.usbswitch_on;
    eeconfig_update_kb(kb_config.raw);
    if (kb_config.usbswitch_on) { // OFF = PC MODE
        writePinHigh(USBSW_PIN);
        writePinHigh(USBSW_LED_PIN);
    } else {
        writePinLow(USBSW_PIN);
        writePinLow(USBSW_LED_PIN);
    };
}

void switch_batmode(void) {
    kb_config.battery_fast_charge_on = !kb_config.battery_fast_charge_on;
    eeconfig_update_kb(kb_config.raw);
    if (kb_config.battery_fast_charge_on) { // ON = FAST CHARGING (1A MAX)
        writePinHigh(BAT_SEL_PIN);
        writePinHigh(BAT_PROG_PIN);
        writePinHigh(FCH_LED_PIN);
    } else {
        writePinLow(BAT_SEL_PIN);
        writePinLow(BAT_PROG_PIN);
        writePinLow(FCH_LED_PIN);
    };
}

void switch_ts(void) {
    kb_config.touchscreen_on = !kb_config.touchscreen_on;
    eeconfig_update_kb(kb_config.raw);
    if (kb_config.touchscreen_on) {
        writePinHigh(TS_EN_PIN);
    } else {
        writePinLow(TS_EN_PIN);
    };
}

//-----------------//
// Custom Keycodes //
//-----------------//

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
      INACTIVE_TIMER = timer_read32();
    };
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
        case DISP_SW:
            if (record->event.pressed) {
                OLED_FORCE_OFF = !OLED_FORCE_OFF;
            }
        break;
    };
    // Hand control to *_user
    return process_record_user(keycode, record);
};

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
    oled_write_P(pikeeb_logo, false);
}

// Layer state monitor
void render_layer_state(void) {
    static const char PROGMEM layerlogo[] = {
        0xD5, 0xD6, 0x00
    };
    oled_write_P(layerlogo, false);
    if (layer_state_is(_BASE)) {
        oled_write_P(PSTR("DFT"), false);
    };
    if (layer_state_is(_FN1)) {
        oled_write_P(PSTR("FN1"), false);
    };
    if (layer_state_is(_FN2)) {
        oled_write_P(PSTR("FN2"), false);
    };
    if (layer_state_is(_SET)) {
        oled_write_P(PSTR("SET"), false);
    };
}

// Caps/Num Lock state monitor
void render_lock_state(led_t led_state) {
    static const char PROGMEM locklogo[] = {
        0xD7, 0xD8, 0x00
    };
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
    oled_write_P(kb_config.touchscreen_on ? onlogo : offlogo, false);
}

void render_rpi_state(void) {
    static const char PROGMEM RPIsmalllogo[] = {
        0x9F, 0xBF, 0x00
    };
    oled_write_P(RPIsmalllogo, false);
    if (RPIV>= 5.3) {
        oled_write_P(PSTR("OV!"), false);
    } else if (RPIV< 5.3 && RPIV>= 4.8) {
        oled_write_P(PSTR("ON "), false);
    } else if (RPIV< 4.8 && RPIV>= 3.3){
        oled_write_P(PSTR("UV!"), false);
    } else {
        oled_write_P(PSTR("OFF"), false);
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
    if (VBAT > 3.9) {
        oled_write_P(bat_100_logo, false);
    } else if (VBAT <= 3.9 && VBAT > 3.6) {
        oled_write_P(bat_75_logo, false);
    } else if (VBAT <= 3.6 && VBAT > 3.5 ) {
        oled_write_P(bat_50_logo, false);
    } else if (VBAT <= 3.5 && VBAT > 3.2 ) {
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
    if (OLED_FORCE_OFF) {
        oled_off();
        return;
    }
    if (timer_elapsed32(INACTIVE_TIMER) > INACTIVE_TIME_OFF) {
        OLED_TOG = true;
    } else if (timer_elapsed32(INACTIVE_TIMER) > INACTIVE_TIME_LOGO) {
        render_logo();
    } else {
        OLED_TOG = false;
    };
    // When the bit is on (true), turn off the display
    if (OLED_TOG) {
        oled_off();
        return;
    } else {
        oled_on();
    };
    render_status();
}
