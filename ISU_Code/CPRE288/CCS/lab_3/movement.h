/*
 * movement.h
 *
 *  Created on: Sep 10, 2024
 *      Author: mamcdade
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"

// moves the iRobot a given number of centimeters
void move(oi_t *sensor, int centimeters);

// turns the iRobot a given number of degrees
void turn_radius(oi_t *sensor, double degrees);

//it changes the direction by moving backwards then to either the left or right
void change_dir(oi_t *sensor);

void move_sensor(oi_t *sensor, int centimeters);

#endif /* MOVEMENT_H_ */
