/*
* Class: CSC-615-01 Spring 2020
* Project: Final Project - Autonomous Vehicle
* File: motors.c
*/

#include "motors.h"
#include "main.h"

void initialize_motors(){
  //set pinModes for motor 1
  pinMode(M1_FORWARD, OUTPUT);
  pinMode(M1_REVERSE, OUTPUT);
  pinMode(M1_ENABLE, OUTPUT);
  
  //set pinModes for motor 2
  pinMode(M2_FORWARD, OUTPUT);
  pinMode(M2_REVERSE, OUTPUT);
  pinMode(M2_ENABLE, OUTPUT);
  
  //set pinModes for motor 3
  pinMode(M3_FORWARD, OUTPUT);
  pinMode(M3_REVERSE, OUTPUT);
  pinMode(M3_ENABLE, OUTPUT);
  
  //set pinModes for motor 4
  pinMode(M4_FORWARD, OUTPUT);
  pinMode(M4_REVERSE, OUTPUT);
  pinMode(M4_ENABLE, OUTPUT);
  
  //turn motors on
  digitalWrite(M1_ENABLE, HIGH);
  digitalWrite(M2_ENABLE, HIGH);
  digitalWrite(M3_ENABLE, HIGH);
  digitalWrite(M4_ENABLE, HIGH);
  
  //setup PWM with range of 100 possible speed values
  softPwmCreate(M1_FORWARD, 0, PWM_MAX);
  softPwmCreate(M1_REVERSE, 0, PWM_MAX);
  
  softPwmCreate(M2_FORWARD, 0, PWM_MAX);
  softPwmCreate(M2_REVERSE, 0, PWM_MAX);
  
  softPwmCreate(M3_FORWARD, 0, PWM_MAX);
  softPwmCreate(M3_REVERSE, 0, PWM_MAX);
  
  softPwmCreate(M4_FORWARD, 0, PWM_MAX);
  softPwmCreate(M4_REVERSE, 0, PWM_MAX);
}

void set_motor_pwm(int magnitude){
  if(magnitude >= 0)
  {
    softPwmWrite(M4_REVERSE, 0);
    softPwmWrite(M4_FORWARD, magnitude);
  }
  else
  {
    softPwmWrite(M4_REVERSE, abs(magnitude));
    softPwmWrite(M4_FORWARD, 0);
  }
}

void set_forward_speed(int magnitude){
  //set reverse motor-pins to 0
  softPwmWrite(M1_REVERSE, 0);
  softPwmWrite(M2_REVERSE, 0);
  softPwmWrite(M3_REVERSE, 0);
  softPwmWrite(M4_REVERSE, 0);
  
  //set forward motor-pins to magnitude
  softPwmWrite(M1_FORWARD, magnitude);
  softPwmWrite(M2_FORWARD, magnitude);
  softPwmWrite(M3_FORWARD, magnitude);
  softPwmWrite(M4_FORWARD, magnitude);
}

void turn_left(int magnitude){
  softPwmWrite(M4_FORWARD, 0);
  softPwmWrite(M4_REVERSE, magnitude);
}

void turn_right(int magnitude){
  softPwmWrite(M1_FORWARD, 0);
  softPwmWrite(M1_REVERSE, magnitude);
}

void stop_turning(int magnitude){
  softPwmWrite(M1_REVERSE, 0);
  softPwmWrite(M2_REVERSE, 0);
  softPwmWrite(M3_REVERSE, 0);
  softPwmWrite(M4_REVERSE, 0);
  softPwmWrite(M1_FORWARD, magnitude);
  softPwmWrite(M2_FORWARD, magnitude);
  softPwmWrite(M3_FORWARD, magnitude);
  softPwmWrite(M4_FORWARD, magnitude);
}

void pivot_right(int speed){
  stop_motors();
  softPwmWrite(M1_REVERSE, speed);
  softPwmWrite(M2_REVERSE, speed);
  softPwmWrite(M3_FORWARD, speed);
  softPwmWrite(M4_FORWARD, speed);
}

void pivot_left(int speed){
  stop_motors();
  softPwmWrite(M1_FORWARD, speed);
  softPwmWrite(M2_FORWARD, speed);
  softPwmWrite(M3_REVERSE, speed);
  softPwmWrite(M4_REVERSE, speed);
}

void stop_motors(){
  softPwmWrite(M1_FORWARD, 0);
  softPwmWrite(M1_REVERSE, 0);
  softPwmWrite(M2_FORWARD, 0);
  softPwmWrite(M2_REVERSE, 0);
  softPwmWrite(M3_FORWARD, 0);
  softPwmWrite(M3_REVERSE, 0);
  softPwmWrite(M4_FORWARD, 0);
  softPwmWrite(M4_REVERSE, 0);
}

void terminate_motors(){
  digitalWrite(M1_ENABLE, LOW);
  digitalWrite(M2_ENABLE, LOW);
  digitalWrite(M3_ENABLE, LOW);
  digitalWrite(M4_ENABLE, LOW);
}

void* sync_motors(void *arg){
  struct SensorData *sd = (struct SensorData *) arg;
  int encoderThreshold = .01;
  int ultrasonicThreshold = 100;
  
  while(sd->vehicleState != stop)
  {
    //if vehicle is going straight
    if(sd->vehicleState == straight)
    {
      stop_turning(sd->rightMotorMagnitude);
      //normalize motor speeds
      if(sd->encoderSpeedLeft < sd->encoderSpeedRight - encoderThreshold)
      {
        //don't let motorMagnitude go outside of range [0-1000]
        if(sd->leftMotorMagnitude < PWM_MAX)
        {
          sd->leftMotorMagnitude += 1;
          set_motor_pwm(sd->leftMotorMagnitude); //increment motor speed by 1
        }
      }
      else if(sd->encoderSpeedLeft > sd->encoderSpeedRight + encoderThreshold)
      {
        if(sd->leftMotorMagnitude > PWM_MIN)
        {
          sd->leftMotorMagnitude -= 1;
          set_motor_pwm(sd->leftMotorMagnitude); //decrement motor speed by 1
        }
      }
    }
    
    //if vehicle is turning left
    else if(sd->vehicleState == left) turn_left(sd->rightMotorMagnitude);
    
    //if pivoting left
    else if(sd->vehicleState == pivotLeft) pivot_left(sd->rightMotorMagnitude);
    
    //if pivoting right
    else if(sd->vehicleState == pivotRight) pivot_right(sd->rightMotorMagnitude);
    
    //if vehicle is turning right
    else if(sd->vehicleState == right) turn_right(sd->rightMotorMagnitude);
    
    //if vehicle is leaving the line
    else if(sd->vehicleState == offroad)
    {
      
      stop_motors();
      delay(3000);
      
      if(sd->ultrasonicDistance > 500)
      {
        sem_wait(&motor_sem);
        sd->vehicleState = straight;
        sem_post(&motor_sem);
        continue;
      }
      
      clock_t start;
      double time;

      //time how long it takes to clear the obstacle
      //start turning around obstacle
      int prevDistance = sd->ultrasonicDistance;
      start = clock();
      pivot_right(sd->rightMotorMagnitude);

      while(1)
      {
        int currDistance = sd->ultrasonicDistance;
        if(currDistance > prevDistance + ultrasonicThreshold) break;
        prevDistance = currDistance;
      }

      time = (double) (clock() - start) / CLOCKS_PER_SEC;
      
      set_forward_speed(sd->rightMotorMagnitude);

      //turn back for the same amount of time it took to clear obstacle
      start = clock();
      while(1) if(((double) (clock() - start) / CLOCKS_PER_SEC) >= (time * 1.5)) break;

      pivot_left(sd->rightMotorMagnitude);

      start = clock();
      while(1) if(((double) (clock() - start) / CLOCKS_PER_SEC) >= (time * 1.3)) break;
      
      set_forward_speed(sd->rightMotorMagnitude);
      
      sem_wait(&line_sem);
      sd->lineHit = 0;
      sem_post(&line_sem);
      
      while(sd->lineHit == 0);
      
      pivot_right(sd->rightMotorMagnitude);
      
      delay(100);
      
      sem_wait(&motor_sem);
      sd->vehicleState = straight;
      sem_post(&motor_sem);
    }
  }
  
  stop_motors();
  terminate_motors();
}







