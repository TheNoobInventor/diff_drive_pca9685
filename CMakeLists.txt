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
# add_executable(motor_test src/DEV_Config.c src/PCA9685.c src/MotorDriver.c src/motor_encoder.c src/motor_test.cpp)

# Install Cpp executables
# Use lines below as a guide
install(TARGETS
  motor_encoder
  # motor_test
  DESTINATION lib/${PROJECT_NAME}
)

# Added math, wiringPi and other libraries
target_link_libraries(motor_encoder m wiringPi pthread crypt rt)
# target_link_libraries(motor_test m wiringPi pthread crypt rt)

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

# install(
#   DIRECTORY include
#   DESTINATION share/${PROJECT_NAME} # Might need to change the destination
# )

install(
  TARGETS diff_drive_pca9685
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_libraries(diff_drive_pca9685)

# if(BUILD_TESTING)
#   find_package(ament_lint_auto REQUIRED)
#   # the following line skips the linter which checks for copyrights
#   # uncomment the line when a copyright and license is not present in all source files
#   #set(ament_cmake_copyright_FOUND TRUE)
#   # the following line skips cpplint (only works in a git repo)
#   # uncomment the line when this package is not in a git repo
#   #set(ament_cmake_cpplint_FOUND TRUE)
#   ament_lint_auto_find_test_dependencies()
# endif()

ament_package()