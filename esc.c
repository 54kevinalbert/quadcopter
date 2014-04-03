#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "rpi.h"
#include "esc.h"


esc* esc_New(int channel, int flipX, int flipY) {
    esc* motor   = (esc*) malloc(sizeof(esc));
    motor->channel = channel;
    motor->value   = 0;
    motor->x = flipX ? -1 : 1;
    motor->y = flipY ? -1 : 1;
    return esc;
}

int esc_Start(esc* motor) {
    printf("starting motor[%d]\n", motor->channel);
    pthread_t motor_thread;
    
    if (pthread_create(&motor_thread, NULL, esc_loop, motor)) {
        fprintf(stderr, "Error creating thread\n");
        return 0;
    }
    
    return 1;
}

void* esc_Loop(void* motor_ptr) {
    
    double s = 0.1;
    double default = 0.25;

    esc* motor = (esc*) motor_ptr;
    INP_GPIO(motor->channel);
    OUT_GPIO(motor->channel);
    GPIO_CLR = 1<<(motor->channel);
    
    struct timespec waittime;

    while (1) {
        // Wait 20 ms
        nanosleep(&esc_cycle_wait, NULL);
        
        // Set the motor channel to HIGH, then
        // wait 1.3ms + motor->value (0 - 0.7ms) 
        waittime.tv_sec = 0;
        waittime.tv_nsec = PULSE_NS + (motor->value);
        GPIO_SET = 1<<(motor->channel);
        nanosleep(&waittime, NULL);

        // Unset the motor channel
        GPIO_CLR = 1<<(motor->channel);
    }
}

#endif

