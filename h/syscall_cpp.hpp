//
// Created by os on 5/29/23.
//

#ifndef _syscall_cpp
#define _syscall_cpp

#include "syscall_c.hpp"

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();

    int start ();

    void join();

    static void dispatch ();
    static int sleep (time_t);



protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
    static void threadWrapper(void *);

};

class Semaphore {
public:
    explicit Semaphore (unsigned init = 1);
    virtual ~Semaphore ();

    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    explicit PeriodicThread (time_t period);
    virtual void periodicActivation () {}
    void run() override {
        while(running){
            periodicActivation();
            time_sleep(period);
        }
    }
private:
    time_t period;
    bool running = true;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};



#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
