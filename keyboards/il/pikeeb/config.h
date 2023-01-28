// Copyright 2023 ILWrites (@PiKeeb)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Setting up the OLED driver */
#ifdef OLED_ENABLE
#   define I2C1_SCL_PIN B8
#   define I2C1_SDA_PIN B9
#   define I2C_DRIVER I2CD1
#   define OLED_FONT_H "./lib/glcdfont_pikeeb.c"
#   define OLED_FONT_START 0
#   define OLED_FONT_END 255
#   define OLED_FONT_WIDTH 6
#   define OLED_FONT_HEIGHT 8
#endif

/* Audio pin settings */
#ifdef AUDIO_ENABLE
#   define AUDIO_PIN A2
#   define AUDIO_PWM_DRIVER PWMD15
#   define AUDIO_PWM_CHANNEL 1
#   define AUDIO_PWM_PAL_MODE 42
#endif

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

/* ADC channel settings */
#define ADC_CHANNEL_TSENSOR 16       //
#define ADC_CHANNEL_VMCU 18

/* ADC pin settings */
#define VBAT_ADC_PIN C0             // Monitoring Lithium battery voltage, 4.3v - 2.9v (with 1K/2K voltage devider: 2.86v - 1.93v)
#define VRPI_ADC_PIN C1             // Monitoring CM4 input voltage, 5.3v max (with 1K/2K voltage devider)

// Calibrated temperature sensor values addresses from datasheet (sections 3.10.1 and 3.10.2, tables 3 and 4)
// Addresses from memory for temperature reading
#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7C2))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
// VDD during calibration (usually, 3.3V or 3300mV)
#define VDD_CALIB ((uint16_t) (3300)) //mv

/* Time defines */
#define MEASURE_CYCLE_TIME_VBAT 10000   // time in ms to take the VBAT voltage measurement
#define MEASURE_CYCLE_TIME_VRPI 12000   // time in ms to take the RPI voltage measurement
#define MEASURE_CYCLE_TIME_VMCU 5000    // time in ms to take the MCU voltage measurement
#define MEASURE_CYCLE_TIME_TMCU 6000    // time in ms to take the MCU temperature measurement

#define OLED_INACTIVE_TIME_LOGO 60000   // time of inactivity in ms to show the logo
#define OLED_INACTIVE_TIME_OFF 120000   // time of inactivity in ms to turn off the oled

#ifdef RGBLIGHT_ENABLE
#   define RGBLIGHT_EFFECT_RAINBOW_MOOD
#   define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#   define RGBLIGHT_LAYERS 
#endif