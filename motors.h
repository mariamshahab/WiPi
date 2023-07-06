/*
 * Class: CSC-615-01 Spring 2020
 * Project: Final Project - Autonomous Vehicle
 * File: motors.h
 *
 */

#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

//motor 1 pins (wiringPi) right-front
#define M1_ENABLE   0 // BCM 17 | Physical 11
#define M1_FORWARD  3 // BCM 22 | Physical 15
#define M1_REVERSE  2 // BCM 27 | Physical 13

//motor 2 pins (wiringPi) right-back
#define M2_ENABLE   6 // BCM 25 | Physical 22
#define M2_FORWARD  4 // BCM 23 | Physical 16
#define M2_REVERSE  5 // BCM 24 | Physical 18

//motor 3 pins (wiringPi) left-back
#define M3_ENABLE   12 // BCM 10 | Physical 19
#define M3_FORWARD  13 // BCM 09 | Physical 21
#define M3_REVERSE  14 // BCM 11 | Physical 23

//motor 4 pins (wiringPi) left-front
#define M4_ENABLE   26 // BCM 12 | Physical 32
#define M4_FORWARD  10 // BCM 08 | Physical 24
#define M4_REVERSE	11 // BCM 07 | Physical 26

#define PWM_MIN -1000
#define PWM_MAX 1000

/*
 * FUNCTION SIGNATURES
 */

//Set pinmodes of motors to out, turn motors on, set up PWM
void initialize_motors();

/*
 * sets magnitude of left forward motor
 *
 * PARAMS
 * magnitude: pwm magnitude to be set [0 - 100]
 *
 */
void set_motor_pwm(int magnitude);

/*
 * Sets all motors' forward speed
 *
 * PARAMS
 * magnitude  speed to set motors to
 */
void set_forward_speed(int magnitude);

void turn_left(int magnitude);
void turn_right(int magnitude);
void stop_turning(int magnitude);

void pivot_right(int speed);
void pivot_left(int speed);

//set all motor outputs to 0
void stop_motors();

//turn all motors off
void terminate_motors();

/*
 * Compares left & right motor speeds,
 * increments/decrements left motor pwm magnitude
 * if the left motor exceeds the threshold.
 */
void* sync_motors(void *arg);

#endif
