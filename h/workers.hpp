//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_WORKERS_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_WORKERS_HPP

extern void workerBodyAM(void* );

extern void workerBodyBM(void* );

extern void workerBodyCM(void* );

extern void workerBodyDM(void* );

extern void idleWorker(void *);

extern void inputWorker(void *);

extern void outputWorker(void *);

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_WORKERS_HPP