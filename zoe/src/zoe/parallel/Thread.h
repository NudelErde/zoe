/*
 * Thread.h
 *
 *  Created on: 28.10.2019
 *      Author: florian
 */

#pragma once

#include <mutex>
#include <condition_variable>
#include "../core/Core.h"

namespace Zoe {

/**
 * ThreadInformation contains the information shared between the threads.
 */
struct ThreadInformation {
    /**
     * The bool used to check whether the thread should be woken up.
     */
    bool m_notifyMe = true;

    /**
     * The mutex used to lock the condition variable with.
     */
    std::mutex m_mutex;

    /**
     * The condition variable used for waiting.
     */
    std::condition_variable m_cv;

    /**
     * The thread name
     */
    std::string name;

    /**
     * The threadID
     */
    unsigned int threadID;
};

/**
 * A Thead is used to execute code in parallel.
 */
class Thread {
private:
    std::shared_ptr<ThreadInformation> threadInfo;

    explicit Thread(const std::string& name);

    explicit Thread(std::shared_ptr<ThreadInformation> threadInformation);

    struct ThreadInformationTransferObject;

    friend void threadMethod(Thread::ThreadInformationTransferObject*);
public:

    /**
     * Creates a thread from a function pointer. The name is the thread ID.
     * @param function the main function of the thread
     */
    explicit Thread(void (* function)());

    /**
     * Creates a thread from a function pointer and the specified name.
     * @param name the specified name
     * @param function the main function of the thread
     */
    Thread(const std::string& name, void (* function)());

    /**
     * Creates an empty thread.
     */
    Thread();

    /**
     * Destructs the thread.
     */
    ~Thread();

    /**
     * Wakes the thread if it sleeps.
     */
    void notify();

    /**
     * Wakes the thread if it sleeps. The function is blocked until the thread is woken up.
     */
    void notifyBlocking();

    /**
     * Returns true if the current thread is the main thread.
     * @return `true` if the current thread is the main thread
     */
    static bool isMainThread();

    /**
     * Sleeps until this thread is woken up from another thread.
     */
    static void waitForThreadNotify();

    /**
     * Get a pointer to the main thread object.
     * @return a pointer to the main thread object
     */
    static Thread* getMainThread();

    /**
     * Get a pointer to the current thread object.
     * @return a pointer to the current thread object
     */
    static Thread* getThisThread();

public:

    /**
     * Ensures that the thread setup was executed.
     */
    static void checkThreadSetup();

public:

    /**
     * Returns the name of the thread.
     * @return the name of the thread
     */
    inline std::string getName() {
        return threadInfo->name;
    }
};

/**
 * Sleeps for the specified time.
 * @param ms the specified time
 */
void sleep(unsigned int ms);

}  // namespace Zoe
