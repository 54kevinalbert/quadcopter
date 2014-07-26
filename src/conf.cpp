#include <iostream>
#include <fstream>
#include <string>
#include "conf.h"

double Conf::GyroAcceleration;

void Conf::init(std::string filename) {

    std::ifstream settingsFile(filename.c_str());
    std::string key;

    while (settingsFile >> key) {
        if (key.compare("GYRO_ACCELERATION"))
            settingsFile >> GyroAcceleration;
    }

    std::cout << "finished reading " << filename << '\n';
    std::cout << "GyroAcceleration = " << GyroAcceleration << '\n';
    settingsFile.close();
}
