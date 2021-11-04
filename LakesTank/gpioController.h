/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name:Lake Jasper
* Student ID:920150605
* Github ID:TheJaspinater
* Project:
*
* File: gpioController.h
*
* Description:
**************************************************************/
#ifndef GPIOCONTROLLER_H
#define GPIOCONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define BCM2708_PERI_BASE 0x3F000000                     // start of BCM memory
#define GPIO_BASE         (BCM2708_PERI_BASE + 0x200000) // advance to location of GPIO memory

#define PAGE_SIZE  (4*1024)
#define BLOCK_SIZE (4*1024)

volatile unsigned *gpio;

// Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37)     // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

#define OUTPUT 1 // 1 is interpreted as output/pin high/on within setPin function
#define INPUT 0  // 0 is interpreted as input/pin low/off within setPin function
#define HIGH 1
#define LOW 0

//Funcitons
void setPin(int, int);
void writePin(int, int);
int readPin(int);
void initGpio();

#endif