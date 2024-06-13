//
// Created by os on 5/29/23.
//
#include "../h/syscall_cpp.hpp"

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle = nullptr;
    this->body = body;
    this->arg = arg;

}

Thread::~Thread() {
    //myHandle->setFinished(true);
    thread_join(myHandle);
    mem_free(myHandle);
}

int Thread::start() {
    if(body != nullptr)
        return thread_create(&myHandle, body, arg);
    else
        return thread_create(&myHandle, threadWrapper, arg);
}

void Thread::join() {
    thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

void Thread::threadWrapper(void *thread) {
    ((Thread*)thread)->run();
}

Thread::Thread() {
    myHandle = nullptr;
    this->body = nullptr;
    this->arg = this;
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

Semaphore::Semaphore(unsigned int init) {
    myHandle = nullptr;
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

PeriodicThread::PeriodicThread(time_t period) : Thread() {
    this->period = period;
}

void PeriodicThread::terminate() {
    this->running = false;
}

