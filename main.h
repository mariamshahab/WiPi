/*
* Class: CSC-615-01 Spring 2020
* Project: Final Project - Autonomous Vehicle
* File: main.h
*/

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>

sem_t motor_sem;
sem_t line_sem;

enum state{straight = 0, left = 1, right = 2, pivotLeft = 3, pivotRight = 4, offroad = 5, stop = 6};

struct SensorData{
  float ultrasonicDistance;
  float encoderSpeedLeft;
  float encoderSpeedRight;
  
  int leftMotorMagnitude;
  int rightMotorMagnitude;
  int lineHit;
  
  enum state vehicleState;
};

int setup();

#endif
