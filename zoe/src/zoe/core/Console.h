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

/**
 * Get name of current thread. If this thread is a named Zoe::Thread class, the name is returned.
 * Otherwise the id is returned.
 * @returns the name of the current thread
 */
std::string getThreadName();

/**
 * LogLevels for Console. If a lower log level is selected, all higher messages with higher log levels are also logged.
 */
enum class LogLevel {
    Debug, Info, Warning, Error, Critical
};

/**
 * Console encapsulates the logging logic. The messages are printed to the console and to a log file.
 */
class DLL_PUBLIC Console {
private:
    LogLevel logLevel;
public:
    /**
     * Constructs a new console. Please use Console::getLogger or one of the logging methods in the Zoe namespace to log messages.
     */
    Console();

    /**
     * Destructs the console object.
     */
    ~Console();

    /**
     * Logs a debug message. Message is only printed to the console and the log file if the log level is LogLevel::Debug.
     * @param m message
     */
    void debug(const std::string &m);
    /**
     * Logs an info message. Message is only printed to the console and the log file if the log level is LogLevel::Info or lower.
     * @param m message
     */
    void info(const std::string &m);
    /**
     * Logs a warning message. Message is only printed to the console and the log file if the log level is LogLevel::Warning or lower.
     * @param m message
     */
    void warning(const std::string &m);
    /**
     * Logs an error message. Message is only printed to the console and the log file if the log level is LogLevel::Error or lower.
     * @param m message
     */
    void error(const std::string &m);
    /**
     * Logs a critical message. Message is only printed to the console and the log file if the log level is LogLevel::critical or lower.
     * @param m message
     */
    void critical(const std::string &m);
    /**
     * Sets the new logging level.
     * @param level new LogLevel
     */
    void setLoggingLevel(LogLevel level);

    /**
     * Initializes the default Console.
     */
    static void init();
    /**
     * Access the default console. Returns a default initialized shared_ptr if init wasn't called.
     * @return the default Console instance
     */
    static std::shared_ptr<Console> getLogger();

private:
    static void print(const std::string &m, const char *type);
};

/**
 * Prints a debug message. The values are converted to a string by the `<<` operator.
 * @tparam Args the argument types.
 * @param args the argument values.
 */
template<typename... Args>
inline void debug(const Args &... args) {
    std::stringstream s;
    (s << ...<< args);
    debug(s.str());
}
/**
 * Prints a specified debug message.
 * @param m the message
 */
template<>
inline void debug<std::string>(const std::string &m) {
    Console::getLogger()->debug(m);
}
/**
 * Prints an info message. The values are converted to a string by the `<<` operator.
 * @tparam Args the argument types.
 * @param args the argument values.
 */
template<typename... Args>
inline void info(const Args &... args) {
    std::stringstream s;
    (s << ...<< args);
    info(s.str());
}
/**
 * Prints a specified info message.
 * @param m the message
 */
template<>
inline void info<std::string>(const std::string &m) {
    Console::getLogger()->info(m);
}
/**
 * Prints a warning message. The values are converted to a string by the `<<` operator.
 * @tparam Args the argument types.
 * @param args the argument values.
 */
template<typename... Args>
inline void warning(const Args &... args) {
    std::stringstream s;
    (s << ...<< args);
    warning(s.str());
}
/**
 * Prints a specified warning message.
 * @param m the message
 */
template<>
inline void warning<std::string>(const std::string &m) {
    Console::getLogger()->warning(m);
}
/**
 * Prints an error message. The values are converted to a string by the `<<` operator.
 * @tparam Args the argument types.
 * @param args the argument values.
 */
template<typename... Args>
inline void error(const Args &... args) {
    std::stringstream s;
    (s << ...<< args);
    error(s.str());
}
/**
 * Prints a specified error message.
 * @param m the message
 */
template<>
inline void error<std::string>(const std::string &m) {
    Console::getLogger()->error(m);
}
/**
 * Prints a critical message. The values are converted to a string by the `<<` operator.
 * @tparam Args the argument types.
 * @param args the argument values.
 */
template<typename... Args>
inline void critical(const Args &... args) {
    std::stringstream s;
    (s << ...<< args);
    critical(s.str());
}
/**
 * Prints a specified critical message.
 * @param m the message
 */
template<>
inline void critical<std::string>(const std::string &m) {
    Console::getLogger()->critical(m);
}

}
