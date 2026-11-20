/*
 * adc.c
 *
 *  Created on: Oct 29, 2024
 *      Author: mamcdade
 */

#include "adc.h"
#include "timer.h"

void adc_init(void){

    SYSCTL_RCGCGPIO_R |= 0b011010;
    SYSCTL_RCGCADC_R  |= 0b000001;
    timer_waitMillis(500);

    ADC0_PC_R = 0x01;
    ADC0_SSPRI_R = 0x0123;
    ADC0_ACTSS_R &= ~0x0001;

    ADC0_EMUX_R  &= ~0x000F;
    ADC0_SSMUX0_R = 0b1010;
    ADC0_SSCTL0_R = 0x0006;
    ADC0_ACTSS_R |= 0x0001;

    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;

    GPIO_PORTD_DIR_R |= 0x3C;
    GPIO_PORTD_DEN_R |= 0x3C;

    GPIO_PORTE_DIR_R |= 0x0F;
    GPIO_PORTE_DEN_R |= 0x0F;

}

uint32_t adc_read(void) {

    ADC0_PSSI_R = 0x0001;

    while ((ADC0_RIS_R & 0x01) == 0);

    uint32_t result = ADC0_SSFIFO0_R & 0xFFF;
    ADC0_ISC_R = 0x0001;

    return result;

}
