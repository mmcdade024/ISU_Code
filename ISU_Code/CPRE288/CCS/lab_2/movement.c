/*
 * movement.c
 *
 *  Created on: Sep 10, 2024
 *      Author: mamcdade
 */

#include <stdio.h>
#include "open_interface.h"
#include "cyBot_uart.h"
#include "movement.h"
#include "timer.h"
#include "lcd.h"

void move(oi_t *sensor, int centimeters){
    double sum = 0;

    if (centimeters < 0){
        oi_setWheels(-300, -300);
        centimeters *= -1;

    }else{
        oi_setWheels(300, 300);
    }

    while (sum < centimeters) {
        oi_update(sensor);

        if (sensor->distance < 0){
            sum += sensor->distance *-1;
        }else{
            sum += sensor->distance;
        }
    }

    oi_setWheels(0, 0);
}


void turn_radius(oi_t *sensor, double degrees)
{
    double angle = 0.0;

    if (degrees < 0){
        oi_setWheels(300, -300);
        degrees *= -1;
    }else{
        oi_setWheels(-300, 300);
    }

    while (angle < degrees){
       oi_update(sensor);

       if (sensor->angle < 0){
           angle += sensor->angle * -1;
       }else{
           angle += sensor->angle;
       }
    }

    oi_setWheels(0, 0);
}


void change_dir(oi_t *sensor){

    oi_setWheels(0, 0);
    if (sensor->bumpLeft){
        move(sensor, -150);
        turn_radius(sensor, 90);
        move(sensor, 250);
        turn_radius(sensor, -90);


    }
    else if(sensor->bumpRight){
        move(sensor, -150);
        turn_radius(sensor, -90);
        move(sensor, 250);
        turn_radius(sensor, 90);
    }
}

void move_sensor(oi_t *sensor, int centimeters){

    double sum = 0.0;
    int negative = 0;

    if (centimeters < 0){
        oi_setWheels(-300, -300);
        centimeters *= -1;
        negative = 1;

    }else{
        oi_setWheels(300, 300);
    }

    while (sum < centimeters){
        oi_update(sensor);

        if (sensor->bumpLeft || sensor->bumpRight){
            change_dir(sensor);

            if (negative){
                oi_setWheels(-300, -300);
            }else{
                oi_setWheels(300, 300);
            }
            centimeters += 160;

        }else if (sensor->distance < 0){
            sum += sensor->distance *-1;
        }else{
            sum += sensor->distance;
        }
    }
}
