//
// Created by marko on 20.4.22..
//
// Fajl preuzet sa teamsa
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/_sem.hpp"

#include "../h/IOBuffer.hpp"

IOBuffer* Riscv::inputBuffer = nullptr;
IOBuffer* Riscv::outputBuffer = nullptr;
sem_t Riscv::inputSem = nullptr;
sem_t Riscv::outputSem = nullptr;

void Riscv::initBuffers() {
    inputBuffer = new IOBuffer();
    outputBuffer = new IOBuffer();
    sem_open(&inputSem, 0);
    sem_open(&outputSem, 0);
}

void Riscv::popSppSpie() {
    if (TCB::running->isUserMode())
        mc_sstatus(Riscv::SSTATUS_SPP);
    else
        ms_sstatus(Riscv::SSTATUS_SPP);

    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap() {
    volatile uint64 sepc = r_sepc();
    volatile uint64 sstatus = r_sstatus();
    volatile uint64 scauseVar = r_scause();

    if (scauseVar == 0x08 || scauseVar == 0x09) {
        uint64 brojOperacije;
        __asm__ volatile("ld %0, 10*8(fp)" : "=r"(brojOperacije)); //a0
        uint64 arg1, arg2, arg3;
        __asm__ volatile("ld %0, 11*8(fp)" : "=r"(arg1)); //a1
        __asm__ volatile("ld %0, 12*8(fp)" : "=r"(arg2)); //a2
        __asm__ volatile("ld %0, 13*8(fp)" : "=r"(arg3)); //a3


        if (brojOperacije == MEM_ALLOC_CODE) { //mem_alloc
            void *ret = MemoryAllocator::kmallocblok((size_t) arg1);
            __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(ret));

        } else if (brojOperacije == MEM_FREE_CODE) { //mem_free
            uint64 ret = MemoryAllocator::kfree((void *) arg1);
            __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(ret));

        } else if (brojOperacije == THREAD_CREATE_CODE) { //thread_create
            //arg1 - t_handle, arg2 - start_routine, arg3 - arg
            typedef void (*start_routine)(void*);
            TCB *temp = TCB::createThread((start_routine)arg2, true);
            if(temp){
                temp->arg = (void*)arg3;
                TCB** t_handle = (TCB**)arg1;
                *t_handle = temp;
                __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(0));
            }else{
                __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(-1));
            }
        } else if (brojOperacije == THREAD_EXIT_CODE) { //thread_exit
            uint64 ret = TCB::exit();
            __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(ret));

        } else if (brojOperacije == THREAD_DISPATCH_CODE) { //thread_dispatch
            TCB::dispatch();
        } else if (brojOperacije == THREAD_JOIN) { //thread_join
            //arg1 - handle
            TCB* handle = (TCB*)arg1;
            if(handle != nullptr && !handle->isFinished())
                handle->joinSem->wait();

        } else if (brojOperacije == SEM_OPEN_CODE) { //sem_open
            //arg1 - handle, arg2 - init
            _sem* temp = new _sem(arg2);
            if(temp){
                _sem** handle = (_sem**)arg1;
                *handle = temp;
                __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(0));
            }else{
                __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(-1));
            }
        } else if (brojOperacije == SEM_CLOSE_CODE) { //sem_close
            //arg1 - handle
            uint64 ret;
            _sem* handle = (_sem*) arg1;
            ret = handle->close();
            __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(ret));
        } else if (brojOperacije == SEM_WAIT_CODE) { //sem_wait
            //arg1 - handle
            uint64 ret;
            _sem* handle = (_sem*) arg1;
            ret = handle->wait();
            __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(ret));
        } else if (brojOperacije == SEM_SIGNAL_CODE) { //sem_signal
            //arg1 - handle
            uint64 ret;
            _sem* handle = (_sem*) arg1;
            ret = handle->signal();
            __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(ret));
        } else if (brojOperacije == TIME_SLEEP_CODE) { //time_sleep
            //arg1 -ul time
            uint64 ret;
            time_t time = (time_t) arg1;
            ret = Scheduler::putSleep(time);
            __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(ret));
        } else if (brojOperacije == GETC_CODE) { //getc
            char ret;
            ret = inputBuffer->get();
            __asm__ volatile ("sd %[ulaz], 10*8(fp)" : : [ulaz]"r"(ret));
        } else if (brojOperacije == PUTC_CODE) { //putc
            //arg1 - char
            char c = (char) arg1;
            outputBuffer->put(c);
        }

        sepc += 4;
        //mc_sip(SIP_SSIP); //signaliranje da je gotovo

    } else if (scauseVar == 0x8000000000000001UL) {
        //Softverski prekid iz treceg rezima (timer)

        Scheduler::updateSleep(); // Dekrementiranje vremena za spavanje i po potrebi vracanje u scheduler
        if(TCB::running != nullptr) TCB::running->timeSlice--;

        if(TCB::running != nullptr && TCB::running->getTimeSlice() <= 0){

            TCB::dispatch();
        }

        mc_sip(SIP_SSIP);
    } else if (scauseVar == 0x8000000000000009UL) {
        //Spoljasnji prekid
        //console_handler();
        int code = plic_claim();

        sem_signal(inputSem);
        sem_signal(outputSem);

        plic_complete(code);
        mc_sip(SIP_SEIP);


    } else {
        //Ilegalna instrukcija/Nedozovljena adresa citanja/ nedozvoljena adresa upisa
        //print greske
        printInteger(scauseVar);
        printStringM(" ERROR \n");

    }

    w_sstatus(sstatus);
    w_sepc(sepc);

}