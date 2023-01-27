#pragma once
#include QMK_KEYBOARD_H
#include <stdio.h>

#ifndef VBAT_100PERCENT
#define VBAT_100PERCENT 4200
#endif

#ifndef VRPI_OFF
#define VRPI_OFF 1000
#endif

#ifndef TEMP110_CAL_ADDR
#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7C2))
#endif

#ifndef TEMP30_CAL_ADDR
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
#endif

#ifndef VDD_CALIB
#define VDD_CALIB ((uint16_t) (3300))
#endif

#ifndef ADC_CHANNEL_TSENSOR
#define ADC_CHANNEL_TSENSOR 16
#endif

#ifndef ADC_CHANNEL_VMCU
#define ADC_CHANNEL_VMCU 18
#endif

#ifndef MEASURE_CYCLE_TIME_VBAT
#define MEASURE_CYCLE_TIME_VBAT 10000
#endif

#ifndef MEASURE_CYCLE_TIME_VRPI
#define MEASURE_CYCLE_TIME_VRPI 12000
#endif

#ifndef MEASURE_CYCLE_TIME_VMCU
#define MEASURE_CYCLE_TIME_VMCU 5000
#endif

#ifndef MEASURE_CYCLE_TIME_TMCU
#define MEASURE_CYCLE_TIME_TMCU 6000
#endif

#ifndef MEASURE_DELAY
#define MEASURE_DELAY 5
#endif

#ifndef MEASURE_CYCLES
#define MEASURE_CYCLES 10
#endif

#ifndef ADC_RES
#define ADC_RES 4096
#endif

/**
 * Values used in `keyboard_post_init_kb()` and `check_measure_timer()` to handle the output of `*_measure` function
 **/
int VBAT;
int VRPI;
int VMCU;
int TMCU;

/**
 * Timers initialized in `keyboard_post_init_kb()` and used by `check_measure_timer()` to compare against `MEASURE_CYCLE_TIME_*`
 * and trigger corresponding `*_measure` function
 **/
uint16_t VRPI_TIMER;
uint16_t VBAT_TIMER;
uint16_t VMCU_TIMER;
uint16_t TMCU_TIMER;

/**
 * Toggles to render corresponding state of Raspberry Pi and VBAT 
 **/
bool RPI_OFF_STATE(void);
bool VBAT_CHARGING_STATE(void);

/**
 * Strings containing the values used in `render_volt_temp_page()` function. Initialized on every `*_measure` function
 **/
char VRPI_String[9];
char VBAT_String[9];
char VMCU_String[9];
char TMCU_String[9];

/**
 * Function to measure, calculate and output the battery voltage (in mV or 1/1000 of V) based on the analog voltage reading.
 **/
int vbat_measure(void);

/**
 * Function to measure, calculate and output the Raspberry Pi voltage (in mV or 1/1000 of V) based on the analog voltage reading
 **/
int vrpi_measure(void);

/**
 * Function to measure, calculate and output the MCU voltage (in mV or 1/1000 of V) based on the analog voltage reading
 **/
int vmcu_measure(void);

/**
 * Function to measure, calculate and output the MCU temperature (in mC or 1/1000 of C) based on the analog voltage reading.
 * Uses `VMCU` value, so make sure that gets calculated first.
 **/
int tmcu_measure(void);

/**
 * 
 **/
void check_measure_timers (void);