#include <inc/tm4c123gh6pm.h>
#include "open_interface.h"
#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "movement.h"
#include "Timer.h"
#include "lcd.h"
#include <math.h>
#include <stdio.h>

/**
 * main.c
 */

int main(void)
{
    cyBOT_init_Scan(0b0011);
    cyBot_uart_init();
    timer_init();
    lcd_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);


    /**
    cyBOT_SERVRO_cal_t cal = cyBOT_SERVO_cal();

    left_calibration_value  = cal.left;
    right_calibration_value = cal.right;

    **/


    left_calibration_value  = 1251250;
    right_calibration_value = 274750;


    typedef struct{
        int degree;
        int distance;
        int width;
    }Object;

    Object objList[20] = {0, 0, 0};


    cyBOT_Scan_t Scan;
    Scan.IR_raw_val = -1;
    Scan.sound_dist = 0.0;


    char savedData[36];
    char startTxt[40] = "Angle(Degrees)        Distance(cm)\n\r";


    cyBOT_Scan(0, &Scan);
    float currentDist = 0.0;

    int dist       = Scan.sound_dist;
    int diff       = 0;
    int difference = 0;
    int numObj     = 0;
    int leniency   = 6;
    int allowed    = 6;
    int cleanDist  = 0;
    float width    = 0;

    int listOfClean[91];

    int i = 0;
    int j = 0;
    int z = 0;

    for(i = 0; i < strlen(startTxt); i++){
        cyBot_sendByte(startTxt[i]);
    }

    int byte = 0;

    for (i = 0; i <= 90; i ++){

        cyBOT_Scan(i*2, &Scan);
        currentDist = Scan.sound_dist;

        cleanDist = currentDist;

        listOfClean[i] = cleanDist;
        lcd_printf("deg: %d\ndist(cm): %d", i*2, listOfClean[i]);

    }

    for (i = 0; i <= 90; i++){
        /**
        if (i+3 <= 90 && listOfClean[i] == listOfClean[i+3]){
            listOfClean[i+1] = listOfClean[i];
            listOfClean[i+2] = listOfClean[i];
        }
        **/

        diff = listOfClean[z] - listOfClean[i];

        if (diff < 0){
            diff *= -1;
        }

        if (diff <= allowed){

            if (diff > leniency){
                listOfClean[i] = listOfClean[z];
            }

        }else{
            i = z;
            z++;
        }

    }


    for (i = 0; i <= 90; i ++){

        sprintf(savedData, "%d              %d\n\r", i*2,  listOfClean[i]);

        for(j = 0; j < strlen(savedData); j++){
            cyBot_sendByte(savedData[j]);
        }

        if(i < 90){
            difference = listOfClean[i] - listOfClean[i+1];
        }

        if (difference < 0){
            difference *= -1;
        }

        if (difference < leniency){

            width += 1;

            if (listOfClean[i] < dist){
                dist = listOfClean[i];
            }

        } else if (difference > leniency & width/4 > 3) {

            objList[numObj].distance = dist;
            objList[numObj].width    = width/4;
            objList[numObj].degree   = i*2 - objList[numObj].width*3;
            numObj ++;

            width = 0;

        } else {
            width = 0;
        }
    }


    if(numObj > 0){

        Object smallest = objList[0];
        char toStart[37] = "Object#  Angle   Distance   Width\n\r";
        char toPrint[120];

        for(i = 0; i < strlen(toStart); i++){
                cyBot_sendByte(toStart[i]);
        }

        for (i = 0; i < numObj; i ++){

            sprintf(toPrint,"%d       %d      %d         %d \n\r", i, objList[i].degree, objList[i].distance, objList[i].width);

            for(j = 0; j < strlen(toPrint); j++){
                cyBot_sendByte(toPrint[j]);
            }

            if (objList[i].width < smallest.width){
                smallest = objList[i];
            }
        }

        cyBOT_Scan(smallest.degree, &Scan);
        lcd_printf("the smallest degree: %d", smallest.degree);
    }

    while (1){

        byte = cyBot_getByte();
        oi_update(sensor_data);

        if (byte == 'w'){
            oi_setWheels(200, 200);

        } else if (byte == 's'){
            oi_setWheels(-200, -200);

        } else if (byte == 'a'){
            oi_setWheels(100, -100);

        } else if (byte == 'd'){
            oi_setWheels(-100, 100);

        } else {
            oi_setWheels(0, 0);
        }
    }

    oi_setWheels(0, 0); // stop
    oi_free(sensor_data);

	return 0;
}
