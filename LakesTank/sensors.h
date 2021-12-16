/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name:Lake Jasper
* Student ID:920150605
* Github ID:TheJaspinater
* Project:  Assignment 2 - Tapeless Ruler
*
* File: sensors.h
*
* Description:
**************************************************************/
#ifndef SENSORS_H
#define SENSORS_H

#include <time.h>
#include <pthread.h>
#include "gpioController.h"

// Sates
#define HIGH 1
#define LOW  0

// Buttons
#define STARTBUTTON 8

//Sonar
#define TRIGGER 1
#define ECHO    0

//Servos
#define SSERVO   23 //Sonar Servo
#define ANGLE0   5 
#define ANGLE90  13
#define ANGLE180 24
#define ANGLE45  9
#define ANGLE135 19

//4 Channel Line tracking
#define LEFT        3
#define LEFTCENTER  5
#define RIGHT       18
#define RIGHTCENTER 4

//RGP
#define REDLIGHT   22
#define GREENLIGHT 27
#define BLUELIGHT  24

// Functions
int    getLineState    ();
int    initLineScanner ();
void   stopLineScanner ();

double getDist         ();
int    initRangeFinder ();
void   stopRangeFinder ();

double getDistanceCM   ();
void   initServo       ( int );
void   look            ( int, int );
void   stopServo       ( int );
int    lineScan        ();
int    lineScanHelper  ( int );

#endif