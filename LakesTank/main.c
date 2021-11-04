/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name:Lake Jasper
* Student ID:920150605
* Github ID:TheJaspinater
* Project:  Assignment 3 - Motors & Motor Shield
*
* File: main.c
*
* Description:
**************************************************************/
#include "gpioController.h"
#include "sensors.h"
#include "softPwm.h"
#include "driveTrain.h"

#define ROTATESPEED 25
#define FORWARDSPEED 25
#define MOTORDELAY 100000
#define OSCCOMP 100000


void stopSystem ();

int main() {
    initGpio();                             // Setup memory region to access GPIO

    setPin(STARTBUTTON, INPUT);             // set STARTBUTTON pin as INPUT
    printf("Waiting on button press...\n");
    while(readPin(STARTBUTTON)) {      // read STARTBUTTON until input is detected
    } sleep(1);                             // Do Nothing until button is pressed

    //Initialize - - - - - - - - - - - - - - -
    initMotors();
    initServo(SSERVO);
    
    // Main Control Loop - - - - - - - - - - - -
    int liRef = 0; // line Refference
    double dist = 100;

    int count = 0;
    int prevLiRef = 0;
    int prevSpeed = 0;
    int prevDirection = FORWARD;

    for (;;) {
        liRef = lineScan();
        printf ("liRef: %d\n", liRef);

        switch (liRef) {
            case 0: // No line detected
                if (count > 100) {
                    stopSystem ();
                    printf("End/Loss Of Track.\n");
                    return 0;
                }
                printf ("prevLiRef: %d\n", prevLiRef);
                steer(prevLiRef, prevSpeed);
                count++;
                break;
            case 1:       // [  |  _  _  _  ]
                steer(ROTATELEFT, ROTATESPEED);
                prevLiRef = liRef;
                count = 0;
                prevSpeed = ROTATESPEED;
                prevDirection = ROTATELEFT;
                while ( lineScan() == 0 ) {
                    usleep(500);
                }
                break;
            case 2:       // [  _  |  _  _  ]
                printf("Error %d: Line Reading Not Possible.\n", liRef);
                break;
            case 3:       // [  |  |  _  _  ]
                steer(ROTATELEFT, ROTATESPEED);
                prevLiRef = liRef;
                prevSpeed = ROTATESPEED;
                prevDirection = ROTATELEFT;
                count = 0;
                while ( lineScan() == 0 ) {
                    usleep(500);
                }
                break;
            case 4:       // [  _  _  |  _  ]
                printf("Error %d: Line Reading Not Possible.\n", liRef);
                break;
            case 5:       // [  |  _  |  _  ]
                printf("Error %d: Line Reading Not Possible.\n", liRef);
                break;
            case 6:       // [  _  |  |  _  ]
                steer(FORWARD, FORWARDSPEED);
                prevLiRef = liRef;
                count = 0;
                prevSpeed = FORWARDSPEED;
                prevDirection = FORWARD;
                break;
            case 7:       // [  |  |  |  _  ]
                if (prevLiRef == 14) {
                    steer(FORWARD, FORWARDSPEED);
                    usleep(OSCCOMP);
                }
                steer(ROTATELEFT, ROTATESPEED);
                prevLiRef = liRef;
                count = 0;
                prevSpeed = ROTATESPEED;
                prevDirection = ROTATELEFT;
                usleep(MOTORDELAY); // Allow time to begin translation to prevent false reads which cause the vehicle to continue forward off of the line
                while ( lineScan() == 0 ) {
                    usleep(500);
                }
                break;
            case 8:       // [  _  _  _  |  ]
                steer(ROTATERIGHT, ROTATESPEED);
                prevLiRef = liRef;
                count = 0;
                prevSpeed = ROTATESPEED;
                prevDirection = ROTATERIGHT;
                while ( lineScan() == 0 ) {
                    usleep(500);
                }
                break;
            case 9:       // [  |  _  _  |  ]
                printf("Error %d: Line Reading Not Possible.\n", liRef);
                break;
            case 10:      // [  _  |  _  |  ]
                printf("Error %d: Line Reading Not Possible.\n", liRef);
                break;
            case 11:      // [  |  |  _  |  ]
                printf("Error %d: Line Reading Not Possible.\n", liRef);
                break;
            case 12:      // [  _  _  |  |  ]
                steer(ROTATERIGHT, ROTATESPEED);
                prevLiRef = liRef;
                count = 0;
                prevSpeed = ROTATESPEED;
                prevDirection = ROTATERIGHT;
                while ( lineScan() == 0 ) {
                    usleep(500);
                }
                break;
            case 13:      // [  |  _  |  |  ]
                printf("Error %d: Line Reading Not Possible.\n", liRef);
                break;
            case 14:      // [  _  |  |  |  ]
                if (prevLiRef == 7) {
                    steer(FORWARD, FORWARDSPEED);
                    usleep(OSCCOMP);
                }
                steer(ROTATERIGHT, ROTATESPEED);
                prevLiRef = liRef;
                count = 0;
                prevSpeed = ROTATESPEED;
                prevDirection = ROTATERIGHT;
                usleep(MOTORDELAY); // Allow time to begin translation to prevent false reads which cause the vehicle to continue forward off of the line
                while ( lineScan() == 0 ) {
                    usleep(500);
                }
                break;
            case 15:      // [  |  |  |  |  ]
                steer(FORWARD, FORWARDSPEED);
                break;
            default:
                break;
        }

        dist = averageDistCM(10); // obsticle detected
        printf("dist: %lf\n", dist);
        if ( dist < 15 && dist > 0) {
            printf("Object Detected.\n");
            
            if (prevLiRef == 6 || prevLiRef == 15) {// Stop vehicle if mocing forward
                steer(REVERSE, FORWARDSPEED);
                usleep(10000);
            }
            steer(STOP, 0);

            while (dist < 15) {
                dist = averageDistCM(10);
                printf("dist: %lf\n", dist);
            }
        }

        if (readPin(STARTBUTTON ) == 0) {
            stopSystem ();
            return 0;
        }

    }

    // Shut down - - - - - - - - - - - - - - - -
    stopSystem ();

    return 0;
}

void stopSystem () {
    steer(STOP, 0);
    stopMotors();
    stopServo(SSERVO);
}