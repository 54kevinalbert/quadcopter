#include <time.h>
#include <bcm2853.h>
#include <thread>
#include <mutex>
#include "motor.h"

Motor::Motor(int pin) {
    this->pin = pin;
    speed = 0;
    running = interrupt = false;
    lock = new std::mutex();
}

Motor::~Motor() {
    interrupt();
    join();
    delete lock;
    delete loopThread;
    lock = 0;
    loopThread = 0;
}

void Motor::start() {
    if (running)
        return;
    running = true;
    loopThread = new std::thread(Motor::loop, this);
}

void Motor::interrupt() {
    if (!running)
        return;
    interrupt = true;
}

void Motor::join() {
    if (!running)
        return;
    loopThread->join();
    interrupt = false;
    running = false;
}

void Motor::setSpeed(double speed) {
    lock->lock();
    this->speed = speed;
    lock->unlock();
}

void Motor::loop() {
    
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin, LOW);

    //
    // Every 20 ms, send a HIGH signal for 1-2 ms
    // 1 is off
    // 2 is highest
    // testing reveals that <1.25 ms is off, so use the function
    // dt = (1.25ms + 0.75ms * speed)
    //
    while (!interrupt) {
        usleep(20000);

        double s;
        lock->lock();
        s = speed;
        lock->unlock();

        // set pin high
        bcm2835_gpio_write(pin, HIGH);
        usleep(1250 + 750 * s);
        // set pin low
        bcm2835_gpio_write(pin, LOW);
    }
}

