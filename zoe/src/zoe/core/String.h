//
// Created by Florian on 31.07.2020.
//

#pragma once

#include <algorithm>
#include <cctype>
#include <locale>

namespace Zoe{

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

static inline void toLower(std::string& s){
    //TODO: make so that utf-8 likes u. maybe something different for text
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
}

static inline bool startsWith(const std::string& s, const std::string& value){
    return s.rfind(value, 0) == 0;
}

template <typename T>
inline std::vector<std::string> split(const std::string& s, T delimiter) {
    std::vector<std::string> result;
    size_t current = 0;
    size_t index = s.find_first_of(delimiter, 0);

    while (index != std::string::npos) {
        result.emplace_back(s, current, index - current);
        current = index + 1;
        index = s.find_first_of(delimiter, current);
    }

    result.emplace_back(s, current);

    return result;
}

template<typename T>
static inline T fromString(std::string string){
    std::stringstream sstream(string);
    T value;
    sstream >> value;
    return value;
}

}