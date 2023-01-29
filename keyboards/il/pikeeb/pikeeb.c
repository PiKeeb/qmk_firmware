#include "pikeeb.h"
#include "lib/oled_render.h"
#include "lib/measure.h"
#include "lib/snake.h"

//--------------------//
// Values & Variables //
//--------------------//

// Toggle booleans
bool ACT_TOG = false;
bool BACK_SW_is_low;
bool OLED_TO_CLEAR = false;
bool OLED_FORCE_OFF = false;

// Current OLED page
int OLED_PAGE = 0;

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
#ifdef EEPROM_HIGHSCORE
    gameData.raw = 0;
    gameData.storedHighScore = 0;
    eeconfig_update_kb(gameData.raw);
#endif
    keyboard_post_init_kb();            // Call the KB level init
}

//----------------------------//
// Keyboard related functions //
//----------------------------//

// Code that runs after the keyboard is turned on
void keyboard_pre_init_kb(void) {
    // Setting the pins to output
    setPinOutput(RGB_SW_PIN);
    setPinOutput(USB_SW_PIN);

    // Setting the LED pins to output
    setPinOutput(ACT_LED_PIN);

    // Setting the ADC pins
    setPinOutput(ADC_EN_PIN);
    setPinInput(VBAT_ADC_PIN);
    setPinInput(VRPI_ADC_PIN);

    // Setting the physical switch pin
    setPinInputHigh(BACK_SW_PIN);

    // Wake up the temperature sensor
    ADC->CCR |= ADC_CCR_TSEN;
}

void keyboard_post_init_kb(void) {
    // Debug enable
    debug_enable = true;
    debug_matrix = true;

    // Take the voltage measurements
    VBAT = vbat_measure();
    VRPI = vrpi_measure();
    VMCU = vmcu_measure();

    // Take the temperature measurement
    TMCU = tmcu_measure();

    // Initialize timers for voltage measurements
    VBAT_TIMER = timer_read();
    VRPI_TIMER = timer_read();
    VMCU_TIMER = timer_read();
    TMCU_TIMER = timer_read();

    // Check if BACK_SW_PIN is low
    if (readPin(BACK_SW_PIN)) {
        BACK_SW_is_low = true;
    } else {
        BACK_SW_is_low = false;
    }

    // EEPROM power-on init
    kb_config.raw = eeconfig_read_kb();

    // Switch pins according to the settings of the EEPROM config
    if (kb_config.usbswitch_high) {
        writePinHigh(USB_SW_PIN);
    } else {
        writePinLow(USB_SW_PIN);
    }

    // Read current OLED_PAGE from EEPROM
    OLED_PAGE = kb_config.current_layer_oled;

    // Check RGB Light state and change the pin.
    if (!rgblight_is_enabled()) {
        writePinLow(RGB_SW_PIN);
    } else {
        writePinHigh(RGB_SW_PIN);
    }
}

/* Housekeeping checks */

void housekeeping_task_kb(void) {
    check_measure_timers();
    // Toggles
    if (!rgblight_is_enabled()) {
        writePinLow(RGB_SW_PIN);
    } else {
        writePinHigh(RGB_SW_PIN);
    }
    if (ACT_TOG) {
        writePinLow(ACT_LED_PIN);
    }
    else {
        writePinHigh(ACT_LED_PIN);
    }
}

/* Key Record checks */

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // Check inactivity timer
    if (record->event.pressed) {
        INACTIVE_TIMER = timer_read32();
        if (LOGO_TOG) {
          OLED_TO_CLEAR = true;
        };
        ACT_TOG = !ACT_TOG;
    };

    // Debug matrix
    dprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);

    // Custom keycodes
    switch (keycode) {
        // USB Switch
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
        // OLED Force Off
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
        // OLED Page Cycle
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

// SNAKE control functions.

#ifdef SNAKE_ENABLE

        // Move UP with W or UP
        case KC_W:
            if (record->event.pressed && game_is_running) {
                snake_dir = sUP;
            }
            moveCheck();
            return true;

        case KC_UP:
            if (record->event.pressed && game_is_running) {
                snake_dir = sUP;
            }
            moveCheck();
            return true;

        // Move LEFT with A or LEFT
        case KC_A: 
            if (record->event.pressed && game_is_running) {
                snake_dir = sLEFT;
            }
            moveCheck();
            return true;

        case KC_LEFT:
            if (record->event.pressed && game_is_running) {
                snake_dir = sLEFT;
            }
            moveCheck();
            return true;

        // Move DOWN with S or DOWN
        case KC_S: 
            if (record->event.pressed && game_is_running) {
                snake_dir = sDOWN;
            }
            moveCheck();
            return true;

        case KC_DOWN:
            if (record->event.pressed && game_is_running) {
                snake_dir = sDOWN;
            }
            moveCheck();
            return true;

        // Move RIGHT with D or RRIGHT.
        case KC_D: 
            if (record->event.pressed && game_is_running) {
                snake_dir = sRIGHT;
            }
            moveCheck();
            return true;

        case KC_RIGHT:
            if (record->event.pressed && game_is_running) {
                snake_dir = sRIGHT;
            }
            moveCheck();
            return true;
        
        // Restart the game when it asks "Play again? y/n"
        case KC_Y:
            if (record->event.pressed && !game_is_running) {
                snakeRestart = !snakeRestart;
            }
            return true;

        // Exit the game when it asks  "Play again? y/n"
        case KC_N:
            if (record->event.pressed && !game_is_running) {
                OLED_PAGE = _OLED_STATUS;
            }
            return true;
        
        // Force-exit the game when it's running.
        case KC_ESC:
            if (record->event.pressed && game_is_running) {
                OLED_PAGE = _OLED_STATUS;
            }
            return true;
#endif
        default:
            return true;
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
                render_volt_temp_page();
                break;
            case _OLED_WPM:
                render_wpm_page();
                break;
            case _OLED_FUN:
                render_fun_page();
                break;
            default:
                OLED_PAGE = _OLED_STATUS;
                render_status_page();
        };
    };
    return false;
}