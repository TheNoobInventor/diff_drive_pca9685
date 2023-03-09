#ifndef _DIFF_DRIVE_PCA9685_CONFIG_H_
#define _DIFF_DRIVE_PCA9685_CONFIG_H_

#include <string>

struct Config
{
    std::string left_wheel_name = "left_wheel";
    std::string right_wheel_name = "right_wheel";
    float loop_rate = 30;
    int enc_ticks_per_rev = 20;
};

#endif