#include <iostream>

#include "conf.h"
#include "gyro.h"
#include "motor.h"
#include "quadcopter.h"

std::thread* QuadCopter::loopThread;
std::mutex* QuadCopter::lock;
bool QuadCopter::interrupt;
Motor* QuadCopter::motors[4];

double QuadCopter::forceX;
double QuadCopter::forceY;
double QuadCopter::forceZ;

void QuadCopter::start() {
    
    std::cout << "Reading settings\n";
    Conf::init("settings.conf");

    std::cout << "bcm2835_init()\n";
    bcm2835_init();
    
    interrupt = false;

    std::cout << "registering motors\n"
    motors[0] = new Motor(15);
    motors[1] = new Motor(18);
    motors[2] = new Motor(23);
    motors[3] = new Motor(24);

    std::cout << "Gyro::start()\n";
    Gyro::start();

    std::cout << "looping\n";
    loopThread = new std::thread(QuadCopter::tick);

    std::cout << "reading from stdin\n";
    char c;
    while (std::cin >> c) {
        cmd(c);
    }

    stop();
}

void QuadCopter::tick() {
    usleep(10000);

}

void QuadCopter::cmd(int c) {
    switch (c) {
        
        case FORWARD
            applyImpulse(&forceY, 0.1);
            break;

        case BACK:
            applyImpulse(&forceY, -0.1);
            break;

        case LEFT:
            applyImpulse(&forceX, -0.1);
            break;

        case RIGHT:
            applyImpulse(&forceX, 0.1);
            break;

        case UP:
            applyImpulse(&forceZ, 0.1);
            break;

        case DOWN:
            applyImpulse(&forceZ, -0.1);
            break;

        case STOP:
            stop();
            break;

        default:
            // nop
    }
}

// this could get more complicated
void applyImpulse(double* component, double amount) {
    *component += amount;
}

void QuadCopter::stop() {
    for (int i = 0; i < 4; i++)
        motors[i]->stop();

    for (int i = 0; i < 4; i++) {
        motors[i]->join();
        delete motors[i];
        motors[i] = 0;
    }

    interrupt = true;
    loopThread->join();
    Gyro::stop();
}

