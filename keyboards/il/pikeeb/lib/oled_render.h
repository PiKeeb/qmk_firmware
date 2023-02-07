#pragma once
#include QMK_KEYBOARD_H

#ifndef OLED_INACTIVE_TIME_LOGO 
#   define OLED_INACTIVE_TIME_LOGO 60000
#endif

#ifndef OLED_INACTIVE_TIME_OFF 
#   define OLED_INACTIVE_TIME_OFF 120000
#endif

/**
 * Inactivity timer. Used in fuction `check_inactive_timer() and compares its value 
 * to `OLED_INACTIVE_TIME_OFF` and `OLED_INACTIVE_TIME_LOGO` to toggle `OLED_TOG` and `LOGO_TOG`.
 **/
uint32_t INACTIVE_TIMER;

/**
 * Toggle. Initialized by the fuction `check_inactive_timer()`
 * and used in `oled_task_kb` turn the OLED on or off
 **/
bool OLED_TOG;

/**
 * Toggle. Initialized by the fuction `check_inactive_timer()`
 * and used in `oled_task_kb` to ewoke `render_logo()` function
 **/
bool LOGO_TOG;

/**
 * String containing the WPM value used in `render_wpm_page()` function
 **/
char WPM_String[9];

/**
 * Function to render the logo contained in `glcfont`
 **/
void render_logo(void);

/**
 * Function to render the current active layer
 **/
void render_layer_state(void);

/**
 * Function to render the current CapsLock and NumLock states
 **/
void render_lock_state(led_t led_state);

/**
 * Function to render the current state of the battery based on the analog voltage reading
 **/
void render_batery_state(void);

/**
 * Function to render the current state of the Raspberry Pi VBUS based on the analog voltage reading
 **/
void render_rpi_state(void);

/**
 * Function to render the current state of the USB switch based on the value stored in EEPROM.
 **/
void render_usbsw_state(void);

/**
 * Function to render the status page based on the `render_*_state` functions
 **/
void render_status_page(void);

/**
 * Function to render the page of values of `VBAT` and `VRPI` based on the analog voltage reading
 **/
void render_volt_page(void);

/**
 * Function to render the page containing the responsive animation of bongo cat and WPM counter
 **/
void render_wpm_page(void);

/**
 * Function to render the page with the snake game
 **/
void render_game_page(void);

/**
 * Function to check the `INACTIVE_TIMER` value and to toggle `OLED_TOG` and `LOGO_TOG`.
 **/
void check_inactive_timer(void);