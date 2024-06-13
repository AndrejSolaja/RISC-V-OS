//
// Created by os on 5/28/23.
//

#include "../h/IOBuffer.hpp"

IOBuffer::IOBuffer() {
    head = 0; tail = 0; currentSize = 0;
    itemAvailable = nullptr; spaceAvailable = nullptr;
    sem_open(&itemAvailable, 0);
    sem_open(&spaceAvailable, capacity);
}

void IOBuffer::put(char c) {
    sem_wait(spaceAvailable);
    currentSize++;
    data[tail] = c;
    tail = (tail + 1) % capacity;
    sem_signal(itemAvailable);
}

char IOBuffer::get() {
    sem_wait(itemAvailable);
    currentSize--;
    char c = data[head];
    head = (head + 1) % capacity;
    sem_signal(spaceAvailable);
    return c;
}

