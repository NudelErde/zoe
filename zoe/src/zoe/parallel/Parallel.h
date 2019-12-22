/*
 * Parallel.h
 *
 *  Created on: 22.10.2019
 *      Author: florian
 */

#pragma once

#include <functional>
#include <cinttypes>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "../Core.h"

#define PARALLEL(C,I,CODE) parallel(C,[&](I)CODE)

typedef std::function<void(unsigned int)> parallelCodeType;

constexpr uint8_t workerAmount = 8;

namespace Zoe{

class Task{
private:
	unsigned int m_codeID;
	unsigned int m_taskID;
	parallelCodeType m_function;
public:
	Task(unsigned int codeID, parallelCodeType function);
	~Task();
	void operator()();
};

void DLL_PUBLIC parallel(unsigned int width, parallelCodeType lam);

}
