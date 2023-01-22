// Copyright 2023 ILWrites (@PiKeeb)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Setting up the OLED driver */
#define I2C1_SCL_PIN B8
#define I2C1_SDA_PIN B9
#define I2C_DRIVER I2CD1
#define OLED_FONT_H "./lib/glcdfont_pikeeb.c"
#define OLED_FONT_START 0
#define OLED_FONT_END 255
#define OLED_FONT_WIDTH 6
#define OLED_FONT_HEIGHT 8

/* Audio pin settings */
#define AUDIO_PIN A2
#define AUDIO_PWM_DRIVER PWMD15
#define AUDIO_PWM_CHANNEL 1
#define AUDIO_PWM_PAL_MODE 42

/* Bluetooth pin settings */
#ifdef BLUETOOTH_ENABLE
#   define SPI_SCK_PIN A5
#   define SPI_MISO_PIN A6
#   define SPI_MOSI_PIN A7
#   define SPI_DRIVER SPID1
#   define BLUEFRUIT_LE_RST_PIN C5
#   define BLUEFRUIT_LE_CS_PIN A4
#   define BLUEFRUIT_LE_IRQ_PIN C4
#endif

/* Pointing device pin settings */
#ifdef POINTING_DEVICE_ENABLE
#   define ANALOG_JOYSTICK_X_AXIS_PIN B0
#   define ANALOG_JOYSTICK_Y_AXIS_PIN B1
#   define ANALOG_JOYSTICK_BUTTON0_PIN B15
#   define ANALOG_JOYSTICK_BUTTON1_PIN C6
#endif

/* Switch pin setting */
#define RGB_SW_PIN B11               // Active High
#define USB_SW_PIN A13              // Low: USB = CM4; High: USB = Type C
#define ADC_EN_PIN C2               // Active Low

/* Indicator pin settings */
#define ACT_LED_PIN C15              // Active Low

/* ADC pin settings */
#define VBAT_ADC_PIN C0             // Monitoring Lithium battery voltage, 4.3v - 2.9v (with 1K/2K voltage devider: 2.86v - 1.93v)
#define RPIV_ADC_PIN C1             // Monitoring CM4 input voltage, 5.3v max (with 1K/2K voltage devider)

/* Time defines */
#define VBAT_MEASURE_CYCLE_TIME 10000    // time in ms to take the VBAT voltage measurement
#define VRPI_MEASURE_CYCLE_TIME 12000    // time in ms to take the RPI voltage measurement

#define OLED_INACTIVE_LOGO 60000     // time of inactivity in ms to show the logo
#define OLED_INACTIVE_OFF 120000     // time of inactivity in ms to turn off the oled

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
