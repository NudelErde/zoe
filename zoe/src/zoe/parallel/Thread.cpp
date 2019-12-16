/*
 * Thread.cpp
 *
 *  Created on: 28.10.2019
 *      Author: florian
 */

#include "Thread.h"
#include <thread>
#include "Console.h"
#include <chrono>

#include <iostream>

namespace Zoe {

Thread* mainThread;
thread_local Thread* thisThread;

std::thread::id mainThreadID = std::this_thread::get_id();
static unsigned int nextThreadID = 0;

void checkSetup() {
	if (mainThreadID == std::this_thread::get_id()) {
		if (thisThread == nullptr) {
			mainThread = new Thread("Main");
			thisThread = mainThread;
		}
	}
}

struct ThreadInformationTransferObject {
	ThreadInformation* info;
	void (*function)();
};

void threadMethod(ThreadInformationTransferObject* ti) {
	std::unique_lock<std::mutex> initMutex(ti->info->m_mutex);
	initMutex.lock();
	sleep(10);
	initMutex.unlock();

	thisThread = new Thread(ti->info);
	ti->function();
	delete ti;
	delete thisThread;
}

void waitForThreadNotify() {
	checkSetup();
	if (thisThread == nullptr) {
		Zoe::error(
				"Error while waiting in thread, which was not created by Zoe Engine");
	} else {
		std::unique_lock<std::mutex> lk(thisThread->threadInfo->m_mutex);
		thisThread->threadInfo->m_notifyMe = false;
		thisThread->threadInfo->m_cv.wait(lk,
				[]() {return thisThread->threadInfo->m_notifyMe;});
		lk.unlock();
	}
}

Thread::Thread(const std::string& name) {
	if (isMainThread()) {
		threadInfo = new ThreadInformation();
		threadInfo->threadID = ++nextThreadID;
		threadInfo->name = name;
	} else {
		Zoe::critical("Dat sollte so nich passieren");
	}
}

Thread::Thread(ThreadInformation* threadInformation) {
	++threadInformation->references;
	this->threadInfo = threadInformation;
}

Thread::Thread(void (*function)()) {
	threadInfo = new ThreadInformation();
	std::lock_guard<std::mutex> initLock(threadInfo->m_mutex);
	threadInfo->threadID = ++nextThreadID;
	threadInfo->references = 1;
	std::stringstream ss;
	ss << threadInfo->threadID;
	threadInfo->name = ss.str();

	ThreadInformationTransferObject* ti = new ThreadInformationTransferObject();
	ti->info = threadInfo;
	ti->function = function;

	std::thread thread(threadMethod, ti);
	thread.detach();
}

Thread::Thread(const std::string& name, void (*function)()) {
	threadInfo = new ThreadInformation();
	std::lock_guard<std::mutex> initLock(threadInfo->m_mutex);
	threadInfo->threadID = ++nextThreadID;
	threadInfo->name = name;
	threadInfo->references = 1;

	ThreadInformationTransferObject* ti = new ThreadInformationTransferObject();
	ti->info = threadInfo;
	ti->function = function;

	std::thread thread(threadMethod, ti);
	thread.detach();
}

Thread::~Thread() {
	--threadInfo->references;
	if (threadInfo->references == 0) {
		delete threadInfo;
	}
}

void Thread::notify() {
	std::lock_guard<std::mutex>(threadInfo->m_mutex);
	threadInfo->m_notifyMe = true;
	threadInfo->m_cv.notify_all();
}

void sleep(unsigned int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

bool isMainThread() {
	return mainThreadID == std::this_thread::get_id();
}

//FROM console.h

std::string getThreadName() {
	checkSetup();
	if (thisThread != nullptr) {
		return thisThread->getName();
	} else {
		std::stringstream ss;
		ss << std::this_thread::get_id();
		return ss.str();
	}
}

}
