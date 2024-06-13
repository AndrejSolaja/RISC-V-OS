//
// Created by marko on 20.4.22..
//

#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"

void *operator new(size_t arg1)
{
    //return __mem_alloc(arg1);

    //arg1 = (arg1 + sizeof(MemoryAllocator::BlockHeader) +MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    //return MemoryAllocator::kmallocblok(arg1);


    return mem_alloc(arg1); //ovo samo ne treba , ne treba preko sistemskog
    //return MemoryAllocator::kmalloc(arg1);

}

void *operator new[](size_t arg1)
{
    //return __mem_alloc(arg1);

    //arg1 = (arg1 + sizeof(MemoryAllocator::BlockHeader) +MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    //return MemoryAllocator::kmallocblok(arg1);

    return mem_alloc(arg1);
    //return MemoryAllocator::kmalloc(arg1);

}

void operator delete(void *p) noexcept
{
    //__mem_free(p);
    mem_free(p);
    //MemoryAllocator::kfree(p);
}

void operator delete[](void *p) noexcept
{
    //__mem_free(p);
    mem_free(p);
    //MemoryAllocator::kfree(p);
}