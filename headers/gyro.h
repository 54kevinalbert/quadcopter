#ifndef _Gyro
#define _Gyro

#include <time.h>

class Gyro {
    public:
        Gyro();
        ~Gyro();
        double getPitch();
        double getRoll();
        double getZ();
        void read();
        //const float ACCELEROMETER_SENSITIVITY 8192.0;
        static const double GYROSCOPE_SENSITIVITY = 0.065536;
    private:
        short accData[3];
        short gyrData[3];
        double pitch;
        double roll;
        double z;
        struct timespec tthen;
};

#endif
