#include <time.h>
#include "rpi.h"

struct bcm2835_peripheral gpio = {GPIO_BASE};
struct bcm2835_peripheral bsc0 = {BSC0_BASE};

int map_peripheral(struct bcm2835_peripheral *p) {
    // Open /dev/mem
    if ((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        printf("Failed to open /dev/mem, try checking permissions.\n");
        return -1;
    }
    
    p->map = mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE,
                  MAP_SHARED, p->mem_fd,p->addr_p);
    
    if (p->map == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    
    p->addr = (volatile unsigned int *)p->map;
    return 0;
}

void unmap_peripheral(struct bcm2835_peripheral *p) {
    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
}


// Initialize I2C
void i2c_init() {
    INP_GPIO(0);
    SET_GPIO_ALT(0, 0);
    INP_GPIO(1);
    SET_GPIO_ALT(1, 0);
}


// Function to wait for the I2C transaction to complete
void wait_i2c_done() {
    struct timespec wait_i2c_ts = {0, 1000000};
    
    int timeout = 50;
    while((!((BSC0_S) & BSC_S_DONE)) && --timeout)
        nanosleep(&wait_i2c_ts, NULL);

    if(timeout == 0)
        printf("Error: wait_i2c_done() timeout.\n");
}

long delta_T_Usecs(struct timespec* t) {
    struct timespec k;
    clock_gettime(CLOCK_REALTIME, &k);
    long v = (k.tv_sec - t->tv_sec) * 1000000 + (k.tv_nsec - t->tv_nsec) / 1000;
    t->tv_sec = k.tv_sec;
    t->tv_nsec = k.tv_nsec;
    return v;
    
}
