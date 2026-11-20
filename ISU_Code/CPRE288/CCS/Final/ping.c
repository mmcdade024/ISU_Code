/*
 * ping.c
 *
 * Created on: Nov 5, 2024
 *      Author: mamcdade
 */

#include "ping.h"
#include "Timer.h"
#include "lcd.h"

// Global variables to store timing information and state
volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum { LOW, HIGH, DONE } STATE = LOW;

int count = 0; // Counter for debugging
bool intflag = true;

void ping_init(void) {
    /*
     * Initializes the hardware for ping sensor operation.
     *
     * 1. Enables clock for Port B.
     * 2. Enables clock for Timer 3.
     * 3. Configures PB4 as an alternate function for the ping sensor.
     * 4. Configures Timer 3 for periodic mode and interrupt generation.
     * 5. Enables the Timer 3B interrupt.
     */

    SYSCTL_RCGCGPIO_R |= 0x02;  // Enable clock for Port B
    timer_waitMicros(100);
    SYSCTL_RCGCTIMER_R |= 0x08;  // Enable clock for Timer 3
    timer_waitMicros(100);

    while ((SYSCTL_PRGPIO_R & 0x02) != 0x02) {}  // Wait for clock to stabilize

    GPIO_PORTB_DIR_R &= ~0x08;  // Set PB4 as input
    GPIO_PORTB_AFSEL_R |= 0x08; // Enable alternate function for PB4
    GPIO_PORTB_PCTL_R |= 0x00007000;  // Configure PB4 for Timer 3
    GPIO_PORTB_PCTL_R &= 0xFFFF7FFF;
    GPIO_PORTB_DEN_R |= 0x08;  // Enable digital function for PB4

    TIMER3_CTL_R &= ~0x100;  // Disable Timer 3B
    TIMER3_CFG_R = 0x4;  // Configure Timer 3B for periodic mode
    TIMER3_TBMR_R = 0x07;  // Configure Timer 3B for 32-bit timer mode
    TIMER3_CTL_R |= (0x0C00);  // Enable Timer 3B and start it
    TIMER3_TBPR_R = 0xFF;  // Set the prescaler value
    TIMER3_TBILR_R = 0xFFFF;  // Set the interval load value
    TIMER3_ICR_R |= 0x400;  // Clear the Timer 3B interrupt flag
    TIMER3_IMR_R |= 0x400;  // Enable Timer 3B interrupt
    NVIC_EN1_R = 0x00000010;  // Enable interrupt for Timer 3B
    NVIC_PRI9_R = 0x8;  // Set interrupt priority for Timer 3B

    IntRegister(INT_TIMER3B, TIMER3B_Handler);  // Register the interrupt handler
    IntMasterEnable();  // Enable global interrupts

    TIMER3_CTL_R |= 0x100;  // Start Timer 3B
}

void ping_trigger(void) {
    /*
     * Triggers the ping sensor by sending a short pulse.
     *
     * 1. Disables Timer 3B interrupts.
     * 2. Configures PB4 as output.
     * 3. Sends a short pulse to the ping sensor.
     * 4. Configures PB4 back as input.
     * 5. Enables Timer 3B interrupts and starts the timer.
     */

    STATE = LOW;

    TIMER3_CTL_R &= ~0x100;  // Disable Timer 3B
    TIMER3_IMR_R = ~0x400;  // Disable Timer 3B interrupt
    GPIO_PORTB_AFSEL_R &= ~0x08;  // Disable alternate function for PB4
    GPIO_PORTB_DIR_R |= 0x08;  // Set PB4 as output
    timer_waitMicros(100);

    GPIO_PORTB_DATA_R &= ~0x08;  // Set PB4 low
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R |= 0x08;  // Set PB4 high
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x08;  // Set PB4 low
    timer_waitMicros(100);

    GPIO_PORTB_DIR_R &= ~0x08;  // Set PB4 as input
    timer_waitMicros(100);

    TIMER3_ICR_R |= 0x400;  // Clear the Timer 3B interrupt flag
    GPIO_PORTB_AFSEL_R |= 0x08;  // Enable alternate function for PB4
    TIMER3_IMR_R |= 0x400;  // Enable Timer 3B interrupt
    TIMER3_CTL_R |= 0x100;  // Start Timer 3B
    timer_waitMicros(100);
}

void TIMER3B_Handler(void) {
    /*
     * Interrupt handler for Timer 3B.
     *
     * Records the start and end times of the ping pulse.
     */

    if (TIMER3_MIS_R & 0x400) {
        TIMER3_ICR_R |= 0x400;  // Clear the Timer 3B interrupt flag

        if (STATE == LOW) {
            START_TIME = TIMER3_TBR_R;  // Record the start time
            STATE = HIGH;
        } else if (STATE == HIGH) {
            END_TIME = TIMER3_TBR_R;  // Record the end time
            STATE = DONE;
        }
    }
}

float ping_getDistance(void) {
    /*
     * Triggers the ping sensor, waits for the echo pulse, and calculates the distance.
     *
     * Returns:
     *  The calculated distance in centimeters.
     */

    ping_trigger();

    while (STATE != DONE) {}  // Wait for the echo pulse

    float timeDif = 0;
    int overflow = (END_TIME > START_TIME);

    // Calculate the time difference between the start and end times, accounting for potential overflows
    if (START_TIME > END_TIME) {
        timeDif = START_TIME - END_TIME;
    } else {
        timeDif = ((unsigned long)overflow << 24) + START_TIME - END_TIME;
    }

    // Convert the time difference to distance in centimeters
    timeDif /= 16000000;  // Convert to seconds
    timeDif = (timeDif / 2) * 343 * 100;  // Calculate distance using speed of sound

    return timeDif;
}