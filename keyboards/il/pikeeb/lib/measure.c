#include "measure.h"
#include <analog.h>
#include <stdio.h>
#include <debug.h>

bool VBAT_CHARGING_STATE (void) {
    if ( VBAT >= VBAT_100PERCENT ) {
        return true;
    } else {
        return false;
    }
};

bool RPI_OFF_STATE (void) {
    if ( VRPI <= VRPI_OFF ) {
        return true;
    } else {
        return false;
    }
};

const double VRPI_VBAT_VDF = 0.66666666666666;  // for VRPI and VBAT equals to  2 / 3 because R2 / (R1 + R2), where R1 = 1K & R2 = 2K

int vbat_measure(void) {

    // Define values to get destroyed after the function ends
    long RAW_val = 0;
    int mV_DIV = 0;
    int mV_REAL = 0;
    int CURRENT_CYCLE = 0;  // Samples taken counter
    uint16_t MEASURE_DELAY_TIMER;

    // Start the timer
    MEASURE_DELAY_TIMER = timer_read();

    // Take the measurements
    writePinLow(ADC_EN_PIN); // Disable discharge protection MOSFET
    while (CURRENT_CYCLE < MEASURE_CYCLES) {
        if (timer_elapsed(MEASURE_DELAY_TIMER) > MEASURE_DELAY) {
            RAW_val += analogReadPin(VBAT_ADC_PIN);
            CURRENT_CYCLE++;
            MEASURE_DELAY_TIMER = timer_read();
        };
    };
    writePinHigh(ADC_EN_PIN); // Enable discharge protection MOSFET

    // Start the calculation
    mV_DIV = RAW_val / ADC_RES / MEASURE_CYCLES;
    mV_REAL = mV_DIV / VRPI_VBAT_VDF;

    // Output to debug console
    dprintf("VBAT_RAW: %lu\n", RAW_val);
    dprintf("VBAT_DIV: %u mV\n", mV_DIV);
    dprintf("VBAT_REAL: %u mV\n", mV_REAL);

    // Output to string
    sprintf(VBAT_String, "%u mV", (int)mV_REAL);
    
    // Output the calculation
    return mV_REAL;
}

int vrpi_measure(void) {
    // Define values to get destroyed after the function ends
    long RAW_val = 0;
    int mV_DIV = 0;
    int mV_REAL = 0;
    int CURRENT_CYCLE = 0;
    uint16_t MEASURE_DELAY_TIMER;

    // Start the timer
    MEASURE_DELAY_TIMER = timer_read();

    // Take the measurements
    writePinLow(ADC_EN_PIN);
    while (CURRENT_CYCLE < MEASURE_CYCLES) {
        if (timer_elapsed(MEASURE_DELAY_TIMER) > MEASURE_DELAY) {
            RAW_val += analogReadPin(VRPI_ADC_PIN);
            CURRENT_CYCLE++;
            MEASURE_DELAY_TIMER = timer_read();
        };
    };
    writePinHigh(ADC_EN_PIN);

    // Start the calculation
    mV_DIV = RAW_val / ADC_RES / MEASURE_CYCLES;
    mV_REAL = mV_DIV / VRPI_VBAT_VDF;

    // Output to debug console
    dprintf("VRPI_RAW: %lu\n", RAW_val);
    dprintf("VRPI_DIV: %u mV\n", mV_DIV);
    dprintf("VRPI: %u mV\n", mV_REAL);

    // Output to string
    sprintf(VRPI_String, "%u mV", (int)mV_REAL);
    
    // Output the calculation
    return mV_REAL;
}

void check_measure_timers(void){
    if (timer_elapsed(VBAT_TIMER) > MEASURE_CYCLE_TIME_VBAT) {
        dprint("VBAT_TIMER elapsed!\n");
        VBAT = vbat_measure();
        VBAT_TIMER = timer_read();
    }
    if (timer_elapsed(VRPI_TIMER) > MEASURE_CYCLE_TIME_VRPI) {
        dprint("VRPI_TIMER elapsed!\n");
        VRPI = vrpi_measure();
        VRPI_TIMER = timer_read();
    }
}