/*
 * Console.cpp
 *
 *  Created on: 01.04.2019
 *      Author: florian
 */

#include "../zpch.h"

#include "Console.h"
#include <mutex>

using namespace Zoe;

static std::mutex consoleMutex;

Console::Console() {
	this->logLevel=LogLevel::Debug;
}

Console::~Console() {
}

void Console::critical(const std::string& m) {
	if(this->logLevel<=LogLevel::Critical)Console::print(m, "critical");
}
void Console::debug(const std::string& m) {
	if(this->logLevel<=LogLevel::Debug)Console::print(m, "debug");
}
void Console::error(const std::string& m) {
	if(this->logLevel<=LogLevel::Error)Console::print(m, "error");
}
void Console::info(const std::string& m) {
	if(this->logLevel<=LogLevel::Info)Console::print(m, "info");
}
void Console::warning(const std::string& m) {
	if(this->logLevel<=LogLevel::Warning)Console::print(m, "warning");
}

void Console::print(const std::string& m, const char* type) {
    std::lock_guard<std::mutex> lock(consoleMutex);
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%Y/%m/%d-%X", timeinfo);

	std::cout << "[" << type << "][THREAD-" << getThreadName()
			<< "][" << buffer << "] " << m << std::endl;
}

void Console::setLoggingLevel(LogLevel level){
	this->logLevel = level;
}

static std::shared_ptr<Console> instance;

std::shared_ptr<Console> Console::getLogger() {
    return instance;
}

void Console::init(){
    instance = std::make_shared<Console>();
	instance->setLoggingLevel(LogLevel::Debug);
}
