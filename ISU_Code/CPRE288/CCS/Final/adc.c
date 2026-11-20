/*
 * adc.c
 *
 * Created on: Oct 29, 2024
 *      Author: mamcdade
 */

#include "adc.h"
#include "timer.h"

// Array to store pre-calibrated distance values
volatile int distance[10] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
// Array to store corresponding sensor values
volatile int sensorVals[10] = {3900, 2655, 1800, 1380, 1155, 990, 890, 750, 620, 500};

// Number of calibration points
volatile int numVals = 10;

// Initializes the ADC module
void adc_init(void) {
    // Enable clock for GPIO port B, D, and E
    SYSCTL_RCGCGPIO_R |= 0b011010;
    // Enable clock for ADC module 0
    SYSCTL_RCGCADC_R |= 0b000001;
    // Delay to allow clock to stabilize
    timer_waitMillis(500);

    // Configure ADC0
    // Enable ADC0
    ADC0_PC_R = 0x01;
    // Set priority for ADC0 sequence 0
    ADC0_SSPRI_R = 0x0123;
    // Disable sample sequence 0
    ADC0_ACTSS_R &= ~0x0001;

    // Configure sample sequence 0
    // Trigger source: processor
    ADC0_EMUX_R &= ~0x000F;
    // Input source: PB4 (ADC0_SSMUX0_R)
    ADC0_SSMUX0_R = 0b1010;
    // Sample and hold time, end of conversion flag
    ADC0_SSCTL0_R = 0x0006;
    // Enable sample sequence 0
    ADC0_ACTSS_R |= 0x0001;

    // Configure PB4 as analog input
    GPIO_PORTB_DIR_R &= ~0x10;  // Input direction
    GPIO_PORTB_DEN_R &= ~0x10;  // Digital enable
    GPIO_PORTB_AFSEL_R |= 0x10; // Alternate function select
    GPIO_PORTB_AMSEL_R |= 0x10; // Analog mode select

    // Configure PD0-PD2 as output (LEDs?)
    GPIO_PORTD_DIR_R |= 0x3C;
    GPIO_PORTD_DEN_R |= 0x3C;

    // Configure PE0-PE3 as output (LEDs?)
    GPIO_PORTE_DIR_R |= 0x0F;
    GPIO_PORTE_DEN_R |= 0x0F;
}

// Reads a value from the ADC
uint32_t adc_read(void) {
    // Initiate a conversion
    ADC0_PSSI_R = 0x0001;

    // Wait for conversion to complete
    while ((ADC0_RIS_R & 0x01) == 0);

    // Read the result and clear the interrupt flag
    uint32_t result = ADC0_SSFIFO0_R & 0xFFF;
    ADC0_ISC_R = 0x0001;

    return result;
}

// Calibrates the sensor reading using a lookup table
int IrCalibrated(int sensorData) {
    int i;

    // Iterate through the calibration points
    for (i = 0; i < numVals - 1; i++) {
        // Check if the sensor data falls within the current range
        if (sensorData <= sensorVals[i] && sensorData >= sensorVals[i + 1]) {
            // Calculate the ratio of the sensor data within the range
            float ratio = (float)(sensorData - sensorVals[i + 1]) / (sensorVals[i] - sensorVals[i + 1]);
            // Interpolate the distance based on the ratio
            return (ratio * (distance[i] - distance[i + 1])) + distance[i + 1];
        }
    }

    // If the sensor data is outside the calibration range, return -1
    return -1;
}