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

/****************************/
/* VOLTAGE MEASUREMENT CODE */
/****************************/

#include "analog.h"

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

// Measurement functions
void vbat_measure(void) {

    // Define values to get destroyed after the function ends
    uint32_t VBAT_RAW = 0;                                  // The RAW value = no caclualtion
    float VBAT_OUT = 0.0;                                   // The OUT value = devided calculated volage
    float VBAT_REAL = 0.0;                                  // The REAL value = calculated voltage
    int VBAT_COUNT = 0;                                     // The counter of number of samples taken
    uint8_t VBAT_10MS_TIMER = 0;                           // The 10ms timer
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
    uint8_t RPIV_10MS_TIMER = 0;                           // The 10ms timer
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
