//
// Created by os on 5/27/23.
//

#include "../lib/hw.h"
#include "../h/TCBList.hpp"
#include "../h/tcb.hpp"
#include "../h/print.hpp"


void TCBList::insertForSleep(TCB *data, time_t time) {
    Elem *prev = nullptr;
    for (Elem *curr = head; curr; curr = curr->next)
    {
        if(time < curr->data->sleepTime)
            break;
        time -= curr->data->sleepTime;
        prev = curr;
    }

    Elem* elem;
    if(prev){
        elem = new Elem(data, prev->next);
        prev->next = elem;
        if(!prev->next)
            tail = elem;
    }else{
        elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }
    elem->data->setSleepTimer(time);

    for(Elem *curr = elem->next; curr; curr = curr->next){
        curr->data->setSleepTimer(curr->data->sleepTime - time);
    }

}

void TCBList::printElements() {
    for (Elem *curr = head; curr ; curr = curr->next)
    {
        printInteger(curr->data->sleepTime);
        printStringM(" ");
    }
    printStringM("\n");
}
