

/**
 * main.c
 */

#include <stdio.h>
#include "open_interface.h"
#include "cyBot_uart.h"
#include "cyBot_uart.h"
#include "movement.h"
#include "timer.h"
#include "lcd.h"

void main(){

    timer_init();
    lcd_init();
    cyBot_uart_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);



    //part 2
    /**
    int i = 0;

    for(i = 0; i < 4; i ++){
        move(sensor_data, 200);
        turn_radius(sensor_data, 90);
    }
    **/

    //part 3
    //move_sensor(sensor_data, 2000);


     while(1){

         int byte = cyBot_getByte();

         if (byte == 'w'){
             oi_setWheels(300, 300);
         }
         else if (byte == 'a'){
             turn_radius(sensor_data, -90);
             oi_setWheels(300, 300);
         }
         else if (byte == 's'){
             turn_radius(sensor_data, 180);
             oi_setWheels(300, 300);
         }
         else if (byte == 'd'){
             turn_radius(sensor_data, 90);
             oi_setWheels(300, 300);

         } else {
             lcd_putc(byte);
             oi_setWheels(0, 0);
         }
     }

    oi_setWheels(0, 0); // stop
    oi_free(sensor_data);

}
