#include <iostream>
#include <math.h>
#include <time.h>
extern "C" {
  #include <bcm2835.h>
}
#include "conf.h"
#include "gyro.h"

inline int abs(short v) {
    return v < 0 ? v * -1 : v;
}

inline double absd(double v) {
    return v < 0 ? v * -1 : v;
}

short Gyro::accData[3];
short Gyro::gyrData[3];
double Gyro::pitch;
double Gyro::roll;
double Gyro::z;
struct timespec Gyro::tthen;

void Gyro::start() {
    clock_gettime(CLOCK_REALTIME, &tthen);

    // initialize pitch and roll with the accelerometer data
    bcm2835_init();
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(0x68);

    char buf[14];
    char regaddr[2];
    int ret;

    //disable sleep mode!!!!!
    regaddr[0] = 107;
    regaddr[1] = 0;

    //This is the basic operation to write to a register
    //regaddr[0] is the register address
    //regaddr[1] is the value
    bcm2835_i2c_write(regaddr, 2);

    regaddr[0] = 59;
    ret = BCM2835_I2C_REASON_ERROR_DATA;
    while (ret != BCM2835_I2C_REASON_OK) {
        //This is the basic operation to read a register
        //regaddr[0] is the register address
        //buf[0] is the value
        bcm2835_i2c_write(regaddr, 1);
        ret = bcm2835_i2c_read(buf, 14);
    }

    regaddr[0] = 60;
    ret = BCM2835_I2C_REASON_ERROR_DATA;
    while (buf[0] == 99) {
        bcm2835_i2c_write(regaddr, 1);
        ret = bcm2835_i2c_read(buf, 1);
    }

    for (int i = 0; i < 3; i++) {
        accData[i] = buf[i<<1] + (buf[i<<1] << 8);
        if (accData[i] & 1<<15)
            accData[i] -= 1<<16;
    }
    pitch = (double) atan((double)accData[1] / (double)accData[2]);// * 180 / M_PI;
    roll =  (double) atan((double)accData[0] / (double)accData[2]);// * 180 / M_PI;
}

void Gyro::stop() {
    bcm2835_i2c_end();
}

double Gyro::getPitch() {
    return pitch;
}

double Gyro::getRoll() {
    return roll;
}

double Gyro::getZ() {
    return z;
}

void Gyro::read() {

    // find deltaT
    struct timespec tnow;
    clock_gettime(CLOCK_REALTIME, &tnow);
    double t = tnow.tv_sec - tthen.tv_sec;
    t += (tnow.tv_sec - tthen.tv_nsec) / 1000000000L;
    tthen.tv_sec = tnow.tv_sec;
    tthen.tv_nsec = tnow.tv_nsec;

    char buf[14];
    char regaddr[2];
    int ret;

    //disable sleep mode!!!!!
    regaddr[0] = 107;
    regaddr[1] = 0;

    //This is the basic operation to write to a register
    //regaddr[0] is the register address
    //regaddr[1] is the value
    bcm2835_i2c_write(regaddr, 2);

    regaddr[0] = 59;
    ret = BCM2835_I2C_REASON_ERROR_DATA;
    while (ret != BCM2835_I2C_REASON_OK) {
        //This is the basic operation to read a register
        //regaddr[0] is the register address
        //buf[0] is the value
        bcm2835_i2c_write(regaddr, 1);
        ret = bcm2835_i2c_read(buf, 14);
    }

    regaddr[0] = 60;
    ret = BCM2835_I2C_REASON_ERROR_DATA;
    while (buf[0] == 99) {
        bcm2835_i2c_write(regaddr, 1);
        ret = bcm2835_i2c_read(buf, 1);
    }

    int i;
    for (i = 0; i < 3; i++) {
        int j = i + 4;
        accData[i] = buf[i<<1] + (buf[i<<1] << 8);
        gyrData[i] = buf[j<<1] + (buf[j<<1] << 8);
        if (accData[i] & 1<<15)
            accData[i] -= 1<<16;
        if (gyrData[i] & 1<<15)
            gyrData[i] -= 1<<16;
    }

    pitch += (double)gyrData[0] * Conf::GyroAcceleration * t;
    roll  -= (double)gyrData[1] * Conf::GyroAcceleration * t;

    int forceMagnitudeApprox = abs(accData[0]) + abs(accData[1]) + abs(accData[2]);
    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768) {
        pitch = 0.98 * pitch + 0.2 * (double) atan((double)accData[1] / (double)accData[2]);
        roll  = 0.98 * roll  + 0.2 * (double) atan((double)accData[0] / (double)accData[2]);

        double cosP = cos(pitch);
        double cosR = cos(roll);
        z = (double) accData[0]*(1-cosR) +
            (double) accData[1]*(1-cosP) +
            (double) accData[2]*cosP*cosR;
        z /= 16448;
    }
} 
