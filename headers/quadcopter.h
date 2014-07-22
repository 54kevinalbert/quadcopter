#pragma once

#include "motor.h"

class QuadCopter {
    public:
        static void start();
        static void do(char c);
        static void stop();
    private:
        Motor** motors;
}
