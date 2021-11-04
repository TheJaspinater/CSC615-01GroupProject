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

    printf("[(%d) (%d) (%d) (%d)]\n", left, leftCenter, rightCenter, right);

    return left + (leftCenter * 2) + (rightCenter * 4) + (right * 8);
}

int lineScanHelper (int sensor) { // Looks for Black lines 
    if (readPin(sensor) == 0)
        return 1;
    else
        return 0;
}

void initServo(int servo) {
    softPwmCreate(servo, ANGLE90, 100);
}

void look(int servo, int angle) {
    softPwmWrite(servo, angle);
}

void stopServo(int servo) {
    softPwmWrite(servo, ANGLE90);
    softPwmStop(servo);
}

double averageDistCM(int measurements) {
    double weight = 0.7;
    double dist = 0;

    for (int i = 0; i < measurements; i++) {
        dist = (dist * weight) + (getDistanceCM() * (1 - weight)); // smooth results
        usleep(500); //wait for sound waves to complete trip. Consider lowering this value later. Fow now it is working though
    }

    return dist/measurements;
}

double getDistanceCM(){
    setPin(ECHO, INPUT);
    setPin(TRIGGER, INPUT); // Confirm trigger is low before going high to ensure a clean sginal
    setPin(TRIGGER, OUTPUT);
    usleep(10);                     // wait 10 microseconds           
    setPin(TRIGGER, INPUT);

    int count = 0;
    clock_t start = clock();
    while (readPin(ECHO) == 0) {     // Update start time until US board becomes active
        start = clock();
        count++;
        if (count > 100) {
            return getDistanceCM();
        }
    }

    clock_t stop = clock();
    while (readPin(ECHO) == 1) {     // Update stop time until US board becomes inactive
        stop = clock();
    }
    double elapsedTime = (double)(stop - start) / CLOCKS_PER_SEC; 
    return (double)(340000 * elapsedTime) / 2; // Derived from 2*S = V*T
}