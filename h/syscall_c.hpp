//
// Created by os on 4/18/23.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP
#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/_sem.hpp"

typedef TCB *thread_t;
typedef _sem* sem_t;
typedef unsigned long time_t;
const int EOF = -1;

void* mem_alloc (size_t size);
int mem_free (void* adr);

int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg);
int thread_exit();
void thread_dispatch();
void thread_join(thread_t handle);
int time_sleep(time_t);

int sem_open(sem_t* handle, uint64 init);
int sem_close (sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

char getc();
void putc(char);



#endif //PROJECT_BASE_V1_1_SYSCALL_C_HPP
