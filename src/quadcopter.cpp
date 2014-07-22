

#include "gyro.h"
#include "motor.h"
#include "quadcopter.h"

public static void QuadCopter::start() {
    motors = new (Motor())[4];
    std::cout << "start\n";
    Gyro::start();
    for (int i = 0; i < 100; i++) {
        usleep(100000);
        Gyro::read();
    }
}

public static void QuadCopter::do(char c) {
    Gyro::stop();
    std::cout << "stop";
}

