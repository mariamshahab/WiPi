/*
* Class: CSC-615-01 Spring 2020
* Project: Final Project - Autonomous Vehicle
* File: main.c
*/

#include "main.h"
#include "motors.h"
#include "sensors.h"

//initialize SensorData struct defined in 'main.h'
//this struct is shared between all threads
struct SensorData sd = {
  .vehicleState = straight,
  .leftMotorMagnitude = 800,
  .rightMotorMagnitude = 800,
  .lineHit = 0;
};

int main(int argc, char** argv){
  //thread pointers
  pthread_t ultrasonicThread, encoderThread, motorThread, lineThread;
  
  //initialize semaphores
  sem_init(&motor_sem, 0, 1);
  sem_init(&line_sem, 0, 1);
  
  //setup wpi and sensors/motors
  if(setup() != 0) return -1;
  
  //spawn thread 'ultrasonicThread' with start routine 'pulse_ultrasonic' from 'senors.h'
  if(pthread_create(&ultrasonicThread, NULL, pulse_ultrasonic, &sd) != 0){
    printf("failed to spawn thread: ultrasonicThread");
    return -1;
  }
  
  //spawn thread 'encoderThread' with start routine 'left_encoder_speed' from 'senors.h'
  if(pthread_create(&encoderThread, NULL, get_encoder_speed, &sd) != 0){
    printf("failed to spawn thread: encoderThread");
    return -1;
  }
  
  //spawn thread 'motorThread' with start routine 'sync_motors' from 'motors.h'
  if(pthread_create(&motorThread, NULL, sync_motors, &sd) != 0){
    printf("failed to spawn thread: motorThread");
    return -1;
  }
  
  //spawn thread 'lineThread' with start routine 'follow_line' from 'sensors.h'
  if(pthread_create(&lineThread, NULL, follow_line, &sd) != 0){
    printf("failed to spawn thread: lineThread");
    return -1;
  }
    
  //start driving
  set_forward_speed(sd.rightMotorMagnitude);

  //set_forward_speed(300);
  //sd.rightMotorMagnitude = 300;
  //sd.leftMotorMagnitude = 300;

  char c;
  while(1)
  {
    c = getchar();
    if(c == 'x')
    {
      sem_wait(&motor_sem);
      sd.vehicleState = stop;
      sem_post(&motor_sem);
      break;
    }
  }

  stop_motors();
  terminate_motors();

  return 0;
}

int setup(){
  //setup wiringPi
  if(wiringPiSetup() == -1) {
    printf("wpi setup failed");
    return -1;
  }
  
  //set pinmodes and turn everything on
  initialize_motors();
  initialize_sensors();
  
  return 0;
}
