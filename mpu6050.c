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
    
    BSC0_A = MPU6050_ADDR;
    
    BSC0_DLEN = 2;
    BSC0_FIFO = (unsigned char)regAddr;
    BSC0_FIFO = (unsigned char)regValue;
    
    BSC0_S = CLEAR_STATUS;  // Reset status bits (see #define)
    BSC0_C = START_WRITE;   // Start Write (see #define)
    
    i2c_Sync();
}

void mpu_Read(short * accData, short * gyrData) {
    // See datasheet (PS) page 37: Burst Byte Read Sequence
    
    // Master:   S  AD+W       RA       S  AD+R           ACK        NACK  P
    // Slave :            ACK      ACK          ACK DATA       DATA
    
    BSC0_DLEN = 1;          // one byte
    BSC0_FIFO = 0x3B;       // value of first register
    BSC0_S = CLEAR_STATUS;  // Reset status bits (see #define)
    BSC0_C = START_WRITE;   // Start Write (see #define)
    
    i2c_Sync();
    
    BSC0_DLEN = 14;
    
    BSC0_S = CLEAR_STATUS;  // Reset status bits (see #define)
    BSC0_C = START_READ;    // Start Read after clearing FIFO (see #define)
    
    i2c_Sync();
    
    short tmp;
    
    // Accelerometer
    int i = 0;
    for(i; i < 3; i++) {
        tmp         = BSC0_FIFO << 8;
        tmp        += BSC0_FIFO;
        accData[i]  = tmp;
    }
    
    // Temperature
    tmp  = BSC0_FIFO << 8;
    tmp += BSC0_FIFO;
    
    // Gyroscope
    i = 0;
    for(i; i < 3; i++) {
        tmp         = BSC0_FIFO << 8;
        tmp        += BSC0_FIFO;
        gyrData[i]  = tmp; 
    }
}
