/*
 * Parallel.cpp
 *
 *  Created on: 22.10.2019
 *      Author: florian
 */

#include "Parallel.h"
#include <mutex>
#include <condition_variable>

static std::mutex taskListMutex;
static std::condition_variable taskListCV;
static bool notifyMePls = false;

static bool hasNextTask(){
	std::lock_guard<std::mutex> lk(taskListMutex);
	return false;
}

static void waitForNewTask(){
	std::unique_lock<std::mutex> lk(taskListMutex);
	taskListCV.wait(lk, [](){return notifyMePls;});
	lk.unlock();
}

static void workerMain(){
	while(true){
		if(hasNextTask()){
			//arbeite
		}else{
			//warte du opfa
			waitForNewTask();
		}
	}
}





void static init() {
	for(uint8_t i = 0; i < workerAmount; ++i){
		std::thread thread(&workerMain);
		thread.detach();
	}
}

void DLL_PUBLIC Zoe::parallel(unsigned int width, parallelCodeType lam) {
	static bool needInit = true;
	if (needInit) {
		init();
		needInit = false;
	}

}

