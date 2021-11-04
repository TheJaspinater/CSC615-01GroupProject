/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name:Lake Jasper
* Student ID:920150605
* Github ID:TheJaspinater
* Project:
*
* File: gpioController.c
*
* Description:
**************************************************************/
#include "gpioController.h"

void setPin(int pin, int state) {
    switch (state) {
        case 1:                  // Set pin as output
            INP_GPIO(pin);       // must use INP_GPIO before we can use OUT_GPIO
            OUT_GPIO(pin);       // set pin to output mode
            GPIO_SET = 1<<pin;
            break;
        case 0:                  // Set pin as input
            GPIO_CLR = 1<<pin;   // Clear bits
            INP_GPIO(pin);       // Set pin as input
            break;
        default:
            break;
    }
}

int readPin(int pin) {  // Return 1 pin read is true/on, 0 if false/off
    return GET_GPIO(pin);
}

void initGpio() {
    int  fd;
    void *gpio_map;

    if ((fd = open("/dev/gpiomem", O_RDWR|O_SYNC) ) < 0) { //gpiomem allows for non super user access to memory
        printf("can't open /dev/gpiomem \n");
        exit(-1);
    }
   
    gpio_map = mmap(
        NULL,                 //If NULL, kernel will place where it sees fit.
        BLOCK_SIZE,           //Map length
        PROT_READ|PROT_WRITE, // Enable reading & writting to mapped memory
        MAP_SHARED,           //Shared with other processes
        fd,                   //File to map
        GPIO_BASE             //Offset to GPIO peripheral
    );

    close(fd); //No need to keep mem_fd open after mmap

    if (gpio_map == MAP_FAILED) {
        printf("mmap error %d\n", (int)gpio_map);
        exit(-1);
    }
   
    gpio = (volatile unsigned *)gpio_map;
}