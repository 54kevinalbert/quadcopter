#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "rpi.h"
#include "esc.h"

//
// 
// TL   TR
//   \ /
//   / \
// BL   BR
// 

int setup_esc() {
    esc_cycle_wait.tv_sec = 0;
    esc_cycle_wait.tv_nsec = CYCLE_NS;

    esc_output TL = {0, MOTOR_TOP_LEFT};
    esc_output TR = {0, MOTOR_TOP_RIGHT};
    esc_output BL = {0, MOTOR_BOTTOM_LEFT};
    esc_output BR = {0, MOTOR_BOTTOM_LEFT};
    
    if (!init_motor(&TL) || !init_motor(&TR) ||
        !init_motor(&BL) || !init_motor(&BR))
    {
        printf("Unable to initialize motor");
        return -1;
    }

    return 0;
}

int init_motor(esc_output* motor) {
    printf("starting motor[%d]\n", motor->channel);
    pthread_t motor_thread;
    
    if (pthread_create(&motor_thread, NULL, esc_loop, motor)) {
        fprintf(stderr, "Error creating thread\n");
        return 0;
    }
    
    return 1;
}

#ifndef __APPLE__

void *esc_loop(void* motor_ptr) {
    
    double s = 0.1;
    double default = 0.25;

    esc_output* motor = (esc_output*) motor_ptr;
    INP_GPIO(motor->channel);
    OUT_GPIO(motor->channel);
    GPIO_CLR = 1<<(motor->channel);
    
    while (1) {
        nanosleep(&esc_cycle_wait, NULL);
        if (!state.on) continue;

        double y = -1 * state->Y;
        double x = -1 * state->X;
        double z = -1 * state->Z;
        
        if (motor->channel == MOTOR_BOTTOM_LEFT || motor->channel == MOTOR_BOTTOM_RIGHT)
            y = 1-y;
        
        if (motor->channel == MOTOR_TOP_LEFT || motor->channel == MOTOR_BOTTOM_LEFT)
            x = 1-x;
        
        double v = x * y * z;
        v = v < 0 ? 0 : v > 1 ? 1 : v;

        // mix it with the default value
        v = default * (1-s) + v * s;
        motor->value = ESC_MAX * v;
   
        struct timespec waittime;
        waittime.tv_sec = 0;
        waittime.tv_nsec = PULSE_NS + (motor->value);
        GPIO_SET = 1<<(motor->channel);
        nanosleep(&waittime, NULL);
        GPIO_CLR = 1<<(motor->channel);
    }
}

#else

void *esc_loop(void* motor_ptr) {
    printf("OSX esc_loop\n");
    return 0;
}

#endif

