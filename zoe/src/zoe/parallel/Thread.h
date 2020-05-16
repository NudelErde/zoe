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

        static bool isMainThread();
        static void waitForThreadNotify();
        static Thread* getMainThread();
        static Thread* getThisThread();

    public:

        static void checkThreadSetup();

    public:
        inline std::string getName() {
            return threadInfo->name;
        }
    };

    DLL_PUBLIC void sleep(unsigned int ms);

}  // namespace Zoe
