/*
 * Class: CSC-615-01 Spring 2020
 * Project: Final Project - Autonomous Vehicle
 * File: sensors.c
 */

#include "main.h"
#include "sensors.h"
#include "motors.h"

int initialize_sensors(){
  //ultrasonic sensor
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  
  //encoder sensors
  pinMode(ENCODER_LEFT, INPUT);
  pinMode(ENCODER_RIGHT, INPUT);
  
  //line sensors
  pinMode(LINE_LEFT, INPUT);
  pinMode(LINE_CENTER, INPUT);
  pinMode(LINE_RIGHT, INPUT);
  
  return 0;
}

void* pulse_ultrasonic(void *arg){
  struct SensorData *sd = (struct SensorData *) arg;
  struct timeval tv1, tv2;
  long time1, time2;
  float finalTime, distance;
  
  while(1)
  {
    //reset trigger
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);
    
    //pulse trigger for 10 microseconds
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);
    
    //begin listening for return signal
    while(!(digitalRead(ECHO) == 1));
    gettimeofday(&tv1, 0);

    while(!(digitalRead(ECHO) == 0));
    gettimeofday(&tv2, 0);

    //calculate distance based on time before and after signal recieved
    time1 = tv1.tv_sec * 1000000 + tv1.tv_usec;
    time2  = tv2.tv_sec * 1000000 + tv2.tv_usec;
    
    finalTime = (float) (time2 - time1) / 1000000.0;

    distance = (finalTime * 343000) / 2;
//    printf("distance: %fmms\n", distance);

    if(distance < 400 && sd->vehicleState != offroad) //1 foot
    {
      sem_wait(&motor_sem);
      sd->vehicleState = offroad;
      sem_post(&motor_sem);
    }
    
    sd->ultrasonicDistance = distance;
    delay(500);
  }
}

void* get_encoder_speed(void *arg){
  struct SensorData *sd = (struct SensorData *) arg;
  int leftCount = 0, leftPrevRead, leftRead;
  int rightCount = 0, rightPrevRead, rightRead;
  float rightSpeed, leftSpeed;
  double samplePeriod;
  clock_t start, time;
  
  //start sampling time
  start = clock();
  
  while(1){
    leftRead = digitalRead(ENCODER_LEFT);
    rightRead = digitalRead(ENCODER_RIGHT);
    
    if(leftRead == 1 && leftPrevRead != leftRead)
    {
      leftCount++;
    }
    if(rightRead == 1 && rightPrevRead != rightRead)
    {
      rightCount++;
    }
    
    //calculate length of period
    time = clock() - start;
    samplePeriod = ((double) time) / CLOCKS_PER_SEC;

    //calculate speed
    leftSpeed = (2 * M_PI * leftCount) / (20 * samplePeriod) * 1.25;
    rightSpeed = (2 * M_PI * rightCount) / (20 * samplePeriod) * 1.25;
    
    sd->encoderSpeedLeft = leftSpeed;
    sd->encoderSpeedRight = rightSpeed;
    
    leftPrevRead = leftRead;
    rightPrevRead = rightRead;
  }
}

void* follow_line(void *arg){
  struct SensorData *sd = (struct SensorData *) arg;
  int lineLeft, lineCenter, lineRight;
  
  while(1)
  {
    lineLeft = digitalRead(LINE_LEFT);
    lineCenter = digitalRead(LINE_CENTER);
    lineRight = digitalRead(LINE_RIGHT);

    if(sd->vehicleState != offroad) //if it is not purposely leaving the line (avoiding obstacle)
    {
      
      //if vehicle is skewing right
      if(lineLeft == 1 && lineCenter == 0 && lineRight == 0)
      {
        sem_wait(&motor_sem);
        sd->vehicleState = left;
        sem_post(&motor_sem);
      }
      
      //if vehicle is skewing left
      else if(lineLeft == 0 && lineCenter == 0 && lineRight == 1)
      {
        sem_wait(&motor_sem);
        sd->vehicleState = right;
        sem_post(&motor_sem);
      }
      
      //if needs to pivot left
      else if(lineLeft == 1 && lineCenter == 1 && lineRight == 0)
      {
        sem_wait(&motor_sem);
        sd->vehicleState = pivotLeft;
        sem_post(&motor_sem);
      }
      
      //if needs to pivot right
      else if(lineLeft == 0 && lineCenter == 1 && lineRight == 1)
      {
        sem_wait(&motor_sem);
        sd->vehicleState = pivotRight;
        sem_post(&motor_sem);
      }
      
      //otherwise continue straight
      else if(lineLeft == 0 && lineRight == 0 && lineCenter == 1)
      {
        sem_wait(&motor_sem);
        sd->vehicleState = straight;
        sem_post(&motor_sem);
      }
    }
    else
    {
      if(lineLeft == 1 || lineRight == 1 || lineCenter == 1){
        printf("lineHit\n");
        sd->lineHit = 1;
      }
    }
  }
}
