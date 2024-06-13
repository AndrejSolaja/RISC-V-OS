#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/_sem.hpp"
#include "../h/syscall_cpp.hpp"

//sepc = supervisor exception pc  , adresa gde treba da se vrati
// SPP = supervisor previous privilege  (vrednost koja pokazuje iz kog je rezima dosao)
// SIE = supervisor interrupt enable
// SPIE = supervisor previous interrupt enbale
// scause = supervisor cause
// stvec = supervisor trap vector
// sip = supervisor interrupt pending (bit1 - zahtev za prekid, vrednost 1-postavlaj zahtev, 0-obradjen prekid, bit9 -postoji zahtev za spoljansji prekid)

class Test : public PeriodicThread{
public:
    Test(time_t vreme) : PeriodicThread(vreme) {}
    void periodicActivation() override{
        putc('M');
        putc('M');
        putc('M');
    }
};


extern void userMain();

void wrapper( void* ){
    userMain();
}
void main() {
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);


    MemoryAllocator::init();
    Riscv::initBuffers();

    thread_t kernelThread;

    kernelThread = TCB::createThread(nullptr, false);
    TCB::running = kernelThread;
    TCB::createThread(idleWorker, false);
    TCB::createThread(outputWorker, false);
    TCB::createThread(inputWorker, false);

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);  //inte


    //userMain();
    thread_t userThread;
    thread_create(&userThread, wrapper, nullptr);
    thread_join(userThread);


    time_sleep(20);
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);  //intd
}