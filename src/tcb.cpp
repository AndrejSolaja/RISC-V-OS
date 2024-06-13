//
// Created by os on 4/21/23.
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/scheduler.hpp"
#include "../h/syscall_c.hpp"

TCB *TCB::running = nullptr;


void TCB::dispatch() {
    TCB* old = running;
    if(!old->isFinished() && !old->isBlocked()) {
        Scheduler::put(old);
    }
    running = Scheduler::get();
    TCB::running->timeSlice = DEFAULT_TIME_SLICE;

    contextSwitch(&old->context, &running->context);

}

TCB *TCB::createThread(TCB::Body body, bool userMode) {
    TCB* temp = new TCB(body, DEFAULT_TIME_SLICE);
    temp->setUserMode(userMode);
    return temp;
}

void TCB::threadWrapper() {
    Riscv::ms_sstatus(Riscv::SSTATUS_SPIE);
    Riscv::popSppSpie(); //izlazak iz sistemskog

    running->body(running->arg);

    thread_exit();
    //gasenje niti
}

int TCB::exit() {
    if(!running->isFinished()){
        running->setFinished(true);
        running->joinSem->signalAll(); //Za join update
        dispatch(); //thread_dispatch();
        return 0;
    }
    return -1;
}





