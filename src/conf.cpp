//
// generated file: src/conf.cpp" >>src/conf.cpp
// 26/07/14
//

#include <iostream>
#include <fstream>
#include <string>
#include "conf.h"

double Conf::GyroAcceleration = 0.000000152587890625;
double Conf::GyroForce = 0.25;
double Conf::UserForce = 0.1;
double Conf::ZForce = 1.0;
double Conf::XYDecay = 0.1;
double Conf::ZDecay = 0.1;
void Conf::init(std::string filename) {

    std::ifstream settingsFile(filename.c_str());
    std::string key;

    while (settingsFile >> key) {

        if (key.compare("#")) {
            std::getline (std::cin,key);
            continue;
        }
        if (key.compare("GyroAcceleration")) {
            settingsFile >> GyroAcceleration;
            continue;
        }
        if (key.compare("GyroForce")) {
            settingsFile >> GyroForce;
            continue;
        }
        if (key.compare("UserForce")) {
            settingsFile >> UserForce;
            continue;
        }
        if (key.compare("ZForce")) {
            settingsFile >> ZForce;
            continue;
        }
        if (key.compare("XYDecay")) {
            settingsFile >> XYDecay;
            continue;
        }
        if (key.compare("ZDecay")) {
            settingsFile >> ZDecay;
            continue;
        }
}

    std::cout << "finished reading " << filename << '\n'
             << "  GyroAcceleration: " << GyroAcceleration << '\n'
             << "  GyroForce: " << GyroForce << '\n'
             << "  UserForce: " << UserForce << '\n'
             << "  ZForce: " << ZForce << '\n'
             << "  XYDecay: " << XYDecay << '\n'
             << "  ZDecay: " << ZDecay << '\n';
    settingsFile.close();
}

