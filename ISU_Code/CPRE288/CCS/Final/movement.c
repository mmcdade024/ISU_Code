/*
 * movement.c
 *
 * Created on: Sep 10, 2024
 *      Author: mamcdade
 */

#include <stdio.h>
#include "open_interface.h"
#include "color_sensor.h"
#include "movement.h"
#include "timer.h"
#include "music.h"
#include "uart.h"
#include "lcd.h"

void move(oi_t *sensor, int centimeters) {
    /*
     * Moves the robot forward or backward a specified distance in centimeters.
     *
     * Parameters:
     *  sensor: A pointer to the OI sensor data structure.
     *  centimeters: The distance to move in centimeters (positive for forward, negative for backward).
     */

    double sum = 0;

    // Set the wheel speeds based on the desired direction
    if (centimeters < 0) {
        oi_setWheels(-300, -300);  // Backward movement
        centimeters *= -1;
    } else {
        oi_setWheels(300, 300);  // Forward movement
    }

    // Keep moving until the desired distance is reached
    while (sum < centimeters) {
        oi_update(sensor);

        // Accumulate the distance traveled based on the sensor reading
        if (sensor->distance < 0) {
            sum += sensor->distance * -1;
        } else {
            sum += sensor->distance;
        }
    }

    // Stop the robot
    oi_setWheels(0, 0);
}


void turn_radius(oi_t *sensor, double degrees) {
    /*
     * Turns the robot in place by a specified angle in degrees.
     *
     * Parameters:
     *  sensor: A pointer to the OI sensor data structure.
     *  degrees: The angle to turn in degrees (positive for right, negative for left).
     */

    double angle = 0.0;

    // Set the wheel speeds to turn in the desired direction
    if (degrees < 0) {
        oi_setWheels(300, -300);  // Turn left
        degrees *= -1;
    } else {
        oi_setWheels(-300, 300);  // Turn right
    }

    // Keep turning until the desired angle is reached
    while (angle < degrees) {
        oi_update(sensor);

        // Accumulate the angle turned based on the sensor reading
        if (sensor->angle < 0) {
            angle += sensor->angle * -1;
        } else {
            angle += sensor->angle;
        }
    }

    // Stop the robot
    oi_setWheels(0, 0);
}


void change_dir(oi_t *sensor) {
    /*
     * Changes the direction of the robot based on bump sensor readings.
     *
     * Parameters:
     *  sensor: A pointer to the OI sensor data structure.
     */

    oi_setWheels(0, 0);  // Stop the robot

    if (sensor->bumpLeft) {
        // Move backward, turn right, move forward, turn left
        move(sensor, -150);
        turn_radius(sensor, 90);
        move(sensor, 250);
        turn_radius(sensor, -90);
    } else if (sensor->bumpRight) {
        // Move backward, turn left, move forward, turn right
        move(sensor, -150);
        turn_radius(sensor, -90);
        move(sensor, 250);
        turn_radius(sensor, 90);
    }
}

void move_sensor(oi_t *sensor, int centimeters) {
    /*
     * Moves the robot forward or backward a specified distance, reacting to
     * color sensor readings, bump sensors, and distance sensor readings.
     *
     * Parameters:
     *  sensor: A pointer to the OI sensor data structure.
     *  centimeters: The distance to move in centimeters (positive for forward, negative for backward).
     */

    double sum = 0.0;

    // Set the wheel speeds based on the desired direction
    if (centimeters < 0) {
        oi_setWheels(-150, -150);  // Backward movement
        centimeters *= -1;
    } else {
        oi_setWheels(150, 150);  // Forward movement
    }

    while (sum < centimeters) {
        oi_update(sensor);

        // Get the current surface type
        getSurfType(sensor);

        // Handle different scenarios based on sensor readings
        if (strcmp(getSurfaceType(), "White") == 0) {
            // Stop and move backward slightly on a white surface
            uart_sendStr("white\n");
            oi_play_song(2);
            oi_setWheels(0, 0);
            oi_setWheels(-150, -150);
        } else if (strcmp(getSurfaceType(), "Black") == 0) {
            // Stop and show black detected message on a black surface
            uart_sendStr("black\n");
            oi_play_song(1);
            oi_setWheels(0, 0);
            oi_setWheels(-150, -150);
        } else if (sensor->bumpLeft) {
            // Handle left bump sensor trigger
            uart_sendStr("left\n");
            oi_setWheels(0, 0);
            oi_setWheels(-150, -150);
            oi_play_song(4);
        } else if (sensor->bumpRight) {
            // Handle right bump sensor trigger
            uart_sendStr("right\n");
            oi_setWheels(0, 0);
            oi_setWheels(-150, -150);
            oi_play_song(3);
        } else {
            // Accumulate the distance traveled based on the distance sensor reading
            if (sensor->distance < 0) {
                sum += sensor->distance * -1;
            } else {
                sum += sensor->distance;
            }
        }
    }
}