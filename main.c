/* 
 * File:   main.c
 * Author: kevin
 *
 * Created on April 3, 2014, 2:54 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include "rpi.h"
#include "mpu6050.h"


#define ACCELEROMETER_SENSITIVITY 8192.0
#define GYROSCOPE_SENSITIVITY 65.536

#define filterConstant 0.10             // Complementary filter

void ComplementaryFilter(short accData[3], short gyrData[3], float *pitch, float *roll);    

int main(int argc, char *argv[])
{
    // Map memory areas         
    if(map_peripheral(&gpio) == -1) {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }
    if(map_peripheral(&bsc1) == -1) {
        printf("Failed to map the physical BSC1 (I2C) registers into the virtual memory space.\n");
        return -1;
    }

    /* BSC0 is on GPIO 0 & 1 */
    i2c_Init();

    mpu_Init();
    printf("MPU6050 initialized.\n");
 
    // Global variables
    short accData[3], gyrData[3];
    short tmp;
    float pitch = 0, roll = 0;
    float pitchOnlyAcc = 0, pitchOnlyGyr = 0;
    float time = 0;
    FILE *fileHandle;
    struct timespec tp;
    long startTime, procesTime;


    while(1) {
        // Get current time
        clock_gettime(CLOCK_REALTIME, &tp);
        startTime = tp.tv_sec*1000000000 + tp.tv_nsec;

        // Read MPU6050 sensor
        mpu_Read(&accData[0], &gyrData[0]);

        // Switch Axes
        tmp = accData[2];
        accData[2] = -tmp;
        tmp = accData[0];
        accData[0] = -tmp;
     
        // Calculate the pitch and roll with the complementary filter
        ComplementaryFilter(&accData[0], &gyrData[0], &pitch, &roll);   

        // Calculate pitch and roll with only accelerometer and gyro for plotting reasons
        pitchOnlyGyr += (float)gyrData[0] / 6553.6;     // Angle around the X-axis
        pitchOnlyAcc = atan2f((float)accData[1], (float)accData[2]) * 180 / M_PI;

        printf("Accelerometer X: %f\n", (float)accData[0]/ACCELEROMETER_SENSITIVITY);
        printf("              Y: %f\n", (float)accData[1]/ACCELEROMETER_SENSITIVITY);
        printf("              Z: %f\n", (float)accData[2]/ACCELEROMETER_SENSITIVITY);

        printf("Gyroscope    RX: %f\n", (float)gyrData[0]/GYROSCOPE_SENSITIVITY);
        printf("             RY: %f\n", (float)gyrData[1]/GYROSCOPE_SENSITIVITY);
        printf("             RZ: %f\n", (float)gyrData[2]/GYROSCOPE_SENSITIVITY);

        printf("Complementary Filter");     
        printf("          Pitch: %f\n", pitch);
        printf("           Roll: %f\n", roll);
        printf("\n\n");
     

        time += 0.01;

        struct timespec waittime = {1 , 0};
        nanosleep(&waittime, 0);
    }
}

void ComplementaryFilter(short accData[3], short gyrData[3], float *pitch, float *roll)
{
    float pitchAcc, rollAcc;               

    // Integrate the gyroscope data -> int(angularSpeed) = angle
    *pitch += (float)gyrData[0] / 6553.6;   // Angle around the X-axis
    *roll -= (float)gyrData[1] / 6553.6;    // Angle around the Y-axis

    // Compensate for drift with accelerometer data if
    // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
    int forceMagnitudeApprox = abs(accData[0]) + abs(accData[1]) + abs(accData[2]);
    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768) {
        // Turning around the X axis results in a vector on the Y-axis
        pitchAcc = atan2f((float)accData[1], (float)accData[2]) * 180 / M_PI;
        *pitch = *pitch * (1 - filterConstant) + pitchAcc * filterConstant;

        // Turning around the Y axis results in a vector on the X-axis
        rollAcc = atan2f((float)accData[0], (float)accData[2]) * 180 / M_PI;
        *roll = *roll * (1 - filterConstant) + rollAcc * filterConstant;
    }
} 
