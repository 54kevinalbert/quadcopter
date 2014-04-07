#include <time.h>
#include <pthread.h>
#include "rpi.h"

#define TRIGGER 27
#define ECHO    22

int main(void) {

    if (map_peripheral(&gpio) == -1) {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }
    
    INP_GPIO(TRIGGER);
    OUT_GPIO(TRIGGER);
    INP_GPIO(ECHO);
    
    
    struct timespec 
        pulse   = {0, 10000},     // 0.10 ms
        wait    = {0, 5000},      // 0.05 ms
        loop    = {0, 25000000};  // 25.00 ms
    
    GPIO_CLR = 1 << TRIGGER;
    
    printf("sleeping\n");
    nanosleep(&loop, NULL);


    while(1) {

        // Set the trigger and wait 0.1 ms
        GPIO_SET = 1 << TRIGGER;
        nanosleep(&pulse, NULL);
        GPIO_CLR = 1 << TRIGGER;
        
        // Record the current time, wait for an echo
        struct timespec tstart={0,0}, tend={0,0};
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        while (!GPIO_READ(ECHO))
            nanosleep(&wait, NULL);
        
        // Get the end time and calculate the difference
        clock_gettime(CLOCK_MONOTONIC, &tend);
        int usecs = (tend.tv_sec - tstart.tv_sec) * 1000000 +
                    (tend.tv_nsec - tstart.tv_nsec) / 1000;

        // Convert to meters
        double r = (340.29 * (double) usecs) / 1000000;
        printf("%f meters\n", r);
        nanosleep(&loop, NULL);
    }
}

