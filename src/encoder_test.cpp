// This program counts the number of wheel encoder ticks/pulses in the forward or backward direction, not both, 
// as the wheel(s) is/are turned and prints the pulses in the terminal.

#include <iostream>
#include <csignal>
#include <wiringPi.h>

using namespace std;

// GPIO PINS
#define LEFT_WHL_ENCODER 25
#define RIGHT_WHL_ENCODER 24

// Pulse counters
int left_wheel_pulse_count = 0;
int right_wheel_pulse_count = 0;

// Left wheel callback function
void left_wheel_pulse(void){
    left_wheel_pulse_count++;
    cout << "Left pulse: " << left_wheel_pulse_count << endl;
}

// Right wheel callback function
void right_wheel_pulse(void){
    right_wheel_pulse_count++;
    cout << "Right pulse: " << right_wheel_pulse_count << endl;
}

// Handles CTRL+C to shutdown the program 
void handler(int sig){
    exit(0);
}

int main()
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
    
    while(1) {

    }
}