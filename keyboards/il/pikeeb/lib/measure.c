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
const int VMCU_VDF = 2;                         // according to datasheet (section 3.10.3), the VBAT pin is internally connected to a bridge divider by 2

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

int vmcu_measure(void) {
    // Define values to get destroyed after the function ends
    long RAW_val = 0;
    int mV_DIV = 0;
    int mV_REAL = 0;
    int CURRENT_CYCLE = 0;
    uint16_t MEASURE_DELAY_TIMER;

    // Start the timer
    MEASURE_DELAY_TIMER = timer_read();

    while (CURRENT_CYCLE < MEASURE_CYCLES) {
        if (timer_elapsed(MEASURE_DELAY_TIMER) > MEASURE_DELAY) {
            RAW_val +=  adc_read(TO_MUX(ADC_CHANNEL_VMCU, 0));
            CURRENT_CYCLE++;
            MEASURE_DELAY_TIMER = timer_read();
        };
    };
    
    // Start the calculation
    mV_DIV = RAW_val / ADC_RES / MEASURE_CYCLES;
    mV_REAL = mV_DIV / VMCU_VDF;

    // Output to debug console
    dprintf("VMCU_RAW: %lu\n", RAW_val);
    dprintf("VMCU_DIV: %u mV\n", mV_DIV);
    dprintf("VMCU: %u mV\n", mV_REAL);

    // Output to string
    sprintf(VMCU_String, "%u mV", (int)mV_REAL);

    // Output the calculation
    return mV_REAL;
}

int tmcu_measure(void) {
    // Define values to get destroyed after the function ends
    long RAW_val = 0;
    int32_t Temp_mC = 0;
    int CURRENT_CYCLE = 0;
    uint16_t MEASURE_DELAY_TIMER;

    // Start the timer
    MEASURE_DELAY_TIMER = timer_read();

    while (CURRENT_CYCLE < MEASURE_CYCLES) {
        if (timer_elapsed(MEASURE_DELAY_TIMER) > MEASURE_DELAY) {
            RAW_val +=  adc_read(TO_MUX(ADC_CHANNEL_TSENSOR, 0));   // Read the analog voltage and sum it up.
            CURRENT_CYCLE++;                                        // Advance the counter
            MEASURE_DELAY_TIMER = timer_read();                     // Restart the timer
        };
    };
    
    // Start the calculation
    Temp_mC = RAW_val / MEASURE_CYCLES;
    Temp_mC = (((int32_t) Temp_mC * VMCU / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
    Temp_mC = Temp_mC * (int32_t)(110000 - 30000);
    Temp_mC = Temp_mC / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
    Temp_mC = Temp_mC + 30000;

    // Output to debug console
    dprintf("TMCU_RAW: %lu\n", RAW_val);
    dprintf("TMCU: %lu mC\n", Temp_mC);

    // Output to string
    sprintf(TMCU_String, "%u mC", (int)Temp_mC);

    // Output the calculation
    return Temp_mC;
}

void check_measure_timers(void){
    if (timer_elapsed(VBAT_TIMER) > MEASURE_CYCLE_TIME_VBAT) {
        dprint("VBAT_TIMER elapsed!\n");
        VBAT = vbat_measure();
        VBAT_TIMER = timer_read();
    };
    if (timer_elapsed(VRPI_TIMER) > MEASURE_CYCLE_TIME_VRPI) {
        dprint("VRPI_TIMER elapsed!\n");
        VRPI = vrpi_measure();
        VRPI_TIMER = timer_read();
    };
    if (timer_elapsed(VMCU_TIMER) > MEASURE_CYCLE_TIME_VMCU) {
        dprint("TMCU_TIMER elapsed!\n");
        VMCU = vmcu_measure();
        VMCU_TIMER = timer_read();
    };
    if (timer_elapsed(TMCU_TIMER) > MEASURE_CYCLE_TIME_TMCU) {
        dprint("TMCU_TIMER elapsed!\n");
        TMCU = tmcu_measure();
        TMCU_TIMER = timer_read();
    };
}