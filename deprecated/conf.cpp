#include <iostream>
#include <fstream>
#include <string>
#include "conf.h"


double Conf::GyroAcceleration;

void Conf::init(std::string filename) {

    std::ifstream settingsFile(filename.c_str());
    std::string key;

    while (settingsFile >> key) {

        if (key.compare("#")) {
            std::getline (std::cin,key);
            continue;
        }

        if (key.compare("GyroAcceleration"))    settingsFile >> GyroAcceleration;
        else if (key.compare("GyroForce"))      settingsFile >> GyroForce;
        else if (key.compare("UserForce"))      settingsFile >> UserForce;
        else if (key.compare("ZForce"))         settingsFile >> ZForce;

    }

    std::cout << "finished reading " << filename << '\n'
              << "  GyroAcceleration: " << GyroAcceleration << '\n'
              << "  GyroForce:        " << GyroForce << '\n'
              << "  UserForce:        " << UserForce << '\n'
              << "  ZForce:           " << ZForce << '\n';

    settingsFile.close();
}
