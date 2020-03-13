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

#define PARALLEL(C,I,CODE) ::Zoe::parallel(C,[&](unsigned int I)CODE)

typedef std::function<void(unsigned int)> parallelCodeType;

constexpr uint8_t workerAmount = 8;

namespace Zoe{

void DLL_PUBLIC parallel(unsigned int width, parallelCodeType lam);

}
