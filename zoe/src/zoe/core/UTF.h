//
// Created by Florian on 01.11.2020.
//

#pragma once

#include "Core.h"
#include <string>
#include <functional>

namespace Zoe {

class UTF {
public:
    static std::string codepointToUTF8(const std::string& codepoint);
    static std::string utf8ToCodepoint(const std::string& utf8);
    static void codepointToUTF8(const std::string& codepoint, const std::function<void(uint8_t)>& callback);
    static void utf8ToCodepoint(const std::string& utf8, const std::function<void(uint32_t)>& callback);
    static void codepointToUTF8(char32_t codepoint, const std::function<void(uint8_t)>& callback);
};

}
