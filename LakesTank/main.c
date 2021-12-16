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
/*****************************
 * global variables and defines
 ****************************/

#define ROTATESPEED  60 // 40 for OBsticles // 60 top speed // maybe 50? Haven't tested full battery
#define FORWARDSPEED 60

#define LINESCANERROR(line) printf( "Error: case %d : Line Reading Not Possible.\n", line );

int CurLineVal = 0, prevLineVal = 0, dist = 0, test = 1;
/*****************************
 * Function signitures
 ****************************/

void   stopSystem    ();
double averageDistCM ( int );
void   findLine      ();
void   avoidObstacle ();

/*****************************
 * begin main loop
 ****************************/

int main() {
    /*****************************
     * map memory
     ****************************/
    initGpio();                                // Setup memory region to access GPIO

    /*****************************
     * Await button press to begin line tracking
     ****************************/
    setPin ( STARTBUTTON, INPUT );             // set STARTBUTTON pin as INPUT
    printf ( "Waiting on button press...\n" );
    while  ( readPin ( STARTBUTTON ) )         // read STARTBUTTON until input is detected
    {           
    } sleep(1);                                // Do Nothing until button is pressed

    /*****************************
     * Initialize hardware
     ****************************/
    initMotors      ();
    initLineScanner ();
    initRangeFinder ();
    sleep           ( 1 );

    /*****************************
     * Begin line tracking
     ****************************/
    for ( ;; )
    {
        /*****************************
         * Debugging
         ****************************/

        /*****************************
         * Button Presses. Shut down and exit.
         ****************************/
        if ( readPin ( STARTBUTTON ) == 0 ) {
            printf ( "Stopping system...\n" );
            stopSystem ();
            return 0;
        }

        /*****************************
         * check for and avoid obsticles
         ****************************/
        dist = getDist ();
        
        if ( dist < 50 && dist > 0 ) {
            printf ( "Object detected at: %dcm\n", dist );
            steer( STOP, 0);    
            avoidObstacle ();
        } else {
            /*****************************
             * No Obsticle, Track line.
             ****************************/
            CurLineVal = getLineState ();
            // if ( CurLineVal != prevLineVal ) {
            //     printf ( "Line Value: %d\n", CurLineVal );
            // }
            switch ( CurLineVal ) {
                case 0: // No line detected
                    if ( prevLineVal == 7 ) {
                        steer ( ROTATELEFT, ROTATESPEED );
                        findLine ();
                    } else if ( prevLineVal == 14 ) {
                        steer ( ROTATERIGHT, ROTATESPEED );
                        findLine ();
                    } else {
                        steer (STOP, 0);
                        printf ( "End/Loss Of Track.\n Place vehicle back on track and press button to restart.\n" );
                        while ( readPin ( STARTBUTTON ) ) {
                        } sleep(1); 
                    }
                    break;
                case 1:       // [  |  _  _  _  ]
                    steer ( ROTATELEFT, ROTATESPEED );
                    findLine ();
                    break;
                case 2:       // [  _  |  _  _  ]
                    LINESCANERROR ( 2 );
                    break;
                case 3:       // [  |  |  _  _  ]
                    steer ( ROTATELEFT, ROTATESPEED );
                    break;
                case 4:       // [  _  _  |  _  ]
                    LINESCANERROR ( 4 );
                    break;
                case 5:       // [  |  _  |  _  ]
                    LINESCANERROR ( 5 );
                    break;
                case 6:       // [  _  |  |  _  ]
                    steer ( FORWARD, FORWARDSPEED );
                    break;
                case 7:       // [  |  |  |  _  ]
                    steer ( TURNLEFT, ROTATESPEED );
                    prevLineVal = CurLineVal;
                    break;
                case 8:       // [  _  _  _  |  ]
                    steer ( ROTATERIGHT, ROTATESPEED );
                    findLine ();
                    break;
                case 9:       // [  |  _  _  |  ]
                    LINESCANERROR ( 9 );
                    break;
                case 10:      // [  _  |  _  |  ]
                    LINESCANERROR ( 10 );
                    break;
                case 11:      // [  |  |  _  |  ]
                    LINESCANERROR ( 11 );
                    break;
                case 12:      // [  _  _  |  |  ]
                    steer ( ROTATERIGHT, ROTATESPEED );
                    break;
                case 13:      // [  |  _  |  |  ]
                    LINESCANERROR ( 13 );
                    break;
                case 14:      // [  _  |  |  |  ]
                    steer ( TURNRIGHT, ROTATESPEED );
                    prevLineVal = CurLineVal;
                    break;
                case 15:      // [  |  |  |  |  ]
                    steer ( FORWARD, FORWARDSPEED );
                    break;
                default:
                    break;
            }
        }
    }
}

void stopSystem () {
    steer ( STOP, 0 );
    stopMotors();
    stopServo ( SSERVO );
    stopLineScanner ();
    stopRangeFinder ();
}

double averageDistCM ( int measurements ) {
    double dist = 0;
    for ( int i = 0; i < measurements; i++ )
        dist += getDistanceCM ();
    return dist/measurements;
}

void findLine () {
    float miliSecOffLine = 0;
    printf("Finding Line...\n");   
    while ( getLineState () == 0  && miliSecOffLine < 600 ) {
        printf("Finding Line: %f\n", miliSecOffLine);
        usleep ( 100 );
        miliSecOffLine+=0.1;
    }
}

void avoidObstacle () {
    printf("Beginning object avoidence routine...\n");

    steer ( REVERSE, 20 );
    sleep(1);

    steer ( ROTATELEFT, 20 ); // arbitrary value used to acheive roughply 90 degree rotation
    sleep(3);

    look ( SSERVO, ANGLE90 );
    while ( getLineState () == 0 ) {
        steer ( CLOCKWISE, 50 );
    }

    printf("Object successfully avoided.\n");
}