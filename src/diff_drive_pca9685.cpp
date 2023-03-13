#include "diff_drive_pca9685/diff_drive_pca9685.hpp"

#include "hardware_interface/types/hardware_interface_type_values.hpp"

DiffDrivePCA9685::DiffDrivePCA9685()
    : logger(rclcpp::get_logger("DiffDrivePCA9685"))
{}

return_type DiffDrivePCA9685::configure(const hardware_interface::HardwareInfo & info)
{
    if (configure_default(info) != return_type::OK)
    {
        return return_type::ERROR;
    }

    RCLCPP_INFO(logger, "Configuring...");

    time = std::chrono::system_clock::now();

    config.left_wheel_name = info_.hardware_parameters["left_wheel_name"];
    config.right_wheel_name = info_.hardware_parameters["right_wheel_name"];
    config.loop_rate = std::stof(info_.hardware_parameters["loop_rate"]);
    config.enc_ticks_per_rev = std::stoi(info_.hardware_parameters["enc_ticks_per_rev"]);

    // Set up weheels
    left_wheel.setup(config.left_wheel_name, config.enc_ticks_per_rev);
    right_wheel.setup(config.right_wheel_name, config.enc_ticks_per_rev);

    // MOTOR ENCODER SETUP?? //

    RCLCPP_INFO(logger, "Finished configuration");

    status_ = hardware_interface::status::CONFIGURED;
    return return_type::OK;
}

std::vector<hardware_interface::StateInterface> DiffDrivePCA9685::export_state_interfaces()
{
    // Set up a position and a velocity interface for each wheel

    std::vector<hardware_interface::StateInterface> state_interfaces;

    state_interfaces.emplace_back(hardware_interface::StateInterface(left_wheel.name, hardware_interface::HW_IF_VELOCITY, &left_wheel.velocity));
    state_interfaces.emplace_back(hardware_interface::StateInterface(left_wheel.name, hardware_interface::HW_IF_POSITION, &left_wheel.position));
    state_interfaces.emplace_back(hardware_interface::StateInterface(right_wheel.name, hardware_interface::HW_IF_VELOCITY, &right_wheel.velocity));
    state_interfaces.emplace_back(hardware_interface::StateInterface(right_wheel.name, hardware_interface::HW_IF_POSITION, &right_wheel.position));

    return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> DiffDrivePCA9685::export_command_interfaces()
{
    // Set up a velocity command for each wheel

    std::vector<hardware_interface::CommandInterface> command_interfaces;

    command_interfaces.emplace_back(hardware_interface::CommandInterface(left_wheel.name, hardware_interface::HW_IF_VELOCITY, &left_wheel.command));
    command_interfaces.emplace_back(hardware_interface::CommandInterface(right_wheel.name, hardware_interface::HW_IF_VELOCITY, &right_wheel.command));

    return command_interfaces;
}

return_type DiffDrivePCA9685::start()
{
    RCLCPP_INFO(logger, "Starting Controller...");
    
    //
    //
    // PID VALUES?

    status_ = hardware_interface::status::STARTED;

    return return_type::OK;

}

return_type DiffDrivePCA9685::stop()
{
    RCLCPP_INFO(logger, "Stopping Controller...");
    status_ = hardware_interface::status::STOPPED;

    return return_type::OK;
}

return_type DiffDrivePCA9685::read()
{
    // Calculate delta time
    auto new_time = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = new_time - time;
    double delta_time = diff.count();
    time = new_time;

    //
    read_encoder_values(left_wheel.encoder_ticks, right_wheel.encoder_ticks);

    // Calculate wheel positions and velocities
    double prev_position = left_wheel.position;
    left_wheel.position = left_wheel.calculate_encoder_angle();
    left_wheel.velocity = (left_wheel.position - prev_position) / delta_time;

    prev_position = right_wheel.position;
    right_wheel.position = right_wheel.calculate_encoder_angle();
    right_wheel.velocity = (right_wheel.position - prev_position) / delta_time;

    return return_type::OK;
}

return_type DiffDrivePCA9685::write()
{

    // Testing
    Motor_Run(MOTORA, FORWARD, 50);
    Motor_Run(MOTORB, BACKWARD, 50);
    delay(5000);
    // this_thread::sleep_for(std::chrono::milliseconds(5000));
    Motor_Stop(MOTORA);
    Motor_Stop(MOTORB);

    // Closed loop control incorporating loop rate

    return return_type::OK;
}

// Should we initialize wiring library and motor driver here? 
// How do we ensure that the motor_encoder main loop is running to drive the motors?

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(
    DiffDrivePCA9685,
    hardware_interface::SystemInterface
)