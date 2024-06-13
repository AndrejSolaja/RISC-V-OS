//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "_sem.hpp"
#include "MemoryAllocator.hpp"

// Thread Control Block
class TCB
{
public:

    void* operator new(size_t size){
        return MemoryAllocator::kmallocblok((size + sizeof(MemoryAllocator::BlockHeader) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    }
    void operator delete (void* adr){
        MemoryAllocator::kfree(adr);
    }

    ~TCB() { delete[] stack; delete joinSem; }

    bool isFinished() const { return finished; }

    bool isBlocked() const { return blocked; }

    bool isUserMode() const {return userMode; }

    void setFinished(bool value) { finished = value; }

    void setBlocked(bool value) { blocked = value; }

    void setSleepTimer(uint64 value) {sleepTime = value;}

    void setUserMode(bool value) {userMode = value;}

    uint64 getTimeSlice() const  {return timeSlice;}

    using Body = void (*)(void*);

    static TCB *createThread(Body body, bool);

    static TCB *running;

private:

    explicit TCB(Body body, uint64 timeSlice) :
        body(body),
        stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
        //stack(body != nullptr ? (uint64*)MemoryAllocator::kmalloc(DEFAULT_STACK_SIZE) : nullptr),
        context({ (uint64) &threadWrapper,
                  stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
        }),
        timeSlice(timeSlice),
        finished(false),
        blocked(false),
        joinSem(new _sem(0)),
        sleepTime(0)
    {
        if(body != nullptr) Scheduler::put(this);
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    bool blocked;
    void* arg;
    _sem* joinSem;
    time_t sleepTime;
    bool userMode = true;

    friend class Riscv;

    friend class _sem;

    friend class TCBList;

    friend class Scheduler;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static int exit();


};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_CCB_HPP