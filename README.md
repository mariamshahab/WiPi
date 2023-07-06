# WiPi Autonomous Vehicle
This project contains code for driving a small autonomous vehicle around a track. The vehicle uses line sensors to stay on the track, an echo sensor for obstical avoidance, 4 motors and IR speed encoders for wheel synchronization and speed/distance calculation. The program listens to each sensor in parallel in order to make timely decisions while driving.

## Table of Contents
- **[Hardware Diagram](#hardware-diagram)**
- **[Motors.c](#motors.h)**
    - [Motor Pins](#motor-pins)
    - [Motor Functions](#motor-functions)
- **[Sensors.c](#sensors.h)**
    - [Sensor Pins](#sensor-pins)
    - [Sensor Functions](#sensor-functions)

## Hardware Diagram:
![Hardware Diagram](hardware_diagram.png)

# Motors.h

## Motor Pins

| Name       | WiringPi  | BCM   | Physical  | Desc |
| ---------- |:---------:|:-----:|:---------:|:--|
| M1_FORWARD | 3         | 22    | 15        | Set this pin to turn motor 2 in forward direction |
| M1_REVERSE | 2         | 27    | 13        | Set this pin to turn motor 2 in reverse direction |
| M1_ENABLE  | 0         | 17    | 11        | Set this pin to turn on motor 2 |
| M2_FORWARD | 4         | 23    | 16        | Set this pin to turn motor 2 in forward direction |
| M2_REVERSE | 5         | 24    | 18        | Set this pin to turn motor 2 in reverse direction |
| M2_ENABLE  | 6         | 25    | 22        | Set this pin to turn on motor 2 |
| M3_FORWARD | 13        | 9     | 21        | Set this pin to turn motor 3 in forward direction |
| M3_REVERSE | 14        | 11    | 23        | Set this pin to turn motor 3 in reverse direction |
| M3_ENABLE  | 12        | 10    | 19        | Set this pin to turn on motor 3 |
| M4_FORWARD | 10        | 08    | 24        | Set this pin to turn motor 2 in forward direction |
| M4_REVERSE | 11        | 07    | 26        | Set this pin to turn motor 2 in reverse direction |
| M4_ENABLE  | 26        | 12    | 32        | Set this pin to turn on motor 2 |

## Motor Functions

| Name | Parameters | Returns | Desc |
|:--|:-:|:-:|:--|
|initialize_motors| none| void| sets pinmodes of motors to out, turns motors on, sets up PWM for forward and reverse on all motors.|
|set_motor_pwm| magnitude | void | sets the pwm magnitude of the left motor |
|set_forward_speed| magnitude | void | Sets all motors' forward speed |
|set_reverse_speed| magnitude | void | Sets all motors' reverse speed |
|stop_motors| none | void | set all motor outputs to 0 |
|terminate_motors| none | void | turn all motors off |
|sync_motors| void \*arg| void\* | Compares left & right motor speeds, increments/decrements left motor pwm magnitude if the left motor exceeds the threshold. |

# Sensors.h

## Sensor Pins

| Name         | WiringPi  | BCM   | Physical  | Desc |
| ------------ |:---------:|:-----:|:---------:|:--|
| TRIGGER #1   | 21        | 5     | 29        | Set this pin to send out a pulse on ultrasonic sensor |
| ECHO #1      | 22        | 6     | 31        | Listen to this pin for response from ultrasonic sensor |
| TRIGGER #2   | 25        | 26    | 37        | Set this pin to send out a pulse on ultrasonic sensor |
| ECHO #2      | 24        | 19    | 35        | Listen to this pin for response from ultrasonic sensor |
| ENCODER_LEFT | 29        | 21    | 40        | Read this pin for current state of the left encoder sensor |
| ECODER_RIGHT | 28        | 20    | 38        | Read this pin for current state of the right encoder sensor |
| LINE_LEFT    | 15        | 14    | 08        | Read pin for current state of left line sensor |
| LINE_CENTER  | 9         | 3     | 5         | Read pin for current state of center line sensor |
| LINE_RIGHT   | 8         | 2     | 3         | Read pin for current state of right line sensor |

## Sensor Functions

| Name | Parameters | Returns | Desc |
|:--|:-:|:-:|:--|
|initialize_sensors| none| int| sets pinmodes of all sensors |
| pulse_ultrasonic| void \*arg | void* | Sends out 10 microsecond pulse and begins listening for return signal. Calculates distance from object based on signal time and updates global variable ultrasonicDistance. |
| get_encoder_speed| void \*arg | void* | calculates linear velocity of one wheel and updates global variable 'encoderSpeedLeft/Right' |
| follow_line | void \*arg | void* | adjusts speed of left motor if line is crossed |
