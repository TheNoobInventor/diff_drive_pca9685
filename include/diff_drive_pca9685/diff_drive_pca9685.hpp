#ifndef _DIFF_DRIVE_PCA9685_H_
#define _DIFF_DRIVE_PCA9685_H_

#include <string>
#include <thread> // for motor start and stop test
// #include <dos.h> // for motor start and stop tests

#include "rclcpp/rclcpp.hpp"

#include "hardware_interface/base_interface.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "hardware_interface/types/hardware_interface_status_values.hpp"

#include "motor_encoder.h"
#include "config.hpp"
#include "wheel.hpp"
#include "diff_drive_pca9685.hpp"

using hardware_interface::return_type;

class DiffDrivePCA9685 : public hardware_interface::BaseInterface<hardware_interface::SystemInterface>
{
  public:
    DiffDrivePCA9685();

    return_type configure(const hardware_interface::HardwareInfo & info) override;

    std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

    std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

    return_type start() override;

    return_type stop() override;

    return_type read() override;

    return_type write() override;

  private:
    Config config;

    Wheel left_wheel;
    Wheel right_wheel;

    rclcpp::Logger logger;

    std::chrono::time_point<std::chrono::system_clock> time;
};

#endif