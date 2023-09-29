#ifdef __cplusplus
extern "C" {
#endif

#ifndef __MOTOR_ENCODER_H
#define __MOTOR_ENCODER_H__

#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <signal.h>
#include <wiringPi.h>

#include "MotorDriver.h"

#define LEFT_WHL_ENCODER 25
#define RIGHT_WHL_ENCODER 24

void handler(int signo);
void left_wheel_pulse();
void right_wheel_pulse();
void set_motor_speeds(double left_wheel_command, double right_wheel_command);
void read_encoder_values(int *left_encoder_value, int *right_encoder_value);

#endif 

#ifdef __cplusplus
}
#endif