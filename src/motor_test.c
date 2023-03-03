#include "diff_drive_pca9685/motor_test.h"

void handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Motor Stop\r\n");
    Motor_Stop(MOTORA);
    Motor_Stop(MOTORB);
    DEV_ModuleExit();

    exit(0);
}

int main(void)
{
    //1.System Initialization
    if(DEV_ModuleInit())
        exit(0);
    
    //2.Motor Initialization
    Motor_Init();

    printf("Motor_Run\r\n");
    Motor_Run(MOTORA, FORWARD, 50);
    Motor_Run(MOTORB, BACKWARD, 50);

    // Exception handling:ctrl + c
    signal(SIGINT, handler);

    // Keeps program running for the next interrupt
    while(1) {}

    //3.System Exit
    DEV_ModuleExit();
    return 0;
}



