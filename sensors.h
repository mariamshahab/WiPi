/*
* Class: CSC-615-01 Spring 2020
* Project: Final Project - Autonomous Vehicle
* File: sensors.h
*
*/

#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H

//echo sensor pins (wiringPi)
#define TRIGGER 21 // BCM 05 | Physical 29
#define ECHO    22 // BCM 06 | Physical 31

//IR encoder pins (wiringPi)
#define ENCODER_LEFT  29 // BCM 21
#define ENCODER_RIGHT 28 // BCM 20

//line sensor pins (wiringPi)
#define LINE_LEFT   15   // BCM 14 | Physical 05
#define LINE_CENTER 9   // BCM 03 | Physical 03
#define LINE_RIGHT  8  // BCM 02 | Physical 08

//sets pinmodes of all sensors
int initialize_sensors();

/*
* Sends out 10 microsecond pulse and begins listening
* for return signal. Calculates distance (in millimeters)*
* from object based on signal time and updates shared variable
* ultrasonicDistance.
*/
void* pulse_ultrasonic(void *arg);

/*
 * Calculates linear velocity of both wheels and
 * updates shared variables 'encoderSpeed'
 */
void* get_encoder_speed(void *arg);

/*
 * Calculates linear velocity of right wheel and
 * updates shared variable 'encoderSpeedRight'
 */
//void* right_encoder_speed(void *arg);

void* follow_line(void *arg);

#endif
