

/**
 * main.c
 */

#include <stdio.h>
#include <math.h>
#include "timer.h"
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include "open_interface.h"

char singleNumChar(int num);
void numToChar(int num);
int IrCalibrated(int sensorData);

volatile int numVals = 10;

// make sure the distance and sensorVals are linear
volatile int distance[10]  = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
volatile int sensorVals[10] = {3900, 2655, 1800, 1380, 1155, 990, 890, 750, 620, 500};

void main()
{

    timer_init();
    lcd_init();
    adc_init();
    uart_init();
    uart_interrupt_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    int value = 0;

    int irVal = 0;

    uart_sendStr("distance");

    while (1){

        if(adc_read() > 0){

            irVal = adc_read();

            value = IrCalibrated(irVal);

            lcd_printf("value: %d", value);

            //numToChar(value);
            //uart_sendStr("\n\r");
            timer_waitMillis(400);

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
            oi_free(sensor_data);

            return;
        }

        key_pressed = 0;

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

int IrCalibrated(int sensorData){

    int i = 0;

    for(i = 0; i < numVals -1; i++){

        if (sensorData <= sensorVals[i] && sensorData >= sensorVals[i+1]){

            float ratio = (float)(sensorData - sensorVals[i+1]) / (sensorVals[i] - sensorVals[i+1]);
            return (ratio * (distance[i] - distance[i+1])) + distance[i+1];
        }

    }

    return -1;
}
