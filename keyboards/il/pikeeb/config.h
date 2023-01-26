// Copyright 2023 ILWrites (@PiKeeb)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Setting up the OLED driver */

#   define OLED_FONT_H "./lib/glcdfont_pikeeb.c"
#   define OLED_FONT_START 0
#   define OLED_FONT_END 255
#   define OLED_FONT_WIDTH 6
#   define OLED_FONT_HEIGHT 8

/* Audio pin settings */
#   define AUDIO_PIN C6

/* Bluetooth pin settings */
#ifdef BLUETOOTH_ENABLE
#   define BLUEFRUIT_LE_RST_PIN D4
#   define BLUEFRUIT_LE_CS_PIN B4
#   define BLUEFRUIT_LE_IRQ_PIN E6
#endif

/* Pointing device pin settings */
#ifdef POINTING_DEVICE_ENABLE
#   define ANALOG_JOYSTICK_X_AXIS_PIN F7
#   define ANALOG_JOYSTICK_Y_AXIS_PIN F6
#   define ANALOG_JOYSTICK_BUTTON0_PIN A1
#   define ANALOG_JOYSTICK_BUTTON1_PIN A0
#endif

/* Switch pin setting */
#define RGB_SW_PIN A5               // Active High
#define USB_SW_PIN B0               // Low: USB = CM4; High: USB = Type C
#define ADC_EN_PIN E7               // Active Low

/* Indicator pin settings */
#define ACT_LED_PIN E3              // Active Low

/* ADC pin settings */
#define VBAT_ADC_PIN F0             // Monitoring Lithium battery voltage, 4.3v - 2.9v (with 1K/2K voltage devider: 2.86v - 1.93v)
#define VRPI_ADC_PIN F1             // Monitoring CM4 input voltage, VCC/5.3v max (with 1K current limiting resistor)

#define OLED_INACTIVE_TIME_LOGO 60000
#define OLED_INACTIVE_TIME_OFF 120000
#define MEASURE_CYCLE_TIME_VBAT 10000
#define MEASURE_CYCLE_TIME_VRPI 12000

#define BONGOCAT_ENABLE