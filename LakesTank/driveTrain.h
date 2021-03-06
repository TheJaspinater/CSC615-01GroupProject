/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name:Lake Jasper
* Student ID:920150605
* Github ID:TheJaspinater
* Project:  Assignment 3 - Motors & Motor Shield
*
* File: driveTrain.c
*
* Description:
**************************************************************/
#ifndef DRIVERTRAIN_H
#define DRIVERTRAIN_H

#include "gpioController.h"
#include "softPwm.h"

#define FORWARD 0
#define REVERSE 1
#define ROTATELEFT 2
#define ROTATERIGHT 3
#define TURNLEFT 4
#define TURNRIGHT 5
#define STOP 6
#define CLOCKWISE 7

#define LEFTMOTOR   16
#define LEFTFORWARD 20
#define LEFTREVERSE 21

#define RIGHTMOTOR   13
#define RIGHTFORWARD 19
#define RIGHTREVERSE 26

//Functions
void initMotors();
void stopMotors();
void steer(int, int);

#endif