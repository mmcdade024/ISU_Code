/**
 * lab6_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */

#define _RESET 0
#define _PART1 1

#include <math.h>
#include <stdio.h>
#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include "cyBot_Scan.h"
#include "open_interface.h"


char singleNumChar(int num);
void numToChar(int num);


#if _RESET
#include "Simulation_User Files_resetSimulation.h"
#endif

volatile  char uart_data;  // Your UART interupt code can place read data here
volatile  char flag;       // Your UART interupt can update this flag


void main(){
#if _RESET
    resetSimulationBoard();
#else

    timer_init();
    lcd_init();
    uart_init();

#if _PART1 // IR sensor and moving roomba


    cyBOT_init_Scan(0b0111);
    uart_interrupt_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    //cyBOT_SERVRO_cal_t cal = cyBOT_SERVO_cal();

    left_calibration_value  = 1204000;
    right_calibration_value = 238000;

    cyBOT_Scan_t Scan;

    int i = 0;

    typedef struct{
        int degree;
        int distance;
        int width;
    }Object;

    Object smallest;

    while(1){

        oi_update(sensor_data);

        if(key_pressed == 'm' || key_pressed == 'h'){

            Object objList[20] = {0, 0, 0};

            int PingData[91];
            int IrData[91];

            int closest = 1000;

            for (i = 0; i <= 90; i ++){

                cyBOT_Scan(i*2, &Scan);
                //uart_sendStr("Angle: ");
                //numToChar(i*2);

                //uart_sendStr("    ");

                //uart_sendStr(" PING: ");
                //numToChar(Scan.sound_dist);
                PingData[i] = Scan.sound_dist;

                //uart_sendStr("    ");

                //uart_sendStr(" IR: ");
                IrData[i] = Scan.IR_raw_val / 10;
                ///numToChar(IrData[i]);

                //uart_sendStr("\n\r");

                if ( closest > IrData[i] ){ closest = IrData[i]; }

             }

             int numObj = 0;
             int width  = 0;
             int widthLean = 3;
             int change    = 0;
             int saveVal   = 0;

             IrData[0] = closest;
             i = 0;

             // put the -10 because the value for the difference was to high was a guessed value
             while(i <= 90){

                 if (abs(IrData[i] - IrData[i ++]) >= closest){ change = !change; saveVal = IrData[i]; }
                 else { IrData[i] = closest; }

                 if(change){ IrData[i] = saveVal; }

             }

             //for testing to see what cleaned IR values are
             for (i = 0; i <= 90; i++){
                 numToChar(i*2);
                 uart_sendStr("    ");
                 numToChar(IrData[i]);
                 uart_sendStr("\n\r");
             }

             i = 0;

             while (i <= 90){

                 if(i < 90 && IrData[i] > closest){ width ++; }

                 else if(width >= widthLean){

                    objList[numObj].degree = i*2 - width;
                    objList[numObj].distance = PingData[i*2 - width];
                    objList[numObj].width = width;

                    numObj ++;
                    width = 0;

                 }

                 i ++;
             }

            if (numObj > 0) {

                smallest = objList[0];

                uart_sendStr("Object#   Angle   Distance   Width\n\r");

                for (i = 0; i < numObj; i++){

                    numToChar(i);
                    uart_sendStr("         ");
                    numToChar(objList[i].degree);
                    uart_sendStr("       ");
                    numToChar(objList[i].distance);
                    uart_sendStr("      ");
                    numToChar(objList[i].width);
                    uart_sendStr("         ");

                    uart_sendStr("\n\r");

                    if (objList[i].width < smallest.width){
                        smallest = objList[i];
                    }

                }

                cyBOT_Scan(smallest.degree, &Scan);
                uart_sendStr("the smallest degree is: ");
                numToChar(smallest.degree);
                uart_sendStr("\n\r");

            }

            if(key_pressed == 'h' && numObj > 0){


                uart_sendStr("I plan to rotate to: ");
                numToChar(smallest.degree);
                uart_sendStr("\n\r");

                uart_sendStr("I plan to move forward by: ");
                numToChar(smallest.distance -5);
                uart_sendStr("\n\r");

                key_pressed = 0;

                while(key_pressed == 0);

                if (key_pressed == 'h'){

                    oi_update(sensor_data);

                    int degrees  = 80 - smallest.degree; //try 2.3333337
                    double angle = 0.0;

                    if (degrees < 0){
                        oi_setWheels(50, -50);
                        degrees *= -1;
                    }else{
                        oi_setWheels(-50, 50);
                    }

                    while (angle < degrees){
                       oi_update(sensor_data);

                       if (sensor_data->angle < 0){
                           angle += sensor_data->angle * -1;
                       }else{
                           angle += sensor_data->angle;
                       }
                    }

                    oi_setWheels(0, 0);

                    while(Scan.sound_dist > 15 && !sensor_data->bumpLeft && !sensor_data->bumpRight){
                        oi_update(sensor_data);
                        cyBOT_Scan(90, &Scan);
                        oi_setWheels(200, 200);
                    }

                    oi_setWheels(0, 0);

                }

            }

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

        //uart_sendStr("junk\n");

    }


#endif



#endif
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

