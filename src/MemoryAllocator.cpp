#include "../h/MemoryAllocator.hpp"

//Korisceni zadaci za inspiraciju:
//2. zadatak, drugi kolokvijum, april 2014.
//2. zadatak, drugi kolokvijum, septembar 2015.

MemoryAllocator::BlockHeader *MemoryAllocator::freeMemHead = nullptr;

void *MemoryAllocator::kmalloc(size_t size) {
    // Try to find an existing free block in the list (first fit):
    BlockHeader *blk = freeMemHead, *prev = nullptr;
    for (; blk!=nullptr; prev=blk, blk=blk->next)
        if (blk->size>=size) break;

    if(blk == nullptr) return nullptr; //NEMA VISE MESTA VIDECU STA TREBA U OVOM SLUCAJU

    size_t remainingSize = blk->size - size;
    if(remainingSize >= sizeof(BlockHeader) + MEM_BLOCK_SIZE){
        // Ostaje fragment
        blk->size=size;
        size_t offset = size + sizeof(BlockHeader);
        BlockHeader* newBlk = (BlockHeader*)((char*)blk + offset);
        if(prev) prev->next = newBlk;
        else freeMemHead = newBlk;
        newBlk->next = blk->next;
        newBlk->size = remainingSize - sizeof(BlockHeader);

    }else{
        // Ceo blok se alocira
        if (prev) prev->next = blk->next;
        else freeMemHead = blk->next;

    }
    blk->next = nullptr;
    return (char*)blk + sizeof(BlockHeader);
}

int MemoryAllocator::kfree(void *ptr) {
    BlockHeader *blockToFree = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));
    BlockHeader *blk = freeMemHead, *prev = nullptr;
    for (; blk!=nullptr; prev=blk, blk=blk->next)
        if (blk > blockToFree) break;

    if (blk == nullptr) {
        // blok je posle poslednjeg freeblocka
        if(prev) prev->next = blockToFree;
        else freeMemHead = blockToFree;
        blockToFree->next = nullptr;

    }else{
        if(prev) prev->next = blockToFree;
        else freeMemHead = blockToFree;
        blockToFree->next = blk;
    }
    return 0;

}



void *MemoryAllocator::kmallocblok(size_t size) {
    // Try to find an existing free block in the list (first fit):
    BlockHeader *blk = freeMemHead, *prev = nullptr;
    for (; blk!=nullptr; prev=blk, blk=blk->next)
        if (blk->size>=size) break;

    if(blk == nullptr) return nullptr; //NEMA VISE MESTA VIDECU STA TREBA U OVOM SLUCAJU

    size_t remainingSize = blk->size - size;
    if(remainingSize > 0){
        // Ostaje fragment
        blk->size=size;
        size_t offset = size * MEM_BLOCK_SIZE ;    //size_t offset = size + sizeof(BlockHeader);
        BlockHeader* newBlk = (BlockHeader*)((char*)blk + offset);
        if(prev) prev->next = newBlk;
        else freeMemHead = newBlk;
        newBlk->next = blk->next;
        newBlk->size = remainingSize;

    }else{
        // Ceo blok se alocira
        if (prev) prev->next = blk->next;
        else freeMemHead = blk->next;

    }
    blk->next = nullptr;
    return (char*)blk + sizeof(BlockHeader);
}

