#ifndef _Gyro
#define _Gyro

#include <time.h>

class Gyro {
    public:
        static void start();
        static void stop();
        static double getPitch();
        static double getRoll();
        static double getZ();
        static void read();
    private:
        static short accData[3];
        static short gyrData[3];
        static double pitch;
        static double roll;
        static double z;
        static struct timespec tthen;
};

#endif
