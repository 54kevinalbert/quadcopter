#ifndef _Conf
#define _Conf

#include <string>

class Conf {
    public:
        static void init(std::string);
        static double GyroAcceleration;
        static double GyroForce;
        static double UserForce;
        static double ZForce;
};

#endif

