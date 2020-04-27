/*
 * Thread.h
 *
 *  Created on: 28.10.2019
 *      Author: florian
 */

#pragma once

#include <mutex>
#include <condition_variable>
#include "../Core.h"

namespace Zoe {

    DLL_PUBLIC void waitForThreadNotify();

    void checkThreadSetup();

    struct ThreadInformation {
        bool m_notifyMe = true;
        std::mutex m_mutex;
        std::condition_variable m_cv;
        std::string name;
        unsigned int threadID;
    };

    class DLL_PUBLIC Thread {
    private:
        std::shared_ptr<ThreadInformation> threadInfo;

        explicit Thread(const std::string &name);

    public:
        explicit Thread(std::shared_ptr<ThreadInformation> threadInformation);

        Thread(void (*function)());

        Thread(const std::string &name, void (*function)());

        Thread();

        ~Thread();

        void notify();
        void notifyBlocking();

    public:

        friend void waitForThreadNotify();

        friend void checkThreadSetup();

    public:
        inline std::string getName() {
            return threadInfo->name;
        }
    };

    DLL_PUBLIC bool isMainThread();

    DLL_PUBLIC void sleep(unsigned int ms);

    DLL_PUBLIC extern Thread *mainThread;
    DLL_PUBLIC extern thread_local Thread *thisThread;

}  // namespace Zoe
