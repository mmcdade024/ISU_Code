
/**
 * main.c
 */

#include <inc/tm4c123gh6pm.h>
#include "ping.h"
#include "Timer.h"
#include "lcd.h"

void main(void) {

    timer_init();
    ping_init();
    lcd_init();

    while (1) {

        ping_getDistance();
        timer_waitMillis(500);

    }

}

