

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_H

#include "../lib/hw.h"

class MemoryAllocator {
private:
    MemoryAllocator() {};

public:
    MemoryAllocator(const MemoryAllocator &obj) = delete;
    void operator=(const MemoryAllocator &) = delete;

    struct BlockHeader {
        BlockHeader *next;
        size_t size;
    };

    static BlockHeader *freeMemHead;

    static void init(){
        freeMemHead = (BlockHeader *) HEAP_START_ADDR;
        freeMemHead->next = nullptr;
        freeMemHead->size = (char *) HEAP_END_ADDR - 1 - (char *) HEAP_START_ADDR - sizeof(BlockHeader);
    }


    static void *kmalloc(size_t size);

    static int kfree(void *ptr);

    static void *kmallocblok(size_t size);

};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_H


