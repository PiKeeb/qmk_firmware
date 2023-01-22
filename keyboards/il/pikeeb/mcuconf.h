#pragma once

#include_next <mcuconf.h>

// ADC
#undef STM32_ADC_USE_ADC1
#define STM32_ADC_USE_ADC1      TRUE

// SPI
#ifdef BLUETOOTH_ENABLE
#undef STM32_SPI_USE_SPI1
#define STM32_SPI_USE_SPI1      TRUE
#endif

// I2C
#undef STM32_I2C_USE_I2C1
#define STM32_I2C_USE_I2C1      TRUE

// PWM
#undef STM32_PWM_USE_TIM15
#define STM32_PWM_USE_TIM15      TRUE
#define STM32_PWM_TIM15_IRQ_PRIORITY 3
#define STM32_TIM15_SUPPRESS_ISR