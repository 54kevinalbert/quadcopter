/**
 * Control brushless motors via electronic speed controller
 */

#ifndef _esc_h
#define _esc_h

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "rpi.h"

#define CYCLE_NS            20000000
#define PULSE_NS            1300000
#define MOTOR_TOP_LEFT      4
#define MOTOR_TOP_RIGHT     14
#define MOTOR_BOTTOM_LEFT   17
#define MOTOR_BOTTOM_RIGHT  18
#define ESC_MAX             700000

typedef struct {
    int value;
    int channel;
} esc_output;

int setup_esc();
void *esc_loop(void*);
int init_motor(esc_output* motor);

struct timespec esc_cycle_wait;

#endif
