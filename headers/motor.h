#ifndef _Motor
#define _Motor

#include <mutex>
#include <thread>

class Motor {
    public:
        /**
         * Initialize the motor with a given GPIO pin
         * but don't start it up.
         */
        Motor(int);
        ~Motor();

        /**
         * This function is given to the thread that runs the motor
         */
        void loop();

        /**
         * Set the speed
         * @param speed 0.0 - 1.0
         */
        void setSpeed(double);

        /**
         * Start the motor
         */
        void start();

        /**
         * Tell the motor to stop. This will return immediately but the thread
         * will not complete until after the current iteration of the loop
         */
        void interrupt();

        /**
         * This will wait for the motor to stop spinning. You need to call
         * interrupt() before this, or else you'll wait forever
         */
        void join();
    private:
        int pin;
        double speed;
        bool running;
        volatile bool interrupt;
        std::mutex* lock;
        std::thread* loopThread;
};

#endif
