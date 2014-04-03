
#ifndef _esc_h
#define _esc_h

#include <time.h>

#define CYCLE_NS            20000000
#define PULSE_NS            1300000
#define MOTOR_TOP_LEFT      4
#define MOTOR_TOP_RIGHT     14
#define MOTOR_BOTTOM_LEFT   17
#define MOTOR_BOTTOM_RIGHT  18
#define ESC_MAX             700000

typedef struct {
    unsigned int value;
    short channel;
    short flipX;
    short flipY;
} esc;


esc* esc_New(short, short, short);
int esc_Start(esc*);
void* esc_Loop(void*);

struct timespec ESC_CYCLE_WAIT {0, CYCLE_NS};

#endif
