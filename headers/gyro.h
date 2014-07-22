#pragma once

#include <time.h>

class Gyro {
    public:
        static void start();
        static void stop();
        static double getPitch();
        static double getRoll();
        static double getZ();
        static void read();
        //const float ACCELEROMETER_SENSITIVITY 8192.0;
        static const double GYROSCOPE_SENSITIVITY = 655360.0;
    private:
        static short accData[3];
        static short gyrData[3];
        static double pitch;
        static double roll;
        static double z;
        static struct timespec tthen;
};

