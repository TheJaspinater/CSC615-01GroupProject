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

#define ROTATESPEED  30
#define FORWARDSPEED 30

/*****************************
 * Function signitures
 ****************************/

void   stopSystem    ();
double averageDistCM ( int );

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
    initServo       ( SSERVO );
    initLineScanner ();
    sleep           ( 1 );

    /*****************************
     * Begin line tracking
     ****************************/
    double dist = 100;
    double leftDist = 0;
    double rightDist = 0;

    int count = 0;
    int curLiRef = 0;
    int prevLiRef = 0;
    int prevSpeed = 0;
    int prevDirection = FORWARD;

    for ( ;; )
    {
        /*****************************
         * Debugging
         ****************************/
        printf ( "Line Read: %d\n", prevLiRef );


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
        dist = averageDistCM ( 5 );                    // obsticle detected
        //printf("dist: %lf\n", dist);
        if ( dist < 30 && dist > 0)
        {
            printf ( "Object Detected - - - - - - - - - - - - - - -\n" );
            
            steer ( STOP, 0 );
            usleep ( 1000000 );

            dist = averageDistCM ( 5 ); // get new and more accurate distance
            printf ( "New distance %lf\n", dist );

            look ( SSERVO, ANGLE0 ) ; // Identify best direction to travel arround object
            usleep ( 500000 ) ;
            leftDist = averageDistCM ( 5 ) ;
            look ( SSERVO, ANGLE180 ) ; // Identify best direction to travel arround object
            usleep ( 500000 ) ;
            rightDist = averageDistCM ( 5 ) ;
            printf("Left: %lf, Right: %lf\n", leftDist, rightDist);

            if ( leftDist > rightDist ) // more room to the left of the vehicle. Go left
            {
                look ( SSERVO, ANGLE0 ); // Identify best direction to travel arround object
                usleep ( 500000 );

                steer  ( ROTATELEFT, ROTATESPEED ) ;
                usleep ( 1000000 ) ; // value picked to acheive roughly a 90 degree rotatio
                dist = averageDistCM ( 5 );
                printf ( "New left distance %lf\n", dist );

            }
            else
            {

            }

            look(SSERVO, ANGLE90);

            printf ( "Waiting on button press...\n" );
            while  ( readPin ( STARTBUTTON ) )         // read STARTBUTTON until input is detected
            {           
            } sleep(1);           
        }

        /*****************************
         * No Obsticle, Track line.
         ****************************/

        curLiRef = getLineState ();
        switch ( curLiRef ) {
            case 0: // No line detected
                if ( count > 20 ) {
                    printf ( "End/Loss Of Track.\n" );
                    stopSystem ();
                    return 0;
                }
                steer ( prevDirection, prevSpeed );
                count++;
                break;
            case 1:       // [  |  _  _  _  ]
                steer ( ROTATELEFT, ROTATESPEED );
                prevLiRef = curLiRef;
                prevDirection = ROTATELEFT;
                count = 0;
                prevSpeed = ROTATESPEED;
                while ( getLineState () == 0 ) {
                    usleep ( 500 );
                }
                break;
            case 2:       // [  _  |  _  _  ]
                printf ( "Error: case 2 : Line Reading Not Possible.\n" );
                break;
            case 3:       // [  |  |  _  _  ]
                steer ( ROTATELEFT, ROTATESPEED );
                prevLiRef = curLiRef;
                prevDirection = ROTATELEFT;
                prevSpeed = ROTATESPEED;
                count = 0;
                while ( getLineState () == 0 ) {
                    usleep ( 500 );
                }
                break;
            case 4:       // [  _  _  |  _  ]
                printf ( "Error: case 4 : Line Reading Not Possible.\n" );
                break;
            case 5:       // [  |  _  |  _  ]
                printf ( "Error: case 5 : Line Reading Not Possible.\n" );
                break;
            case 6:       // [  _  |  |  _  ]
                steer ( FORWARD, FORWARDSPEED );
                prevLiRef = curLiRef;
                prevDirection = FORWARD;
                count = 0;
                prevSpeed = FORWARDSPEED;
                break;
            case 7:       // [  |  |  |  _  ]
                if ( prevLiRef == 14 ) {
                    steer ( FORWARD, FORWARDSPEED );
                    usleep ( 50000 );
                    prevDirection = FORWARD;
                    prevSpeed = FORWARDSPEED;
                } else {
                    steer ( ROTATELEFT, ROTATESPEED );
                    prevDirection = ROTATELEFT;
                    prevSpeed = ROTATESPEED;
                    usleep ( 100000 ); // Allow time to begin translation to prevent false reads which cause the vehicle to continue forward off of the line
                    while ( getLineState () == 0 ) {
                        usleep(500);
                    }
                    prevLiRef = curLiRef;
                    count = 0;
                }
                break;
            case 8:       // [  _  _  _  |  ]
                steer ( ROTATERIGHT, ROTATESPEED );
                prevLiRef = curLiRef;
                prevDirection = ROTATERIGHT;
                count = 0;
                prevSpeed = ROTATESPEED;
                while ( getLineState () == 0 ) {
                    usleep ( 500 );
                }
                break;
            case 9:       // [  |  _  _  |  ]
                printf ( "Error: case 9 : Line Reading Not Possible.\n" );
                break;
            case 10:      // [  _  |  _  |  ]
                printf ( "Error: case 10 : Line Reading Not Possible.\n" );
                break;
            case 11:      // [  |  |  _  |  ]
                printf ( "Error: case 11 : Line Reading Not Possible.\n" );
                break;
            case 12:      // [  _  _  |  |  ]
                steer ( ROTATERIGHT, ROTATESPEED );
                prevLiRef = curLiRef;
                prevDirection = ROTATERIGHT;
                count = 0;
                prevSpeed = ROTATESPEED;
                while ( getLineState () == 0 ) {
                    usleep ( 500 );
                }
                break;
            case 13:      // [  |  _  |  |  ]
                printf ( "Error: case 13 : Line Reading Not Possible.\n" );
                break;
            case 14:      // [  _  |  |  |  ]
                if ( prevLiRef == 7 ) {
                    steer ( FORWARD, FORWARDSPEED );
                    usleep ( 50000 );
                    prevDirection = FORWARD;
                    prevSpeed = FORWARDSPEED;
                } else {
                    steer ( ROTATERIGHT, ROTATESPEED );
                    prevDirection = ROTATERIGHT;
                    prevSpeed = ROTATESPEED;
                    usleep ( 100000 ); // Allow time to begin translation to prevent false reads which cause the vehicle to continue forward off of the line
                    while ( getLineState () == 0 ) {
                        usleep(500);
                    }
                    prevLiRef = curLiRef;
                    count = 0;
                }
                break;
            case 15:      // [  |  |  |  |  ]
                // if ( prevLiRef == 7 )
                // {
                //     steer(ROTATELEFT, ROTATESPEED);
                //     usleep(10000); // Allow time to begin translation to prevent false reads which cause the vehicle to continue forward off of the line
                // }
                // else if ( prevLiRef == 14 )
                // {
                //     steer ( ROTATERIGHT, ROTATESPEED );
                //     usleep ( 10000 ); // Allow time to begin translation to prevent false reads which cause the vehicle to continue forward off of the line
                // }
                // else
                // {
                    steer ( FORWARD, FORWARDSPEED );
                    prevLiRef = curLiRef;
                    prevDirection = FORWARD;
                    count = 0;
                    prevSpeed = FORWARDSPEED;
                    //usleep ( 1000 );
                // }
                break;
            default:
                break;
        }
    }
}

void stopSystem () {
    steer ( STOP, 0 );
    stopMotors();
    stopServo ( SSERVO );
    stopLineScanner ();
}

double averageDistCM ( int measurements ) {
    double dist = 0;

    for ( int i = 0; i < measurements; i++ ) {
        dist += getDistanceCM ();
    }

    return dist/measurements;
}