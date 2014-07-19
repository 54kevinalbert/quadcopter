#include "rpi.h"
#include "mpu6050.h"

void mpu_Init(void) {
    mpu_SetRegister(PWR_MGMT_1, 0x00);    // Clear sleep bit
    mpu_SetRegister(CONFIG, 0x00);
    mpu_SetRegister(GYRO_CONFIG, 0x08);
    mpu_SetRegister(ACCEL_CONFIG, 0x08);
}

void mpu_SetRegister(unsigned char regAddr, unsigned char regValue) {
    // See datasheet (PS) page 36: Single Byte Write Sequence
    
    // Master:   S  AD+W       RA       DATA       P
    // Slave :            ACK      ACK        ACK
    
    BSC1_A = MPU6050_ADDR;
    
    BSC1_DLEN = 2;
    BSC1_FIFO = (unsigned char)regAddr;
    BSC1_FIFO = (unsigned char)regValue;
    
    BSC1_S = CLEAR_STATUS;  // Reset status bits (see #define)
    BSC1_C = START_WRITE;   // Start Write (see #define)
    
    i2c_Sync();
}

void mpu_Read(short * accData, short * gyrData) {
    // See datasheet (PS) page 37: Burst Byte Read Sequence
    
    // Master:   S  AD+W       RA       S  AD+R           ACK        NACK  P
    // Slave :            ACK      ACK          ACK DATA       DATA
    
    BSC1_DLEN = 1;          // one byte
    BSC1_FIFO = 0x3B;       // value of first register
    BSC1_S = CLEAR_STATUS;  // Reset status bits (see #define)
    BSC1_C = START_WRITE;   // Start Write (see #define)
    
    i2c_Sync();
    
    BSC1_DLEN = 14;
    
    BSC1_S = CLEAR_STATUS;  // Reset status bits (see #define)
    BSC1_C = START_READ;    // Start Read after clearing FIFO (see #define)
    
    i2c_Sync();
    
    short tmp;
    
    // Accelerometer
    int i = 0;
    for(i; i < 3; i++) {
        tmp         = BSC1_FIFO << 8;
        tmp        += BSC1_FIFO;
        accData[i]  = tmp;
    }
    
    // Temperature
    tmp  = BSC1_FIFO << 8;
    tmp += BSC1_FIFO;
    
    // Gyroscope
    i = 0;
    for(i; i < 3; i++) {
        tmp         = BSC1_FIFO << 8;
        tmp        += BSC1_FIFO;
        gyrData[i]  = tmp; 
    }
}
