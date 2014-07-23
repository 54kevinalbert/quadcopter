

#include "gyro.h"
#include "motor.h"
#include "quadcopter.h"


public static void QuadCopter::start() {


    interrupt = false;

    motors[0] = new Motor(15);
    motors[1] = new Motor(18);
    motors[2] = new Motor(23);
    motors[3] = new Motor(24);

    std::cout << "start\n";
    Gyro::start();
    for (int i = 0; i < 100; i++) {
        usleep(100000);
        Gyro::read();
    }

    // Start the loop!
    loopThread = new std::thread(QuadCopter::tick);
}

public static void QuadCopter::do(char c) {
    Gyro::stop();
    std::cout << "stop";
}

public static void QuadCopter::stop() {
    delete[] motors;
    interrupt = true;
    loopThread->join();
}

