#pragma once
#include QMK_KEYBOARD_H
#include <stdio.h>

#ifndef VBAT_100PERCENT
#define VBAT_100PERCENT 4200
#endif

#ifndef VRPI_OFF
#define VRPI_OFF 1000
#endif

#ifndef MEASURE_CYCLE_TIME_VBAT
#define MEASURE_CYCLE_TIME_VBAT 10000
#endif

#ifndef MEASURE_CYCLE_TIME_VRPI
#define MEASURE_CYCLE_TIME_VRPI 12000
#endif

#ifndef MEASURE_DELAY
#define MEASURE_DELAY 5
#endif

#ifndef MEASURE_CYCLES
#define MEASURE_CYCLES 10
#endif

#ifndef ADC_RES
#define ADC_RES 1024
#endif

/**
 * Values used in `keyboard_post_init_kb()` and `check_measure_timer()` to handle the output of `*_measure` function
 **/
int VBAT;
int VRPI;

/**
 * Timers initialized in `keyboard_post_init_kb()` and used by `check_measure_timer()` to compare against `MEASURE_CYCLE_TIME_*`
 * and trigger corresponding `*_measure` function
 **/
uint16_t VRPI_TIMER;
uint16_t VBAT_TIMER;

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

/**
 * Function to measure, calculate and output the battery voltage (in mV or 1/1000 of V) based on the analog voltage reading.
 **/
int vbat_measure(void);

/**
 * Function to measure, calculate and output the Raspberry Pi voltage (in mV or 1/1000 of V) based on the analog voltage reading
 **/
int vrpi_measure(void);

/**
 * 
 **/
void check_measure_timers (void);