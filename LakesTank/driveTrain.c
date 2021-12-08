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
#include "driveTrain.h"

void initMotors() {
    setPin(LEFTFORWARD, INPUT);
    setPin(LEFTREVERSE, INPUT);
    setPin(RIGHTFORWARD, INPUT);
    setPin(RIGHTREVERSE, INPUT);
    softPwmCreate(LEFTMOTOR, 0, 100);
    softPwmCreate(RIGHTMOTOR, 0, 100);
}

void stopMotors() {
    softPwmStop(LEFTMOTOR);
    softPwmStop(RIGHTMOTOR);
    setPin(LEFTFORWARD, INPUT);
    setPin(LEFTREVERSE, INPUT);
    setPin(RIGHTFORWARD, INPUT);
    setPin(RIGHTREVERSE, INPUT);
}

void steer(int direction, int speed) {
    switch (direction) {
        case FORWARD:
            setPin(LEFTREVERSE, INPUT);
            setPin(RIGHTREVERSE, INPUT);
            setPin(LEFTFORWARD, OUTPUT);
            setPin(RIGHTFORWARD, OUTPUT);
            softPwmWrite(LEFTMOTOR, speed);
            softPwmWrite(RIGHTMOTOR, speed);
            break;
        case REVERSE:
            setPin(LEFTFORWARD, INPUT);
            setPin(RIGHTFORWARD, INPUT);
            setPin(LEFTREVERSE, OUTPUT);
            setPin(RIGHTREVERSE, OUTPUT);
            softPwmWrite(LEFTMOTOR, speed);
            softPwmWrite(RIGHTMOTOR, speed);
            break;
        case ROTATELEFT:
            setPin(LEFTFORWARD, INPUT);
            setPin(RIGHTREVERSE, INPUT);
            setPin(RIGHTFORWARD, OUTPUT);
            setPin(LEFTREVERSE, OUTPUT);
            softPwmWrite(LEFTMOTOR, speed);
            softPwmWrite(RIGHTMOTOR, speed);
            break;
        case ROTATERIGHT:
            setPin(RIGHTFORWARD, INPUT);
            setPin(LEFTREVERSE, INPUT);
            setPin(LEFTFORWARD, OUTPUT);
            setPin(RIGHTREVERSE, OUTPUT);
            softPwmWrite(LEFTMOTOR, speed);
            softPwmWrite(RIGHTMOTOR, speed);
            break;
        case STOP:
            softPwmWrite(LEFTMOTOR, 0);
            softPwmWrite(RIGHTMOTOR, 0);
            setPin(LEFTFORWARD, INPUT);
            setPin(LEFTREVERSE, INPUT);
            setPin(RIGHTFORWARD, INPUT);
            setPin(RIGHTREVERSE, INPUT);
            break;
    }
}