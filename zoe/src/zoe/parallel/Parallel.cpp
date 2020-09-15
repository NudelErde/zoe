/*
 * Parallel.cpp
 *
 *  Created on: 22.10.2019
 *      Author: florian
 */

#include "Parallel.h"
#include "Thread.h"
#include "../core/Console.h"
#include <mutex>
#include <condition_variable>
#include <queue>
#include <sstream>

namespace Zoe {

    struct Task {
        Task(const unsigned int &index, const parallelCodeType &lambda) : index(index), lambda(lambda) {}

        unsigned int index;
        parallelCodeType lambda;
    };

    struct TaskGroup {
        TaskGroup(Thread *associatedThread, const unsigned int &workAmount) : associatedThread(associatedThread),
                                                                              progress(workAmount) {}

        Thread *associatedThread;
        unsigned int progress;
    };

    static Thread **workerArray = 0;
    static std::queue<Task> taskQueue;
    static std::mutex queueMutex;

    static thread_local bool living = true;

    static void workerMain() {
        std::unique_lock lock(queueMutex, std::defer_lock_t());
        Thread::waitForThreadNotify();
        while (living) {
            Thread::waitForThreadNotify();
            lock.lock();
            while (taskQueue.size() > 0) {
                Task task = taskQueue.front();
                taskQueue.pop();
                lock.unlock();
                task.lambda(task.index);
                std::this_thread::yield();
                lock.lock();
            }
            lock.unlock();
        }

    }

    static void init() {
        Thread::checkThreadSetup();
        workerArray = new Thread *[workerAmount];
        for (unsigned int i = 0; i < workerAmount; ++i) {
            std::stringstream stringStream;
            stringStream << "Worker-" << i;
            workerArray[i] = new Thread(stringStream.str(),&workerMain);
        }
        for (unsigned int i = 0; i < workerAmount; ++i) {
            workerArray[i]->notifyBlocking();
        }
    }

    void DLL_PUBLIC parallel(unsigned int width, parallelCodeType lam) {
        if (workerArray == 0)
            init();

        std::unique_lock<std::mutex> lock(queueMutex);
        TaskGroup *tg = new TaskGroup(Thread::getThisThread(), width);
        for (unsigned int i = 0; i < width; ++i) {
            taskQueue.push(Task(i, [lam, tg](unsigned int i) {
                lam(i);
                std::lock_guard<std::mutex> lockGuard(queueMutex);
                if ((--tg->progress) == 0) {
                    tg->associatedThread->notifyBlocking();
                    delete tg;
                }
            }));
        }
        for (unsigned int i = 0; i < workerAmount; ++i) {
            workerArray[i]->notify();
        }
        lock.unlock();
        Thread::waitForThreadNotify();
    }

}