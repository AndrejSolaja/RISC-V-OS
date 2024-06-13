//
// Created by os on 5/27/23.
//

#ifndef PROJECT_BASE_V1_1_TCBLIST_HPP
#define PROJECT_BASE_V1_1_TCBLIST_HPP

#include "../lib/hw.h"
#include "MemoryAllocator.hpp"

class TCB;

class TCBList
{
private:
    struct Elem
    {
        TCB *data;
        Elem *next;

        Elem(TCB *data, Elem *next) : data(data), next(next) {}

        void* operator new(size_t size){
            return MemoryAllocator::kmallocblok((size + sizeof(MemoryAllocator::BlockHeader) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
        }
        void operator delete (void* adr){
            MemoryAllocator::kfree(adr);
        }

    };

    Elem *head, *tail;

public:
    TCBList() : head(0), tail(0) {}

    TCBList(const TCBList &) = delete;

    TCBList &operator=(const TCBList &) = delete;

    void printElements();

    void insertForSleep(TCB *data, time_t time);

    void addFirst(TCB *data)
    {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(TCB *data)
    {
        Elem *elem = new Elem(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    TCB *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        TCB *ret = elem->data;
        delete elem;
        return ret;
    }

    TCB *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    TCB *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        TCB *ret = elem->data;
        delete elem;
        return ret;
    }

    TCB *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }

};



#endif //PROJECT_BASE_V1_1_TCBLIST_HPP
