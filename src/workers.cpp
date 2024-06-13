//
// Created by marko on 20.4.22..
//

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"
#include "../h/IOBuffer.hpp"


void workerBodyAM(void* arg)
{
    for (uint64 i = 0; i < 10; i++)
    {
        printStringM("A: i=");
        printInteger(i);
        printStringM("\n");
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield();
        }
    }
}

void workerBodyBM(void* )
{
    for (uint64 i = 0; i < 16; i++)
    {
        printStringM("B: i=");
        printInteger(i);
        printStringM("\n");
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield();
        }
    }
}

static uint64 fibonacci(uint64 n)
{
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) {
        thread_dispatch();
        //TCB::yield();
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyCM(void* )
{
    uint8 i = 0;
    for (; i < 3; i++)
    {
        printStringM("C: i=");
        printInteger(i);
        printStringM("\n");
    }

    printStringM("C: yield\n");
    __asm__ ("li t1, 7");
    //TCB::yield();
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printStringM("C: t1=");
    printInteger(t1);
    printStringM("\n");

    uint64 result = fibonacci(12);
    printStringM("C: fibonaci=");
    printInteger(result);
    printStringM("\n");

    for (; i < 6; i++)
    {
        printStringM("C: i=");
        printInteger(i);
        printStringM("\n");
    }
//    TCB::yield();
}

void workerBodyDM(void* )
{
    uint8 i = 10;
    for (; i < 13; i++)
    {
        printStringM("D: i=");
        printInteger(i);
        printStringM("\n");
    }

    printStringM("D: yield\n");
    __asm__ ("li t1, 5");
    //TCB::yield();
    thread_dispatch();

    uint64 result = fibonacci(16);
    printStringM("D: fibonaci=");
    printInteger(result);
    printStringM("\n");

    for (; i < 16; i++)
    {
        printStringM("D: i=");
        printInteger(i);
        printStringM("\n");
    }
//    TCB::yield();
}

void idleWorker(void* ){
    while(true){
        thread_dispatch();
    }
}

void inputWorker(void *){
    while(true){

        sem_wait(Riscv::inputSem);
        while(*((char*)(CONSOLE_STATUS)) & CONSOLE_RX_STATUS_BIT) {
            //uzimanje sa konzole
            char c = *((char*)CONSOLE_RX_DATA);
            //if(c != '\r') Riscv::inputBuffer->put(c);
            Riscv::inputBuffer->put(c);
            // stavljati u ulazni buffer
        }
    }


}

void outputWorker(void *){
    while(true) {
        sem_wait(Riscv::outputSem);
        while (*((char *) (CONSOLE_STATUS)) & CONSOLE_TX_STATUS_BIT) {
            //pisanje u konzolu
            //uzeti iz izlaznog da se prikaze
            char c = Riscv::outputBuffer->get();
            *((char *) CONSOLE_TX_DATA) = c;

        }
    }
}
