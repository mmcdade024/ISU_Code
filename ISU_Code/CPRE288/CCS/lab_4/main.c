/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#define REPLACEME 0


int main(void) {

    cyBOT_init_Scan();
    cyBot_uart_init();
    button_init();
    lcd_init();

    int pressed = 0;
    int buttonValue;

    while(1)
    {
        buttonValue = button_getButton();
        char c = button_getButton();

        char value[2];
        sprintf(value,"%d", buttonValue);

        lcd_printf("-> %d", c);

        if (buttonValue != 0 && !pressed){
            cyBot_sendByte(value[0]);
            pressed = 1;

        } else if (buttonValue == 0) {
            pressed = 0;
        }
    }

}
