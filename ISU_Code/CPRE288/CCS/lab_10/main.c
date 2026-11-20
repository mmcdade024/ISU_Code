
/**
 * main.c
 */

#include <inc/tm4c123gh6pm.h>
#include <stdio.h>
#include <stdlib.h>
#include "open_interface.h"
#include "button.h"
#include "Timer.h"
#include "servo.h"
#include "ping.h"
#include "uart.h"
#include "lcd.h"

char singleNumChar(int num);
void numToChar(int num);

void main(void)
{

    timer_init();
    ping_init();
    lcd_init();
    button_init();
    init_button_interrupts();
    servo_init();
    uart_init();
    //uart_interrupt_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);


    extern volatile int button_event;
    extern volatile int button_num;
    extern volatile int clockwise;
    extern volatile int degree;


    while(1){

        float dist = ping_getDistance();
        button_num = button_getButton();
        int temp = button_Handler(button_num);

        timer_waitMillis(50);
        lcd_printf("%d \n%d, dist: %.2f", abs(180 - temp), clockwise, dist);


        if(key_pressed == 'm'){

            int i = 0;

            for (i = 0; i <= 90; i ++){

                degree = i*2;
                servo_move(degree);

                //uart_sendStr("Angle: ");
                numToChar(i*2);

                uart_sendStr("    ");

                //uart_sendStr(" PING: ");
                numToChar(ping_getDistance());

                uart_sendStr("\n\r");

             }
             uart_sendStr("\n");

        }

        if (key_pressed == 'w'){
            oi_setWheels(200, 200);

        }else if (key_pressed == 's'){
            oi_setWheels(-200, -200);

        }else if (key_pressed == 'a'){
            oi_setWheels(100, -100);

        }else if (key_pressed == 'd'){
            oi_setWheels(-100, 100);

        } else {
            oi_setWheels(0, 0);
        }

        if (key_pressed == 'q'){

            oi_setWheels(0, 0);

            return;
        }

        key_pressed = uart_receive();

    }

}

char singleNumChar(int num){

    switch(num){
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        default:
            return '0';
    }

}

void numToChar(int num){

    if (num < 10){ uart_sendChar(singleNumChar(num)); return;}

    int save = num;
    int toSend = 0;
    int numOfDig = 0;

    int i = 0;

    while(save > 0){

       save /= 10;
       numOfDig ++;

    }

    save = num;

    for (i = numOfDig-1; i >= 0; i--){

       toSend = save / pow(10, i);
       save -= toSend * pow(10, i);

       uart_sendChar(singleNumChar(toSend));

    }

}

