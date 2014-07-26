#include <iostream>
#include <iomanip>
#include <bcm2835.h>

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

    std::cout << "registering motors\n";
    motors[0] = new Motor(15);
    motors[1] = new Motor(18);
    motors[2] = new Motor(23);
    motors[3] = new Motor(24);

    std::cout << "Gyro::start()\n";
    Gyro::start();

    lock = new std::mutex();
    loopThread = new std::thread(QuadCopter::tick);
    std::cout << "looping\n";

    std::cout << "reading from stdin\n";
    char c;
    while (std::cin >> c) {
        cmd(c);
    }

    stop();
}

void QuadCopter::forceDecay(double* force, double decay) {
    if (*force < -1 * decay) *force += decay;
    else if (*force > decay) *force -=decay;
    else *force = 0;
}

void QuadCopter::tick() {

    while (!interrupt) {

        // Loop every 10 ms
        usleep(10000);

        // Populate gyroscope data
        Gyro::read();

        lock->lock();

        forceDecay(&forceX, Conf::XYDecay);
        forceDecay(&forceY, Conf::XYDecay);
        forceDecay(&forceZ, Conf::ZDecay);

        forceX -= Gyro::getRoll() * Conf::GyroForce;
        forceY -= Gyro::getPitch() * Conf::GyroForce;
        forceZ -= Gyro::getZ() * Conf::ZForce;

        lock->unlock();

        for (int i = 0; i < 4; i++) {
            // calculate motor speed
        }
    }
}

void QuadCopter::cmd(int c) {
    
    double forces[3];

    switch (c) {
        
        case FORWARD:
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

        case 'f':

            // DEBUG
            // print forces
            lock->lock();
            forces[0] = forceX;
            forces[1] = forceY;
            forces[2] = forceZ;
            lock->unlock();

            std::cout << std::setw(10) << forces[0]
                      << std::setw(10) << forces[1]
                      << std::setw(10) << forces[2]
                      << '\n';

            break;

        case 'g':

            // DEBUG
            // print gyroscope data
            lock->lock();
            lock->unlock();

            std::cout << std::setw(10) << Gyro::getRoll()
                      << std::setw(10) << Gyro::getPitch()
                      << std::setw(10) << Gyro::getZ()
                      << '\n';

            break;

        default:
            // nop
            break;
    }
}

// this could get more complicated, which is why it's in 
// its own function
void QuadCopter::applyImpulse(double* component, double amount) {
    *component += amount;
}

void QuadCopter::stop() {
    for (int i = 0; i < 4; i++)
        motors[i]->interrupt();

    for (int i = 0; i < 4; i++) {
        motors[i]->join();
        delete motors[i];
        motors[i] = 0;
    }

    interrupt = true;
    loopThread->join();
    Gyro::stop();
}

