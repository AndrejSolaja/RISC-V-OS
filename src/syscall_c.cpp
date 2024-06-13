//
// Created by os on 4/18/23.
//

#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/TCBList.hpp"

void *mem_alloc(size_t size) {
    volatile uint64 ret;
    size_t arg1 = (size + sizeof(MemoryAllocator::BlockHeader) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE; //poravnato na blokove
    //size_t arg1 = size;
    __asm__ volatile ("mv a1, %[ulaz]" : :[ulaz]"r"(arg1));
    __asm__ volatile ("li a0, 0x01");  // li - load immediate
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[izlaz], a0" :[izlaz]"=r"(ret));
    return (void *) ret;
}

int mem_free(void *adr) {
    volatile uint64 ret;
    void *arg1 = adr;
    __asm__ volatile ("mv a1, %[ulaz]" : :[ulaz]"r"(arg1));
    __asm__ volatile ("li a0, 0x02");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[izlaz], a0" :[izlaz]"=r"(ret));
    return ret;
}

int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg){
    volatile uint64 ret;
    thread_t* arg1 = handle;
    void(*arg2)(void *) = start_routine;
    void* arg3 = arg;
    __asm__ volatile ("mv a3, %[ulaz]" : :[ulaz]"r"(arg3));
    __asm__ volatile ("mv a2, %[ulaz]" : :[ulaz]"r"(arg2));
    __asm__ volatile ("mv a1, %[ulaz]" : :[ulaz]"r"(arg1));
    __asm__ volatile ("li a0, 0x11");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[izlaz], a0" :[izlaz]"=r"(ret));
    return ret;
}

int thread_exit(){
    volatile uint64 ret;
    __asm__ volatile ("li a0, 0x12");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[izlaz], a0" :[izlaz]"=r"(ret));
    return ret;

};

void thread_dispatch(){
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
    return;
};

void thread_join(thread_t handle){
    thread_t arg1 = handle;
    __asm__ volatile ("mv a1, %[ulaz]" : :[ulaz]"r"(arg1));
    __asm__ volatile ("li a0, 0x14");
    __asm__ volatile ("ecall");
    return;
};

int sem_open(sem_t* handle, uint64 init){
    volatile uint64 ret;
    sem_t* arg1 = handle;
    uint64 arg2 = init;
    __asm__ volatile ("mv a2, %[ulaz]" : :[ulaz]"r"(arg2));
    __asm__ volatile ("mv a1, %[ulaz]" : :[ulaz]"r"(arg1));
    __asm__ volatile ("li a0, 0x21");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[izlaz], a0" :[izlaz]"=r"(ret));
    return ret;
}
int sem_close (sem_t handle){
    volatile uint64 ret;
    sem_t arg1 = handle;
    __asm__ volatile ("mv a1, %[ulaz]" : :[ulaz]"r"(arg1));
    __asm__ volatile ("li a0, 0x22");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[izlaz], a0" :[izlaz]"=r"(ret));
    return ret;
}
int sem_wait(sem_t id){
    volatile uint64 ret;
    sem_t arg1 = id;
    __asm__ volatile ("mv a1, %[ulaz]" : :[ulaz]"r"(arg1));
    __asm__ volatile ("li a0, 0x23");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[izlaz], a0" :[izlaz]"=r"(ret));
    return ret;
}
int sem_signal(sem_t id){
    volatile uint64 ret;
    sem_t arg1 = id;
    __asm__ volatile ("mv a1, %[ulaz]" : :[ulaz]"r"(arg1));
    __asm__ volatile ("li a0, 0x24");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[izlaz], a0" :[izlaz]"=r"(ret));
    return ret;
}

int time_sleep(time_t time) {
    volatile uint64 ret;
    time_t arg1 = time;
    __asm__ volatile ("mv a1, %[ulaz]" : :[ulaz]"r"(arg1));
    __asm__ volatile ("li a0, 0x31");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[izlaz], a0" :[izlaz]"=r"(ret));
    return ret;
}

char getc(){
    volatile char ret;
    __asm__ volatile ("li a0, 0x41");
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %[izlaz], a0" :[izlaz]"=r"(ret));
    return ret;
}

void putc(char c){
    char arg1 = c;
    __asm__ volatile ("mv a1, %[ulaz]" : :[ulaz]"r"(arg1));
    __asm__ volatile ("li a0, 0x42");
    __asm__ volatile ("ecall");
    return;
}

