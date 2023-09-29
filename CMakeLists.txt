cmake_minimum_required(VERSION 3.5)
project(diff_drive_pca9685)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(controller_manager REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)

# COMPILE
add_library(diff_drive_pca9685 SHARED 
          src/diff_drive_pca9685.cpp
          src/wheel.cpp
          src/MotorDriver.c 
          src/motor_encoder.c
          src/PCA9685.c 
          src/DEV_Config.c 
)

target_include_directories(diff_drive_pca9685 PRIVATE include)

ament_target_dependencies(
  diff_drive_pca9685
  hardware_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
)

# Link WiringPi library to diff_drive_pca9685 target
target_link_libraries(diff_drive_pca9685 wiringPi)

# Export hardware plugin
pluginlib_export_plugin_description_file(hardware_interface diff_drive_pca9685.xml)

# INSTALL
install(
  DIRECTORY include/
  DESTINATION include/diff_drive_pca9685
)

install(
  TARGETS diff_drive_pca9685
  DESTINATION share/${PROJECT_NAME}
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

# Create Cpp executable
# add_executable(motor_encoder src/DEV_Config.c src/PCA9685.c src/MotorDriver.c src/motor_encoder.c)

# Install Cpp executables
# Use lines below as a guide
# install(TARGETS
#   motor_encoder
#   DESTINATION lib/${PROJECT_NAME}
# )

# # Added math, wiringPi and other libraries
# target_link_libraries(motor_encoder m wiringPi pthread crypt rt)


# EXPORTS
ament_export_libraries(diff_drive_pca9685)

ament_package()