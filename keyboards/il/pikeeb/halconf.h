#pragma once

// ADC
#define HAL_USE_ADC TRUE

// SPI
#ifdef BLUETOOTH_ENABLE
#define HAL_USE_SPI TRUE
#endif

// I2C
#define HAL_USE_I2C TRUE

// PWM
#define HAL_USE_PWM TRUE

#include_next <halconf.h>
