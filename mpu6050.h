/* 
 * File:   mpu6050.h
 * Author: kevin
 *
 * Created on April 3, 2014, 3:09 PM
 */

#ifndef MPU6050_H
#define	MPU6050_H

#define MPU6050_ADDR    0b01101001
#define CONFIG          0x1A
#define ACCEL_CONFIG    0x1C
#define GYRO_CONFIG     0x1B
#define PWR_MGMT_1      0x6B

void mpu_Read(short * accData, short * gyrData);
void mpu_Init(void);
void mpu_SetRegister(unsigned char regAddr, unsigned char regValue);


#endif	/* MPU6050_H */

