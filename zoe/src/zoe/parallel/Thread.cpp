/*
 * Thread.cpp
 *
 *  Created on: 28.10.2019
 *      Author: florian
 */

#include "Thread.h"
#include <thread>
#include "../core/Console.h"
#include <chrono>

#include <iostream>
#include <utility>

namespace Zoe {

static Thread* mainThread;
static thread_local Thread* thisThread;

std::thread::id mainThreadID = std::this_thread::get_id();
static unsigned int nextThreadID = 0;

void Thread::checkThreadSetup() {
    if (mainThreadID == std::this_thread::get_id()) {
        if (thisThread == nullptr) {
            mainThread = new Thread("Main");
            thisThread = mainThread;
        }
    }
}

struct Thread::ThreadInformationTransferObject {
    std::shared_ptr<ThreadInformation> info;

    void (* function)(){};
};

void threadMethod(Thread::ThreadInformationTransferObject* ti) {
    std::unique_lock<std::mutex> initLock(ti->info->m_mutex);
    if (!initLock.owns_lock()) {
        initLock.lock();
    }
    sleep(10);
    initLock.unlock();

    thisThread = new Thread(ti->info);
    ti->function();
    delete ti;
    delete thisThread;
}

Thread::Thread(const std::string& name) {
    if (isMainThread()) {
        threadInfo = std::make_shared<ThreadInformation>();
        threadInfo->threadID = ++nextThreadID;
        threadInfo->name = name;
    } else {
        Zoe::critical("Dat sollte so nich passieren");
    }
}

Thread::Thread(std::shared_ptr<ThreadInformation> threadInformation) {
    this->threadInfo = std::move(threadInformation);
}

Thread::Thread(void (* function)()) {
    threadInfo = std::make_shared<ThreadInformation>();
    std::lock_guard<std::mutex> initLock(threadInfo->m_mutex);

    threadInfo->threadID = ++nextThreadID;
    std::stringstream ss;
    ss << threadInfo->threadID;
    threadInfo->name = ss.str();

    ThreadInformationTransferObject* ti = new ThreadInformationTransferObject();
    ti->info = threadInfo;
    ti->function = function;

    std::thread thread(threadMethod, ti);
    thread.detach();
}

Thread::Thread(const std::string& name, void (* function)()) {
    threadInfo = std::make_shared<ThreadInformation>();
    std::lock_guard<std::mutex> initLock(threadInfo->m_mutex);
    threadInfo->threadID = ++nextThreadID;
    threadInfo->name = name;

    ThreadInformationTransferObject* ti = new ThreadInformationTransferObject();
    ti->info = threadInfo;
    ti->function = function;

    std::thread thread(threadMethod, ti);
    thread.detach();
}

Thread::~Thread() = default;

void Thread::waitForThreadNotify() {
    checkThreadSetup();
    if (thisThread == nullptr) {
        Zoe::error(
                "Error while waiting in thread, which was not created by Zoe Engine");
    } else {
        std::unique_lock<std::mutex> lk(thisThread->threadInfo->m_mutex);
        thisThread->threadInfo->m_notifyMe = false;
        thisThread->threadInfo->m_cv.wait(lk,
                                          []() { return thisThread->threadInfo->m_notifyMe; });
        lk.unlock();
    }
}

void Thread::notify() {
    std::lock_guard<std::mutex> lockGuard(threadInfo->m_mutex);
    threadInfo->m_notifyMe = true;
    threadInfo->m_cv.notify_all();
}

void Thread::notifyBlocking() {
    std::unique_lock<std::mutex> lock(this->threadInfo->m_mutex, std::defer_lock_t());

    bool val = true;
    while (val) {
        lock.lock();
        val = this->threadInfo->m_notifyMe;
        lock.unlock();
        std::this_thread::yield();
    }
    notify();
}

Thread::Thread() = default;

void sleep(unsigned int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

bool Thread::isMainThread() {
    return mainThreadID == std::this_thread::get_id();
}

Thread* Thread::getMainThread() {
    return mainThread;
}

Thread* Thread::getThisThread() {
    return thisThread;
}

//FROM console.h

std::string getThreadName() {
    Thread::checkThreadSetup();
    if (thisThread != nullptr) {
        return thisThread->getName();
    } else {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        return ss.str();
    }
}

}
