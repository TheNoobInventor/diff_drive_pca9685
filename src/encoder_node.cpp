//    The encoder node subscribes to topics /right_motor_dir and /left_motor_dir in order to 
//    publish to the /ticks topic which contains message fields for right and left motor pulses/ticks.

#include <iostream>
#include <csignal>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <wiringPi.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "diff_drive_pca9685_interfaces/msg/tick.hpp"

using namespace std;
using std::placeholders::_1;

// Default wheel directions
string left_wheel_direction = "forward";
string right_wheel_direction = "forward";

// GPIO PINS
#define LEFT_WHL_ENCODER 25
#define RIGHT_WHL_ENCODER 24

// Pulse counters
int left_wheel_pulse_count = 0;
int right_wheel_pulse_count = 0;

// Left wheel callback function
void left_wheel_pulse(void){

    if (left_wheel_direction == "forward")
    {
        left_wheel_pulse_count++;
    } else {
        left_wheel_pulse_count--;
    }
    cout << "Left pulse: " << left_wheel_pulse_count << endl;
}

// Right wheel callback function
void right_wheel_pulse(void){

    if (right_wheel_direction == "forward")
    {
        right_wheel_pulse_count++;
    } else {
        right_wheel_pulse_count--;
    }
    cout << "Right pulse: " << right_wheel_pulse_count << endl;
}

// Handles CTRL+C to shutdown the program 
void handler(int sig){

    // Shutdown ROS C++ client
    rclcpp::shutdown();

    // Shutdown program
    exit(sig);
}

// Encoder class
class Encoder : public rclcpp::Node
{
    public:
        Encoder() : Node("encoder_node")
        {   
            RCLCPP_INFO(this->get_logger(), "is initialized");

            // Create subscription to /right_motor_dir topic
            right_dir_sub = this->create_subscription<std_msgs::msg::String>("right_motor_dir", 1, 
            std::bind(&Encoder::right_dir_callback, this, _1));

            // Create subscription to /left_motor_dir topic
            left_dir_sub = this->create_subscription<std_msgs::msg::String>("left_motor_dir", 1, 
            std::bind(&Encoder::left_dir_callback, this, _1));

            // Create a publisher for the /ticks topic, which holds both left and right ticks, of type Tick
            ticks_pub = this->create_publisher<diff_drive_pca9685_interfaces::msg::Tick>("ticks", 1);

            // Create timer that runs every 50ms
            timer = this->create_wall_timer(50ms, std::bind(&Encoder::timer_callback, this));  
        }

    private:
        void right_dir_callback(const std_msgs::msg::String::SharedPtr msg) const{
            // Assign the right wheel direction obtained from the /right_motor_dir topic
            right_wheel_direction = msg->data.c_str();
        }

        void left_dir_callback(const std_msgs::msg::String::SharedPtr msg) const{
            //Assign the left wheel direction obtained from the /left_motor_dir topic
            left_wheel_direction = msg->data.c_str();
        }

        // This function executes every 0.05 seconds to assign the wheel pulse counts to a tick 
        // message and publishes this message to the /ticks topic.
        void timer_callback()
        {
            // Initialize tick message
            auto ticks = diff_drive_pca9685_interfaces::msg::Tick();

            // Assign wheel pulse counts to tick message
            ticks.right_tick = right_wheel_pulse_count;
            ticks.left_tick = left_wheel_pulse_count;

            // Publish tick message to /ticks topic
            ticks_pub->publish(ticks);
        }

        // Declare timer, publisher and subscribers
        rclcpp::TimerBase::SharedPtr timer;
        rclcpp::Publisher<diff_drive_pca9685_interfaces::msg::Tick>::SharedPtr ticks_pub;
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr right_dir_sub;
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr left_dir_sub;
};

int main(int argc, char * argv[])
{
    // Initialize wiringPi using GPIO BCM pin numbers
    wiringPiSetupGpio();

    // Setup GPIO encoder pins
    pinMode(LEFT_WHL_ENCODER, INPUT);
    pinMode(RIGHT_WHL_ENCODER, INPUT);

    // Setup pull up resistors on encoder pins
    pullUpDnControl(LEFT_WHL_ENCODER, PUD_UP);
    pullUpDnControl(RIGHT_WHL_ENCODER, PUD_UP);

    // Initialize encoder interrupts for falling signal states
    wiringPiISR(LEFT_WHL_ENCODER, INT_EDGE_FALLING, left_wheel_pulse);
    wiringPiISR(RIGHT_WHL_ENCODER, INT_EDGE_FALLING, right_wheel_pulse);

    // Initialize signal handler for Ctrl+C exception handling
    signal(SIGINT, handler);

    // Initialize ROS C++ client
    rclcpp::init(argc, argv);

    // Processing node data
    rclcpp::spin(std::make_shared<Encoder>());

    return 0;
}