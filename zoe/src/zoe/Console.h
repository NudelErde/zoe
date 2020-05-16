/*
 * Console.h
 *
 *  Created on: 01.04.2019
 *      Author: florian
 */

#include "Core.h"
#include <memory>
#include <stdarg.h>
#include <sstream>

#pragma once

namespace Zoe {

std::string getThreadName();

template<typename T>
static inline std::string buildString(T t){
	std::stringstream ss;
	ss<<t;
	return ss.str();
}
template<typename T, typename... Args>
static inline void buildString(std::stringstream& ss, T t){
	ss<<t;
}
template<typename T, typename... Args>
static inline void buildString(std::stringstream& ss, T t, Args... args){
	ss<<t;
	buildString(ss, args...);
}
template<typename T, typename... Args>
static inline std::string buildString(T t, Args... args){
	std::stringstream ss;
	ss<<t;
	buildString(ss,args...);
	return ss.str();
}

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


template<typename T>
inline void debug(T t) {
	std::stringstream s;
	s << t;
	debug(s.str());
}
template<typename T, typename... Args>
inline void debug(T t, Args... args){
	debug(buildString(t, args...));
}
template<>
inline void debug<std::string>(std::string m) {
	Console::getLogger()->debug(m);
}
template<>
inline void debug<const std::string&>(const std::string& m) {
	Console::getLogger()->debug(m);
}
template<typename T>
inline void info(T t) {
	std::stringstream s;
	s << t;
	info(s.str());
}
template<typename T, typename... Args>
inline void info(T t, Args... args){
	info(buildString(t, args...));
}
template<>
inline void info<std::string>(std::string m) {
	Console::getLogger()->info(m);
}
template<>
inline void info<const std::string&>(const std::string& m) {
	Console::getLogger()->info(m);
}
template<typename T>
inline void warning(T t) {
	std::stringstream s;
	s << t;
	warning(s.str());
}
template<typename T, typename... Args>
inline void warning(T t, Args... args){
	warning(buildString(t, args...));
}
template<>
inline void warning<std::string>(std::string m) {
	Console::getLogger()->warning(m);
}
template<>
inline void warning<const std::string&>(const std::string& m) {
	Console::getLogger()->warning(m);
}
template<typename T>
inline void error(T t) {
	std::stringstream s;
	s << t;
	error(s.str());
}
template<typename T, typename... Args>
inline void error(T t, Args... args){
	error(buildString(t, args...));
}
template<>
inline void error<std::string>(std::string m) {
	Console::getLogger()->error(m);
}
template<>
inline void error<const std::string&>(const std::string& m) {
	Console::getLogger()->error(m);
}
template<typename T>
inline void critical(T t) {
	std::stringstream s;
	s << t;
	critical(s.str());
}
template<typename T, typename... Args>
inline void critical(T t, Args... args){
	critical(buildString(t, args...));
}
template<>
inline void critical<std::string>(std::string m) {
	Console::getLogger()->critical(m);
}
template<>
inline void critical<const std::string&>(const std::string& m) {
	Console::getLogger()->error(m);
}

}
