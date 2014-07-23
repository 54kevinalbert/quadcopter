#include "gyro.h"
#include "motor.h"
#include "quadcopter.h"

void QuadCopter::start() {
    motors = new (Motor())[4];
    std::cout << "start\n";
    Gyro::start();
    for (int i = 0; i < 100; i++) {
        usleep(100000);
        Gyro::read();
    }
}

void QuadCopter::cmd(char c) {
    Gyro::stop();
    std::cout << "stop";
}

