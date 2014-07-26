#ifndef _QuadCopter
#define _QuadCopter

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
        static void cmd(char);

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
        static void tick();

    private:

        // Force components
        static double forceX;
        static double forceY;
        static double forceZ;

        static void applyImpulse(double*, double);

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

        // Control characters
        static const int FORWARD        = 'w';
        static const int BACK           = 's';
        static const int LEFT           = 'a';
        static const int RIGHT          = 'd';
        static const int UP             = 'k';
        static const int DOWN           = 'j';
        static const int STOP           = 'x';
};

#endif
