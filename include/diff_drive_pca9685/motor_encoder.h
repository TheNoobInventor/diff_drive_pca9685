#ifdef __cplusplus
extern "C" {
#endif

#ifndef __MOTOR_ENCODER_H
#define __MOTOR_ENCODER_H__

#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <signal.h>

#include <time.h>
#include "DEV_Config.h"
#include "MotorDriver.h"

void set_motor_speeds();
void read_encoder_values(int left_encoder_value, int right_encoder_value);
void calculate_encoder_angle();

#endif 

#ifdef __cplusplus
}
#endif