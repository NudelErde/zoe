/*
 * Console.h
 *
 *  Created on: 01.04.2019
 *      Author: florian
 */

#include "Core.h"
#include <memory>
#include <cstdarg>
#include <sstream>
#include <iostream>

#pragma once

#ifdef ZOE_DEBUG
#define devDebug(...) debug(__VA_ARGS__,"\n[",__FILE__, ":",__LINE__,"]")
#else
#define devDebug(...)
#endif

namespace Zoe {

std::string getThreadName();

enum LogLevel {
	Debug, Info, Warning, Error, Critical
};

class DLL_PUBLIC Console {
private:
	LogLevel logLevel;
public:
	Console();
	virtual ~Console();

	void debug(const std::string& m);
	void info(const std::string& m);
	void warning(const std::string& m);
	void error(const std::string& m);
	void critical(const std::string& m);
	void setLoggingLevel(LogLevel level);

	static void init();
	static std::shared_ptr<Console> getLogger();

private:
    static void print(const std::string& m, const char* type);
};

template<typename... Args>
inline void debug(const Args&... args){
    std::stringstream s;
    (s << ...<< args);
    debug(s.str());
}
template<>
inline void debug<std::string>(const std::string& m) {
    Console::getLogger()->debug(m);
}
template<typename... Args>
inline void info(const Args&... args){
    std::stringstream s;
    (s << ...<< args);
    info(s.str());
}
template<>
inline void info<std::string>(const std::string& m) {
    Console::getLogger()->info(m);
}
template<typename... Args>
inline void warning(const Args&... args){
    std::stringstream s;
    (s << ...<< args);
    warning(s.str());
}
template<>
inline void warning<std::string>(const std::string& m) {
    Console::getLogger()->warning(m);
}
template<typename... Args>
inline void error(const Args&... args){
    std::stringstream s;
    (s << ...<< args);
    error(s.str());
}
template<>
inline void error<std::string>(const std::string& m) {
    Console::getLogger()->error(m);
}
template<typename... Args>
inline void critical(const Args&... args){
    std::stringstream s;
    (s << ...<< args);
    critical(s.str());
}
template<>
inline void critical<std::string>(const std::string& m) {
    Console::getLogger()->critical(m);
}

}
