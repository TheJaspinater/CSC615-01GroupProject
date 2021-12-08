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

static volatile int lineState;
static volatile pthread_t scannerThread;
static volatile int threadCheck = -1;
static int threadController = 0;

/*****************************
 * multi threading
 ****************************/

static void * scanner ( void * args )
{
    threadCheck = -1;
    while ( threadController != 1 )
    {
        lineState = lineScanHelper   ( LEFT        )
                  + ( lineScanHelper ( LEFTCENTER  ) * 2 )
                  + ( lineScanHelper ( RIGHTCENTER ) * 4 )
                  + ( lineScanHelper ( RIGHT       ) * 8 );
    }

    return NULL ;
}

int getLineState ()
{
    return lineState;
}

int initLineScanner ()
{
    int res;
    pthread_t newThread ;
    
    threadCheck = 1;
    res = pthread_create (&newThread, NULL, scanner, NULL) ;
    
    if (res != 0)
        return res ;

    while ( threadCheck != -1 ) // thread not created, store scannerThread. threadcheck updated from within thread
    {
        usleep (1000) ;
    }

    scannerThread = newThread;

    return res;
}

void stopLineScanner ()
{
    threadController = 1;
    pthread_cancel ( scannerThread ) ;
    pthread_join   ( scannerThread, NULL ) ;
    lineState = 0;
}

/*****************************
 * Prototyped / proof of concept
 ****************************/

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
            return getDistanceCM();
        }
    }

    clock_t stop = clock();
    while (readPin(ECHO) == 1) {     // Update stop time until US board becomes inactive
        stop = clock();
    }
    double elapsedTime = (double)(stop - start) / CLOCKS_PER_SEC; 
    return (double)(34000 * elapsedTime) / 2; // Derived from 2*S = V*T
}