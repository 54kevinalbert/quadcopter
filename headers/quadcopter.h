#pragma once

#include <mutex>
#include <thread>
#include "motor.h"

class QuadCopter {
    public:
        
        /**
         * initialize everything
         * start the loop
         */
        static void start();

        /**
         * Process an instruction
         */
        static void do(char);

        /**
         * Set the interrupt flag
         * join the loop thread
         * turn off gyroscope
         * shutdown
         */
        static void stop();

        /**
         * Perform an iteration of the loop
         */
        static bool tick();

    private:

        // Threads
        static std::thread* loopThread;
        static std::mutex* lock;
        static bool interrupt;

        // Motor layout:
        // 0   1
        //  \ /
        //   X
        //  / \
        // 2   3
        static Motor* motors[4];
        static const int TOP_LEFT       = 0;
        static const int TOP_RIGHT      = 1;
        static const int BOTTOM_LEFT    = 2;
        static const int BOTTOM_RIGHT   = 3;
}
