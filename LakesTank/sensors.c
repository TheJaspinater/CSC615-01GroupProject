/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name:Lake Jasper
* Student ID:920150605
* Github ID:TheJaspinater
* Project:  Assignment 2 - Tapeless Ruler
*
* File: sensors.c
*
* Description:
**************************************************************/
#include "sensors.h"
#include "softPwm.h"

int lineScan() {

    int left = lineScanHelper(LEFT);
    int leftCenter = lineScanHelper(LEFTCENTER);
    int rightCenter = lineScanHelper(RIGHTCENTER);
    int right = lineScanHelper(RIGHT);

    printf("[(%d) (%d) (%d) (%d)]", left, leftCenter, rightCenter, right);

    return left + (leftCenter * 2) + (rightCenter * 4) + (right * 8);
}

int lineScanHelper (int sensor) {
    if (readPin(sensor) == 0)
        return 1;
    else
        return 0;
}

void initSServo() {
    softPwmCreate(SSERVO, 90, 100);
}

void look(int servo, int angle) {
    softPwmWrite(servo, angle);
}

void stopServo(int servo) {
    softPwmWrite(servo, ANGLE90);
    softPwmStop(servo);
}

double getDistanceCM(){
    setPin(ECHO, INPUT);
    setPin(TRIGGER, INPUT); // Confirm trigger is low before going high to ensure a clean sginal
    usleep(5);
    setPin(TRIGGER, OUTPUT);
    usleep(20);                     // wait 10 microseconds           
    setPin(TRIGGER, INPUT);

    int count = 0;
    clock_t start = clock();
    while (readPin(ECHO) == 0) {     // Update start time until US board becomes active
        start = clock();
        count++;
        if (count > 100) {
            return -1;
        }
    }

    clock_t stop = clock();
    while (readPin(ECHO) == 1) {     // Update stop time until US board becomes inactive
        stop = clock();
    }
    double elapsedTime = (double)(stop - start) / CLOCKS_PER_SEC; 
    return (double)(34000 * elapsedTime) / 2; // Derived from 2*S = V*T
}