#include <iostream>
#include "gyro.cpp"

int main(int argc, char** argv) {
    std::cout << "start\n";
    Gyro* gyro = new Gyro();
    for (int i = 0; i < 100; i++) {
        usleep(100000);
        gyro->read();
    }
    std::cout << "stop\n";
}

