cmake_minimum_required(VERSION 3.5)
project(diff_drive_pca9685)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(controller_manager REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)

add_library(diff_drive_pca9685 SHARED src/diff_drive_pca9685.cpp src/DEV_Config.c src/PCA9685.c src/MotorDriver.c src/motor_encoder.c)

# Include C/Cpp "include" directory
include_directories(include)

# Create Cpp executable
add_executable(motor_encoder src/DEV_Config.c src/PCA9685.c src/MotorDriver.c src/motor_encoder.c)

# Install Cpp executables
# Use lines below as a guide
install(TARGETS
  motor_encoder
  DESTINATION lib/${PROJECT_NAME}
)

# Added math, wiringPi and other libraries
target_link_libraries(motor_encoder m wiringPi pthread crypt rt)

target_include_directories(diff_drive_pca9685 PRIVATE include)

ament_target_dependencies(
  diff_drive_pca9685
  hardware_interface
  controller_manager
  pluginlib
  rclcpp
  rclcpp_lifecycle
)

pluginlib_export_plugin_description_file(hardware_interface diff_drive_pca9685.xml)

install(
  DIRECTORY include/
  DESTINATION include/diff_drive_pca9685
)

install(
  TARGETS diff_drive_pca9685
  DESTINATION lib/${PROJECT_NAME}
)

# Sort out
# if(BUILD_TESTING)
#   find_package(ament_cmake_mock REQUIRED)
#   find_package(hardware_interface REQUIRED)
# endif()

ament_export_libraries(diff_drive_pca9685)

ament_package()