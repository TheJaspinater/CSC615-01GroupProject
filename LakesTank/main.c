/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name:Lake Jasper
* Student ID:920150605
* Github ID:TheJaspinater
* Project:  Assignment 3 - Motors & Motor Shield
*
* File: main.c
*
* Description:
**************************************************************/
#include "gpioController.h"
#include "sensors.h"
#include "softPwm.h"
#include "driveTrain.h"

int main() {
    initGpio();                             // Setup memory region to access GPIO

    setPin(STARTBUTTON, INPUT);             // set STARTBUTTON pin as INPUT
    printf("Waiting on button press...\n");
    while(readPin(STARTBUTTON)) {      // read STARTBUTTON until input is detected
    } sleep(1);                             // Do Nothing until button is pressed
    // - - - - - - - - - - - - - - - - - - - -
    int value = 0;
    for (;;) {
        value = lineScan();
        printf(" = %d\n", value);
        sleep(1);
    }

    return 0;
}