/*
 * scan.c
 *
 *  Created on: Dec 3, 2024
 *      Author: mamcdade
 */

#include <inc/tm4c123gh6pm.h>
#include "open_interface.h"
#include "color_sensor.h"
#include "movement.h"
#include <stdio.h>
#include <math.h>
#include "music.h"
#include "timer.h"
#include "servo.h"
#include "uart.h"
#include "ping.h"
#include "lcd.h"
#include "adc.h"


void getObjs();
void cleanUp();
void sendIrData();
void servo_move();
void printIrData();
void getCleanPingData();
void getCleanIRDataV2();
void getSmallestObj();
void numToChar(int num);

typedef struct{
    int degree;
    int distance;
    int width;
}Object;


volatile Object objList[20] = {0, 0, 0};
volatile Object smallest;
volatile int minWidth = 2; // DONT CHANGE
volatile int closest = 2000; // closest point to CyBot

volatile int IrData[91]; // Holds Ir Values
volatile int PingData[91];
volatile int numObj = 0; // number of objects
volatile int deg = 0; // degree that the sensor is pointed to
volatile int min = 3; // minium width an obj can be
volatile int i = 0;
volatile int j = 0;


void getIRData() {
  /*
   * Reads IR sensor data for all angles from 0 to 180 degrees.

   * 1. Iterates over 91 angles (0 to 180 degrees in 2-degree increments).
   * 2. Reads the IR sensor value using adc_read() and scales it by 10.
   * 3. Reads the distance using the ping sensor and stores it in PingData.
   * 4. Updates the closest variable if the current IR reading is closer than the previous minimum.
   * 5. Calculates the current angle in degrees.
   * 6. Moves the servo to the calculated angle using servo_move().
   * 7. Waits for 250 milliseconds for the sensor to get better values.
   */

  for (i = 0; i <= 90; i++) {
    IrData[i] = adc_read() / 10;
    PingData[i] = ping_getDistance();

    if (closest > IrData[i]) {
      closest = IrData[i];
    }

    deg = i * 2;
    servo_move(deg);
    timer_waitMillis(250);
  }
}

void getCleanIRDataV2() {
  /*
   * Cleans and filters the IR sensor data.

   * 1. Gets raw IR sensor data using getIRData().
   * 2. Initializes a change flag to track significant changes in IR readings.
   * 3. Initializes a saveVal variable to store previous significant IR values.
   * 4. Sets the first element of IrData to the closest object distance.
   * 5. Iterates through the IR data:
   *    - If the difference between consecutive readings is significant, toggles the change flag and stores the current value in saveVal.
   *    - Otherwise, sets the current value to the closest object distance.
   *    - If the change flag is set, replaces the current value with the stored saveVal.
   * 6. Calls cleanUp() to further refine the data.
   * 7. Sends the cleaned IR data to the UART using sendIrData().
   */

  getIRData();

  int change = 0;
  int saveVal = 0;

  IrData[0] = closest;
  i = 0;

  while (i <= 90) {
    if (abs(IrData[i] - IrData[i++]) >= closest) {
      change = !change;
      saveVal = IrData[i];
    } else {
      IrData[i] = closest;
    }

    if (change) {
      IrData[i] = saveVal;
    }
  }

  cleanUp();
  sendIrData();
}

void getCleanIRData() {
  /*
   * Cleans and filters the IR sensor data.

   * 1. Gets raw IR sensor data using getIRData().
   * 2. Initializes a counter add to track the number of consecutive identical readings.
   * 3. Iterates through the IR data:
   *    - If the difference between consecutive readings is significant (greater than `closest + 50`), it's considered a potential object boundary.
   *    - If a potential boundary is found:
   *      - Initializes a counter j to track the end of the object.
   *      - While the difference between consecutive readings is within a certain threshold and the index is within bounds, it continues to mark the readings as part of the same object by setting them to the value of the first reading.
   *      - Updates the i index to the end of the object.
   *      - Sets the next reading to the closest object distance.
   *    - If no significant difference is found, the current reading is set to the closest object distance.
   * 4. Sets the first and last elements of IrData to the closest object distance to handle edge cases.
   * 5. Calls cleanUp() to further refine the data.
   * 6. Sends the cleaned IR data to the UART using sendIrData().
   */

  int add = 0;
  getIRData();

  i = 0;

  while (i <= 90) {
    if (abs(IrData[i++] - IrData[i]) > closest + 50) {
      j = i;
      add = 0;

      while (j < 91 && abs(IrData[j] - IrData[j + minWidth]) <= 200 + closest) {
        IrData[++j] = IrData[i];
        add = 1;
      }

      i = j + add;
      IrData[i] = closest;
    } else {
      IrData[i] = closest;
    }
  }

  IrData[0] = closest;
  IrData[90] = closest;
  cleanUp();
  sendIrData();
}

void getCleanPingData() {
  /*
   * Cleans and filters the ping sensor data.
   * 
   * This function works similarly to getCleanIRData(), but uses the PingData array instead of IrData.
   */

  int add = 0;
  getIRData();  // Note: This function also populates PingData

  i = 0;

  while (i <= 90) {
    if (abs(PingData[i++] - PingData[i]) > closest + 50) {
      j = i;
      add = 0;

      while (j < 91 && abs(PingData[j] - PingData[j + minWidth]) >= closest) {
        PingData[++j] = PingData[i];
        add = 1;
      }

      i = j + add;
      PingData[i] = closest;
    } else {
      PingData[i] = closest;
    }
  }

  PingData[0] = closest;
  PingData[90] = closest;
  cleanUp();
  sendIrData();
}

void cleanUp() {
  /*
   * Cleans the IR data by filling in gaps between objects.

   * 1. Iterates through the IR data.
   * 2. For each point, checks a window of min points ahead.
   * 3. If the current point and the point min points ahead are the same, but the points in between are different, it fills the gap with the current value.
   * 4. This helps to smooth out small variations in the data and identify more accurate object boundaries.
   */

  i = 0;
  j = 0;

  while (i < 90) {
    j = i;
    while (min + i > j && j < 91) {
      ++j;
      if (IrData[i] != IrData[j] && IrData[i] == IrData[i + min + 1]) {
        IrData[j] = IrData[i];
      }
    }
    i++;
  }
}

void getObjs() {
  /*
   * Detects objects in the IR data and stores information about them in the objList.

   * 1. Initializes a width counter to track the width of a potential object.
   * 2. Iterates through the IR data:
   *    - If the current point is above the closest distance threshold, it increments the width counter.
   *    - If the width reaches the minimum threshold, it's considered a potential object:
   *      - The object's degree, distance, and width are calculated and stored in the objList.
   *      - The numObj counter is incremented to keep track of the number of detected objects.
   *      - The width counter is reset.
   */

  int width = 0;
  i = 0;

  while (i <= 90) {
    if (i < 90 && IrData[i] > closest) {
      width++;
    } else if (width >= min) {
      objList[numObj].degree = i * 2 - width;
      objList[numObj].distance = IrData[i * 2 - width];
      objList[numObj].width = width;

      numObj++;
      width = 0;
    }

    i++;
  }
}

void getSmallestObj() {
  /*
   * Finds the smallest object detected based on width.

   * 1. Calls getObjs() to populate the objList with detected objects.
   * 2. Iterates through the objList:
   *    - Prints information about each object (degree, distance, and width) using uart_sendStr.
   *    - Compares the current object's width with the smallest object's width.
   *    - If the current object is smaller, updates the smallest variable to store its information.
   * 3. Sets the servo position to the degree of the smallest object using servo_move.
   * 4. Prints the degree of the smallest object using uart_sendStr.
   */

  getObjs();

  for (i = 0; i < numObj; i++) {
    numToChar(i);
    uart_sendStr(" Object ");
    numToChar(i);
    uart_sendStr(": ");
    uart_sendStr(" Degree: ");
    numToChar(objList[i].degree);
    uart_sendStr(" Distance: ");
    numToChar(objList[i].distance);
    uart_sendStr(" Width: ");
    numToChar(objList[i].width);
    uart_sendStr("\n\r");

    if (objList[i].width < smallest.width) {
      smallest = objList[i];
    }
  }

  deg = smallest.degree;
  servo_move(deg);
  uart_sendStr(" The smallest degree is: ");
  numToChar(smallest.degree);
  uart_sendStr("\n\r");
}

void sendIrData() {
  /*
   * Sends the IR sensor data to the UART for debugging or visualization.
   *
   * 1. Iterates through the IrData array.
   * 2. Converts the index (i) to degrees (multiply by 2) and sends it using uart_sendStr.
   * 3. Sends the corresponding IR sensor reading from IrData using uart_sendStr.
   * 4. Adds a newline character (\n\r) after each data point.
   * 5. Sends an additional newline character (\n) at the end.
   */

  i = 0;
  while (i < 91) {
    numToChar(i * 2);
    uart_sendStr("  ");
    numToChar(IrData[i++]);
    uart_sendStr("\n\r");
  }
  uart_sendStr("\n");
}

void printIrData() {
  /*
   * Prints the IR sensor data to the standard output for debugging or visualization.

   * 1. Iterates through the IrData array.
   * 2. Prints the index (i) multiplied by 2 (degrees) and the corresponding IR sensor reading from IrData using printf.
   * 3. Adds a newline character (\n\r) after each data point.
   */

  i = 0;
  while (i < 91) {
    printf("%d  %d\n\r", i * 2, IrData[i++]);
  }
}

void printIrData() {
  /*
   * Prints the IR sensor data to the standard output for debugging or visualization.

   * 1. Iterates through the IrData array.
   * 2. Prints the index (i) multiplied by 2 (degrees) and the corresponding IR sensor reading from IrData using printf.
   * 3. Adds a newline character (\n\r) after each data point.
   */

  i = 0;
  while (i < 91) {
    printf("%d  %d\n\r", i * 2, IrData[i++]);
  }
}

char singleNumChar(int num) {
  /*
   * Converts a single digit integer to its corresponding character representation.

   * 1. Takes an integer num as input.
   * 2. Uses a switch statement to map the integer to its corresponding character.
   * 3. Returns the character representation of the input integer.
   */

  switch (num) {
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

void numToChar(int num) {
  /*
   * Converts an integer to a sequence of characters representing its digits and sends them to the UART.

   * 1. Handles single-digit numbers by directly sending the corresponding character using singleNumChar.
   * 2. For multi-digit numbers:
   *    - Calculates the number of digits using a loop and division by 10.
   *    - Iterates over each digit, extracting it using division and modulo operations.
   *    - Converts the extracted digit to a character using singleNumChar and sends it to the UART.
   */

  if (num < 10) {
    uart_sendChar(singleNumChar(num));
    return;
  }

  int save = num;
  int toSend = 0;
  int numOfDig = 0;

  int i = 0;

  while (save > 0) {
    save /= 10;
    numOfDig++;
  }

  save = num;

  for (i = numOfDig - 1; i >= 0; i--) {
    toSend = save / pow(10, i);
    save -= toSend * pow(10, i);

    uart_sendChar(singleNumChar(toSend));
  }
}

void keyMove(char keypressed, oi_t *sensor_data) {
  /*
   * Handles keypresses and performs corresponding actions.

   * 1. Checks the pressed key:
   *    - m: Calls getCleanIRDataV2() to process IR sensor data and identify objects.
   *    - w: Moves the robot forward using move_sensor.
   *    - s: Moves the robot backward using move_sensor.
   *    - a: Turns the robot left using turn_radius.
   *    - d: Turns the robot right using turn_radius.
   * 2. After each action, stops the robot's wheels using oi_setWheels(0, 0).
   * 3. Waits for 60 milliseconds using timer_waitMillis.
   * 4. Sends a "junk" message to the UART to prevent the GUI from blocking.
   */

  // CyBot 10

  if (keypressed == 'm') {
    getCleanIRDataV2();
    //getSmallestObj(); // Uncomment to activate object detection and movement
    //uart_sendStr("junk_M\n"); // Uncomment to prevent GUI from blocking
  } else if (keypressed == 'w') {
    move_sensor(sensor_data, 20);
    //uart_sendStr("junk_W\n"); // Uncomment to prevent GUI from blocking
  } else if (keypressed == 's') {
    move_sensor(sensor_data, -20);
    //uart_sendStr("junk_S\n"); // Uncomment to prevent GUI from blocking
  } else if (keypressed == 'a') {
    turn_radius(sensor_data, -10);
    //uart_sendStr("junk_A\n"); // Uncomment to prevent GUI from blocking
  } else if (keypressed == 'd') {
    turn_radius(sensor_data, 10);
    //uart_sendStr("junk_D\n"); // Uncomment to prevent GUI from blocking
  }

  oi_setWheels(0, 0);

  if (key_pressed == 'q') {
    oi_setWheels(0, 0);
    oi_free(sensor_data);
    return;
  }

  timer_waitMillis(60);
  uart_sendStr("junk\n");
}

