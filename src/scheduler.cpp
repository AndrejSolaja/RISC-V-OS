//
// Created by marko on 20.4.22..
//

#include "../h/scheduler.hpp"
#include "../h/TCBList.hpp"
#include "../h/tcb.hpp"
#include "../h/syscall_c.hpp"

TCBList Scheduler::readyThreadQueue;
TCBList Scheduler::sleepingThreadQueue;

TCB *Scheduler::get()
{
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *ccb)
{
    readyThreadQueue.addLast(ccb);
}

int Scheduler::putSleep(time_t time) {
    if(time == 0) return 0;
    if(!TCB::running->isFinished()){
        TCB::running->setBlocked(true);
        sleepingThreadQueue.insertForSleep(TCB::running, time);
        //TCB::dispatch();
        thread_dispatch();
        return 0;
    }
    return -1;

}

void Scheduler::updateSleep() {
    if(sleepingThreadQueue.peekFirst() == nullptr) return;
    sleepingThreadQueue.peekFirst()->sleepTime--;
    while(sleepingThreadQueue.peekFirst() != nullptr && sleepingThreadQueue.peekFirst()->sleepTime == 0){
        TCB* temp = sleepingThreadQueue.removeFirst();
        temp->setBlocked(false);
        Scheduler::put(temp);
    }

}
