/*
 * button.c
 *
 *  Created on: Nov 8, 2024
 *      Author: mamcdade
 */

#include "button.h"
#include "lcd.h"

volatile int button_event;
volatile int button_num;

/**
 * Initialize PORTE and configure bits 0-3 to be used as inputs for the buttons.
 */
void button_init() {
    static uint8_t initialized = 0;

    //Check if already initialized
    if(initialized){
        return;
    }

    SYSCTL_RCGCGPIO_R |= 0b010000;   // enables register E's clock
    GPIO_PORTE_DEN_R  |= 0b00001111; // enables pins 0-3
    GPIO_PORTE_DIR_R  &= 0b00001111;  // sets pins 0-3 as inputs

    initialized = 1;
}

void init_button_interrupts() {

    GPIO_PORTE_IM_R &= 0b11110000;

    GPIO_PORTE_IS_R &= 0xF0; // Set pins 0-3 to use edge sensing

    GPIO_PORTE_IBE_R |=0x0F; // Set pins 0-3 to use both edges.

    GPIO_PORTE_ICR_R = 0xFF; // Clear the interrupt s

    GPIO_PORTE_IM_R |= 0x0F; // Unmask the bits for pins 0-3

    NVIC_EN0_R |= 0b10000; // Enable GPIO port E interrupt

    IntRegister(INT_GPIOE, gpioe_handler);
}

void gpioe_handler() {

    GPIO_PORTE_ICR_R = 0xFF;
    button_event = 1;
    button_num = button_getButton();
    //timer_waitMillis(20);
}




/**
 * Returns the position of the rightmost button being pushed.
 * @return the position of the rightmost button being pushed. 4 is the rightmost button, 1 is the leftmost button.  0 indicates no button being pressed
 */
uint8_t button_getButton() {

    //
    // DELETE ME - How bitmasking works
    // ----------------------------------------
    // In embedded programming, often we only care about one or a few bits in a piece of
    // data.  There are several bitwise operators that we can apply to data in order
    // to "mask" the bits that we don't care about.
    //
    //  | = bitwise OR      & = bitwise AND     ^ = bitwise XOR     ~ = bitwise NOT
    //        << x = shift left by x bits        >> x = shift right by x bits
    //
    // Let's say we want to know if push button 3 (S3) of GPIO_PORTE_DATA_R is
    // pushed.  Since push buttons are high (1) initially, and low (0) if pushed, PORTE should
    // look like:
    // GPIO_PORTE_DATA_R => 0b???? ?0?? if S3 is pushed
    // GPIO_PORTE_DATA_R => 0b???? ?1?? if S3 is not pushed
    //
    // This is not useful: There are 128 different 8 bit numbers that have the 3rd bit high or low.
    // We can make it more clear if we mask the other 7 bits:
    //
    // Bitwise AND:
    // (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0000 if S3 is pushed
    // (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0100 if S3 is not pushed
    //
    // Bitwise OR:
    // (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1011 if S3 is pushed
    // (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1111 if S3 is not pushed
    //
    // Other techniques (Shifting and bitwise AND)
    // ((GPIO_PORTE_DATA_R >> 2) & 1) => 0 if S3 is pushed
    // ((GPIO_PORTE_DATA_R >> 2) & 1) => 1 if S3 is not pushed

    // TODO: Write code below -- Return the left must button position pressed

    // INSERT CODE HERE!

    char value = GPIO_PORTE_DATA_R ^ 0b1111;

    if (value >= 8){
        return 4;
    }
    if (value >= 4 && value <= 7){
        return 3;
    }
    if (value >= 2 && value <= 3){
        return 2;
    }
    if (value == 1){
        return 1;
    }

    return value;

}





