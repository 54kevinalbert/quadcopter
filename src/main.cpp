#include "gyro.cpp"

int main(int argc, char** argv) {
    Gyro* gryo = new Gyro();
    for (int i = 0; i < 100; i++) {
        usleep(10000);
        gryo->read();
    }
}

