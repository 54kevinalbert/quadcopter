#include <iostream>

#include "conf.h"
#include "gyro.h"
#include "motor.h"
#include "quadcopter.h"

std::thread* QuadCopter::loopThread;
std::mutex* QuadCopter::lock;
bool QuadCopter::interrupt;
Motor* QuadCopter::motors[4];

void QuadCopter::start() {
    
    Conf::init("settings.conf");
    
    interrupt = false;

    motors[0] = new Motor(15);
    motors[1] = new Motor(18);
    motors[2] = new Motor(23);
    motors[3] = new Motor(24);

    std::cout << "start\n";
    Gyro::start();
    /*
    for (int i = 0; i < 100; i++) {
        usleep(10000);
        Gyro::read();
    }
    */
    for (int i = 0; i < 100; i++) {
        usleep(100000);
        Gyro::read();
        std::cout << "p" << Gyro::getPitch() << "\tr" << Gyro::getRoll() << "\tz: " << Gyro::getZ() << '\n';
    }

    // Start the loop!
    loopThread = new std::thread(QuadCopter::tick);
}

void QuadCopter::tick() {
    std::cout << "tick\n";
}

void QuadCopter::cmd(char c) {
    Gyro::stop();
    std::cout << "stop\n";
}

void QuadCopter::stop() {
    for (int i = 0; i < 4; i++) {
        delete motors[i];
        motors[i] = 0;
    }
    interrupt = true;
    loopThread->join();
    Gyro::stop();
}

