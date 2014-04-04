#include <time.h>
#include <pthread.h>
#include "rpi.h"
#include "cloudkicker.h"

#define TRIGGER 4
#define ECHO    10


int main(void) {

    printf("mapping peripherals\n");
    if (map_peripheral(&gpio) == -1) {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }

    // Use inverted IO mapping because TRIGGER
    // and ECHO are both run through transistors    
    INP_GPIO(TRIGGER);
    OUT_GPIO(TRIGGER);
    INP_GPIO(ECHO);
    
    
    struct timespec 
        pulse   = {0, 10000},    //    0.10 ms
        wait    = {0, 5000},     //    0.05 ms
        loop    = {0, 500000000}; // 500.00 ms
    
    GPIO_SET = 1 << TRIGGER;
    
    printf("sleeping\n");
    nanosleep(&loop, NULL);
    
    while(1) {
    
        printf("looping\n");

        // Set the trigger and wait 0.1 ms
        GPIO_CLR = 1 << TRIGGER;
        nanosleep(&pulse, NULL);
        GPIO_SET = 1 << TRIGGER;
        
        // Record the current time, wait for an echo
        struct timespec tstart={0,0}, tend={0,0};
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        while (GPIO_READ(ECHO))
            nanosleep(&wait, NULL);
        
        // Get the end time and calculate the difference
        clock_gettime(CLOCK_MONOTONIC, &tend);
        printf("calculating results\n");
        int usecs = (tend.tv_sec - tstart.tv_sec) * 1000000 +
                    (tend.tv_nsec - tstart.tv_nsec) / 1000;

        // Convert to meters
        printf("saving results\n");
        int r = (340.29 * usecs) / 1000000;
        printf("%f meters\n", r);
        nanosleep(&loop, NULL);
    }
}

