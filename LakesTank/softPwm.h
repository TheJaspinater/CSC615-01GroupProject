/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name:Lake Jasper
* Student ID:920150605
* Github ID:TheJaspinater
* Project:  Assignment 2 - Tapeless Ruler
*
* File: assignment2.h
*
* Description: This file is reponsible for supporting
* assignment.c with needed library inclusions, definitions, and
* and method signitures.
**************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

extern int  softPwmCreate (int pin, int value, int range) ;
extern void softPwmWrite  (int pin, int value) ;
extern void softPwmStop   (int pin) ;
extern int  piHiPri       (const int pri) ;

#ifdef __cplusplus
}
#endif