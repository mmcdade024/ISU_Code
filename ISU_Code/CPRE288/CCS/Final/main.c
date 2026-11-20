/**
 * main.c
 */

#include <inc/tm4c123gh6pm.h>
#include "cyBot_Scan.h"
#include "open_interface.h"
#include "color_sensor.h"
#include "Timer.h"
#include "servo.h"
#include "uart.h"
#include "ping.h"
#include "scan.h"
#include "lcd.h"
#include "adc.h"

void keyMove(char keyPressed, oi_t *sensor_data);
void load_songs();

void main(void){

    cyBOT_init_Scan(0b0111);
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    timer_init();
    //servo_init();
    //ping_init();
    //adc_init();
    lcd_init();
    uart_init();
    load_songs();

    left_calibration_value  = 1204000;
    right_calibration_value = 238000;

    uart_sendStr("junk_T\n");

    while(1){

        char keypressed = uart_receive();
        keyMove(keypressed, sensor_data);

    }

}
