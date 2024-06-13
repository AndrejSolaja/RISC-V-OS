//
// Created by os on 5/24/23.
//

#include "../h/_sem.hpp"
#include "../h/syscall_c.hpp"

int _sem::wait() {
    if (closed) return -1;
    value--;
    if(value<0){
        block();
    }
    if (closed) return -1; // do ovde dolaze niti koje su odblikirane zatvaranjem semafora
    return 0;
}

int _sem::signal() {
    if (closed) return -1;

    value++;
    if(value<=0){
        unblock();
    }
    return 0;
}

void _sem::block() {
    numOfBlockedThreads++;
    TCB::running->setBlocked(true);
    blockedThreads.addLast(TCB::running);
    thread_dispatch();
    //TCB::dispatch();
}

void _sem::unblock() {
    numOfBlockedThreads--;
    TCB* temp = blockedThreads.removeFirst();
    temp->setBlocked(false);
    Scheduler::put(temp);
}

int _sem::close() {
    if(closed) return -1;
    closed = true;
    for(int i = 0 ; i < numOfBlockedThreads; i++){
        TCB* temp = blockedThreads.removeFirst();
        temp->setBlocked(false);
    }
    return 0;


}

_sem::~_sem() {
    close();
}

int _sem::signalAll() {
    while(numOfBlockedThreads > 0){
        if(signal()) return -1;
    }
    return 0;
}
