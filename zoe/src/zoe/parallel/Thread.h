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

void waitForThreadNotify();

struct ThreadInformation {
	bool m_notifyMe = false;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::string name;
	unsigned int threadID;
	unsigned int references;
};

class DLL_PUBLIC Thread {
private:
	ThreadInformation* threadInfo;

	Thread(const std::string& name);

public:
	Thread(ThreadInformation* threadInformation);
	Thread(void (*function)());
	Thread(const std::string& name, void (*function)());
	~Thread();

	void notify();

public:

	friend void waitForThreadNotify();
	friend void checkSetup();

public:
	inline std::string getName() {
		return threadInfo->name;
	}
};

bool isMainThread();
void sleep(unsigned int ms);

extern Thread* mainThread;
extern thread_local Thread* thisThread;

}  // namespace Zoe
