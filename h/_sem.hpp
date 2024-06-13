//
// Created by os on 5/24/23.
//

#ifndef PROJECT_BASE_V1_1__SEM_HPP
#define PROJECT_BASE_V1_1__SEM_HPP

#include "TCBList.hpp"
#include "MemoryAllocator.hpp"

class _sem {
private:
    bool closed = false;
    int value;
    int numOfBlockedThreads = 0;
    TCBList blockedThreads;
    void block();
    void unblock();


public:
    _sem(int val = 0) : value(val) {}
    int wait();
    int signal();
    int close();
    int signalAll();

    ~_sem();

    void* operator new(size_t size){
        return MemoryAllocator::kmallocblok((size + sizeof(MemoryAllocator::BlockHeader) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    }
    void operator delete (void* adr){
        MemoryAllocator::kfree(adr);
    }

};


#endif //PROJECT_BASE_V1_1__SEM_HPP
