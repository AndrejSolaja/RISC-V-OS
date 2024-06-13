//
// Created by os on 5/28/23.
//

#ifndef PROJECT_BASE_V1_1_IOBUFFER_HPP
#define PROJECT_BASE_V1_1_IOBUFFER_HPP

#include "../lib/hw.h"
#include "syscall_c.hpp"

class _sem;

class IOBuffer {
private:
    uint64 head, tail;
    sem_t itemAvailable, spaceAvailable;
    static const uint64 capacity = 128;
    char data[capacity];
    uint64 currentSize;

public:
    IOBuffer();
    void put(char);
    char get();

    void* operator new(size_t size){
        return MemoryAllocator::kmallocblok((size + sizeof(MemoryAllocator::BlockHeader) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    }
    void operator delete (void* adr){
        MemoryAllocator::kfree(adr);
    }

};

#endif //PROJECT_BASE_V1_1_IOBUFFER_HPP
