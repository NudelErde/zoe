/*
 * Console.cpp
 *
 *  Created on: 01.04.2019
 *      Author: florian
 */

#include "../zpch.h"

#include "Console.h"
#include "File.h"
#include <mutex>
#include <ctime>

using namespace Zoe;

static std::mutex consoleMutex;

Console::Console() {
    this->logLevel = LogLevel::Debug;
}

Console::~Console() = default;

void Console::critical(const std::string &m) {
    if (this->logLevel <= LogLevel::Critical)Console::print(m, "critical");
}
void Console::debug(const std::string &m) {
    if (this->logLevel <= LogLevel::Debug)Console::print(m, "debug");
}
void Console::error(const std::string &m) {
    if (this->logLevel <= LogLevel::Error)Console::print(m, "error");
}
void Console::info(const std::string &m) {
    if (this->logLevel <= LogLevel::Info)Console::print(m, "info");
}
void Console::warning(const std::string &m) {
    if (this->logLevel <= LogLevel::Warning)Console::print(m, "warning");
}

static inline std::string getTimeString(const std::string& format){
    std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm now_tm = *std::localtime(&now_c);
    char buffer[80];
    strftime(buffer, 80, format.c_str(), &now_tm);
    return std::string((char*)buffer);
}

static File createLogFile() { //TODO: c++20 std::chrono::format
    std::string name = "./log/";
    name += getTimeString("%Y-%m-%d-%H-%M-%S");
    name += ".log";
    File file = File(name);
    file.create();
    return file;
}

void Console::print(const std::string &m, const char *type) {
    static File logFile = createLogFile();
    static std::unique_ptr<std::iostream> fileStream = logFile.createIOStream(false);
    static std::iostream& fstream = *fileStream;
    std::lock_guard<std::mutex> lock(consoleMutex);

    std::stringstream sstream;
    sstream << "[" << type << "][THREAD-" << getThreadName() << "][" << getTimeString("%Y/%m/%d-%X") << "] " << m;

    std::cout << sstream.str() << std::endl;
    fstream << sstream.str() << std::endl;
}

void Console::setLoggingLevel(LogLevel level) {
    this->logLevel = level;
}

static std::shared_ptr<Console> instance;

std::shared_ptr<Console> Console::getLogger() {
    return instance;
}

void Console::init() {
    instance = std::make_shared<Console>();
    instance->setLoggingLevel(LogLevel::Debug);
}
