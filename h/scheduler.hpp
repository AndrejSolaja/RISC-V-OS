//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP

#include "TCBList.hpp"
class TCB;

class Scheduler
{
private:
    Scheduler(){};

    static TCBList readyThreadQueue;
    static TCBList sleepingThreadQueue;
public:
    Scheduler(const Scheduler &obj) = delete;
    void operator=(const Scheduler &) = delete;

    static TCB *get();
    static void put(TCB *ccb);

    static int putSleep(time_t);
    static void updateSleep();

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP