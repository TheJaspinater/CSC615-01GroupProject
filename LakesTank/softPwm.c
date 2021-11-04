/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name:Lake Jasper
* Student ID:920150605
* Github ID:TheJaspinater
* Project:
*
* File:softPwm.c
*
* Description:
**************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>

#include "gpioController.h"
#include "softPwm.h"

#define	MAX_PINS	64

#define	PULSE_TIME	100

static volatile int marks         [MAX_PINS] ;
static volatile int range         [MAX_PINS] ;
static volatile pthread_t threads [MAX_PINS] ;
static volatile int newPin = -1 ;

static void *softPwmThread (void *arg)
{
  int pin, mark, space ;
  struct sched_param param ;

  param.sched_priority = sched_get_priority_max (SCHED_RR) ;
  pthread_setschedparam (pthread_self (), SCHED_RR, &param) ;

  pin = *((int *)arg) ;
  free (arg) ;

  pin    = newPin ;
  newPin = -1 ;

  piHiPri (90) ;

  for (;;)
  {
    mark  = marks [pin] ;
    space = range [pin] - mark ;
    if (mark != 0) {
      setPin(pin, OUTPUT);
    }
    usleep (mark * 100) ;

    if (space != 0) {
      setPin(pin, INPUT);
    }
    usleep (space * 100) ;
  }

  return NULL ;
}

void softPwmWrite (int pin, int value)
{
  if (pin < MAX_PINS)
  {
    if (value < 0)
      value = 0 ;
    else if (value > range [pin])
      value = range [pin] ;
    marks [pin] = value ;
  }
}

int softPwmCreate (int pin, int initialValue, int pwmRange)
{
  int res ;
  pthread_t myThread ;
  int *passPin ;

  if (pin >= MAX_PINS)
    return -1 ;

  if (range [pin] != 0)	// Already running on this pin
    return -1 ;

  if (pwmRange <= 0)
    return -1 ;

  passPin = malloc (sizeof (*passPin)) ;
  if (passPin == NULL)
    return -1 ;

  marks [pin] = initialValue ;
  range [pin] = pwmRange ;

  *passPin = pin ;
  newPin   = pin ;
  res      = pthread_create (&myThread, NULL, softPwmThread, (void *)passPin) ;

  if (res != 0)
    return res ;
  
  while (newPin != -1)
    usleep (1000) ;

  threads [pin] = myThread ;

  return res ;
}

void softPwmStop (int pin)
{
  if (pin < MAX_PINS)
  {
    if (range [pin] != 0)
    {
      pthread_cancel (threads [pin]) ;
      pthread_join   (threads [pin], NULL) ;
      range [pin] = 0 ;
      setPin (pin, LOW) ;
    }
  }
}

int piHiPri (const int pri) {
  struct sched_param sched ;

  memset (&sched, 0, sizeof(sched)) ;

  if (pri > sched_get_priority_max (SCHED_RR))
    sched.sched_priority = sched_get_priority_max (SCHED_RR) ;
  else
    sched.sched_priority = pri ;

  return sched_setscheduler (0, SCHED_RR, &sched) ;
}