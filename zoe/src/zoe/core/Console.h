/**
 * @file
 * This header provides the Console class and global logging functions.
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
    void debug(const std::string& m);
    /**
     * Logs an info message. Message is only printed to the console and the log file if the log level is LogLevel::Info or lower.
     * @param m message
     */
    void info(const std::string& m);
    /**
     * Logs a warning message. Message is only printed to the console and the log file if the log level is LogLevel::Warning or lower.
     * @param m message
     */
    void warning(const std::string& m);
    /**
     * Logs an error message. Message is only printed to the console and the log file if the log level is LogLevel::Error or lower.
     * @param m message
     */
    void error(const std::string& m);
    /**
     * Logs a critical message. Message is only printed to the console and the log file if the log level is LogLevel::critical or lower.
     * @param m message
     */
    void critical(const std::string& m);
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
    static void print(const std::string& m, const char* type);
};

template<typename Type>
static inline void collect(std::iostream& str, int index, const Type& value) {
    if (index == 0) {
        str << value;
    }
}

template<typename Type, typename ...Types>
static inline void collect(std::iostream& str, int index, const Type& value, const Types& ...values) {
    if (index == 0) {
        str << value;
    } else {
        collect(str, index - 1, values...);
    }
}

template<typename ...Types>
static inline std::string preparef(const std::string& format, const Types& ...values) {
    std::stringstream stringStream;
    const char* cstr = format.c_str();
    bool escaped = false;
    int index = 0;
    std::string buffer;
    for (const char* ptr = cstr; *ptr; ++ptr) {
        if (*ptr == '{' && !escaped) {
            for (++ptr; *ptr && *ptr != '}'; ++ptr) {
                buffer += *ptr;
            }
            if (buffer.length() == 0) {
                collect(stringStream, index, values...);
                index++;
            } else {
                collect(stringStream, std::stol(buffer), values...);
            }
        } else if (*ptr == '\\' && !escaped) {
            escaped = true;
        } else {
            escaped = false;
            stringStream << *ptr;
        }
    }
    return stringStream.str();
}

/**
 * Prints a debug message. The values are converted to a string by the `<<` operator.
 * @tparam Args the argument types.
 * @param args the argument values.
 */
template<typename... Args>
inline void debug(const Args& ... args) {
    std::stringstream s;
    (s << ...<< args);
    debug(s.str());
}

/**
 * Prints a specified debug message.
 * @param m the message
 */
template<>
inline void debug<std::string>(const std::string& m) {
    Console::getLogger()->debug(m);
}

/**
 * Prints a formatted debug message. The formatting can be escaped with `\\`.
 * Every unescaped `{index}` is replaced by the element in values at `index`.
 * If `index` is empty an intern index is used.
 * \n
 * Examples:
 * \code
 * debugf("Foo {}", "bar");                //Output: "Foo bar"
 * debugf("Foo {} {}", "bar", "boo");      //Output: "Foo bar boo"
 * debugf("Foo {1} {0}", "bar", "boo");    //Output: "Foo boo bar"
 * debugf("Foo {} {0} far", "bar", "boo"); //Output: "Foo bar bar far"
 * \endcode
 * @tparam Types the types of the parameters
 * @param format the format
 * @param values the parameters
 */
template<typename ...Types>
inline void debugf(const std::string& format, const Types& ... values) {
    std::string val = preparef(format, values...);
    debug(val);
}

/**
 * Prints an info message. The values are converted to a string by the `<<` operator.
 * @tparam Args the argument types.
 * @param args the argument values.
 */
template<typename... Args>
inline void info(const Args& ... args) {
    std::stringstream s;
    (s << ...<< args);
    info(s.str());
}
/**
 * Prints a specified info message.
 * @param m the message
 */
template<>
inline void info<std::string>(const std::string& m) {
    Console::getLogger()->info(m);
}

/**
 * Prints a formatted info message. The formatting can be escaped with `\\`.
 * Every unescaped `{index}` is replaced by the element in values at `index`.
 * If `index` is empty an intern index is used.
 * \n
 * Examples:
 * \code
 * infof("Foo {}", "bar");                //Output: "Foo bar"
 * infof("Foo {} {}", "bar", "boo");      //Output: "Foo bar boo"
 * infof("Foo {1} {0}", "bar", "boo");    //Output: "Foo boo bar"
 * infof("Foo {} {0} far", "bar", "boo"); //Output: "Foo bar bar far"
 * \endcode
 * @tparam Types the types of the parameters
 * @param format the format
 * @param values the parameters
 */
template<typename ...Types>
inline void infof(const std::string& format, const Types& ... values) {
    std::string val = preparef(format, values...);
    info(val);
}

/**
 * Prints a warning message. The values are converted to a string by the `<<` operator.
 * @tparam Args the argument types.
 * @param args the argument values.
 */
template<typename... Args>
inline void warning(const Args& ... args) {
    std::stringstream s;
    (s << ...<< args);
    warning(s.str());
}
/**
 * Prints a specified warning message.
 * @param m the message
 */
template<>
inline void warning<std::string>(const std::string& m) {
    Console::getLogger()->warning(m);
}

/**
 * Prints a formatted warning message. The formatting can be escaped with `\\`.
 * Every unescaped `{index}` is replaced by the element in values at `index`.
 * If `index` is empty an intern index is used.
 * \n
 * Examples:
 * \code
 * warningf("Foo {}", "bar");                //Output: "Foo bar"
 * warningf("Foo {} {}", "bar", "boo");      //Output: "Foo bar boo"
 * warningf("Foo {1} {0}", "bar", "boo");    //Output: "Foo boo bar"
 * warningf("Foo {} {0} far", "bar", "boo"); //Output: "Foo bar bar far"
 * \endcode
 * @tparam Types the types of the parameters
 * @param format the format
 * @param values the parameters
 */
template<typename ...Types>
inline void warningf(const std::string& format, const Types& ... values) {
    std::string val = preparef(format, values...);
    warning(val);
}

/**
 * Prints an error message. The values are converted to a string by the `<<` operator.
 * @tparam Args the argument types.
 * @param args the argument values.
 */
template<typename... Args>
inline void error(const Args& ... args) {
    std::stringstream s;
    (s << ...<< args);
    error(s.str());
}
/**
 * Prints a specified error message.
 * @param m the message
 */
template<>
inline void error<std::string>(const std::string& m) {
    Console::getLogger()->error(m);
}

/**
 * Prints a formatted error message. The formatting can be escaped with `\\`.
 * Every unescaped `{index}` is replaced by the element in values at `index`.
 * If `index` is empty an intern index is used.
 * \n
 * Examples:
 * \code
 * errorf("Foo {}", "bar");                //Output: "Foo bar"
 * errorf("Foo {} {}", "bar", "boo");      //Output: "Foo bar boo"
 * errorf("Foo {1} {0}", "bar", "boo");    //Output: "Foo boo bar"
 * errorf("Foo {} {0} far", "bar", "boo"); //Output: "Foo bar bar far"
 * \endcode
 * @tparam Types the types of the parameters
 * @param format the format
 * @param values the parameters
 */
template<typename ...Types>
inline void errorf(const std::string& format, const Types& ... values) {
    std::string val = preparef(format, values...);
    error(val);
}

/**
 * Prints a critical message. The values are converted to a string by the `<<` operator.
 * @tparam Args the argument types.
 * @param args the argument values.
 */
template<typename... Args>
inline void critical(const Args& ... args) {
    std::stringstream s;
    (s << ...<< args);
    critical(s.str());
}
/**
 * Prints a specified critical message.
 * @param m the message
 */
template<>
inline void critical<std::string>(const std::string& m) {
    Console::getLogger()->critical(m);
}

/**
 * Prints a formatted critical message. The formatting can be escaped with `\\`.
 * Every unescaped `{index}` is replaced by the element in values at `index`.
 * If `index` is empty an intern index is used.
 * \n
 * Examples:
 * \code
 * criticalf("Foo {}", "bar");                //Output: "Foo bar"
 * criticalf("Foo {} {}", "bar", "boo");      //Output: "Foo bar boo"
 * criticalf("Foo {1} {0}", "bar", "boo");    //Output: "Foo boo bar"
 * criticalf("Foo {} {0} far", "bar", "boo"); //Output: "Foo bar bar far"
 * \endcode
 * @tparam Types the types of the parameters
 * @param format the format
 * @param values the parameters
 */
template<typename ...Types>
inline void criticalf(const std::string& format, const Types& ... values) {
    std::string val = preparef(format, values...);
    critical(val);
}

}
