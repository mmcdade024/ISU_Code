/*
 * button.h
 *
 *  Created on: Nov 8, 2024
 *      Author: mamcdade
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"


void button_init();

void init_button_interrupts();

void gpioe_handler();

uint8_t button_getButton();


#endif /* BUTTON_H_ */
