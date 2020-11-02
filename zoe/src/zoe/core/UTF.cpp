//
// Created by Florian on 01.11.2020.
//

#include "UTF.h"
#include "Console.h"

namespace Zoe {

std::string UTF::codepointToUTF8(const std::string& codepoint) {
    std::string result;
    codepointToUTF8(codepoint, [&result](uint8_t ch) { result += (char) ch; });
    return result;
}
std::string UTF::utf8ToCodepoint(const std::string& utf8) {
    std::string result;
    utf8ToCodepoint(utf8, [&result](uint32_t ch) {
        result += (char) ((ch >> 24u) & 0xFFu);
        result += (char) ((ch >> 16u) & 0xFFu);
        result += (char) ((ch >> 8u) & 0xFFu);
        result += (char) ((ch >> 0u) & 0xFFu);
    });
    return result;
}
void UTF::codepointToUTF8(const std::string& codepoint, const std::function<void(uint8_t)>& callback) {
    const char* cstr = codepoint.c_str();
    auto length = codepoint.length();
    if (length % 4 != 0) {
        warning("The UTF32 string has an illegal size.");
    }
    for (decltype(length) i = 0; i < length / 4; ++i) {
        uint32_t ch32 = 0;
        ch32 |= (uint8_t) cstr[i * 4 + 0];
        ch32 <<= 8u;
        ch32 |= (uint8_t) cstr[i * 4 + 1];
        ch32 <<= 8u;
        ch32 |= (uint8_t) cstr[i * 4 + 2];
        ch32 <<= 8u;
        ch32 |= (uint8_t) cstr[i * 4 + 3];
        codepointToUTF8(ch32, callback);
    }
}
void UTF::codepointToUTF8(char32_t codepoint, const std::function<void(uint8_t)>& callback) {
    if (codepoint <= 0b01111111) {
        //one byte boi
        //0XXX XXXX
        callback((uint8_t) codepoint);
    } else if (codepoint <= 0b011111111111) {
        //two bytes boi
        //110X XXXX 10XX XXXX
        uint8_t ch0 = 0b11000000u;
        uint8_t ch1 = 0b10000000u;

        ch1 |= (codepoint & 0b00111111u);
        codepoint >>= 6u;
        ch0 |= (codepoint & 0b00011111u);

        callback(ch0);
        callback(ch1);
    } else if (codepoint <= 0b1111111111111111) {
        //three bytes boi
        //1110 XXXX 10XX XXXX 10XX XXXX

        uint8_t ch0 = 0b11100000u;
        uint8_t ch1 = 0b10000000u;
        uint8_t ch2 = 0b10000000u;

        ch2 |= (codepoint & 0b00111111u);
        codepoint >>= 6u;
        ch1 |= (codepoint & 0b00111111u);
        codepoint >>= 6u;
        ch0 |= (codepoint & 0b00001111u);

        callback(ch0);
        callback(ch1);
        callback(ch2);
    } else if (codepoint <= 0b000111111111111111111111) {
        //four bytes boi
        //1111 0XXX 10XX XXXX 10XX XXXX 10XX XXXX
        uint8_t ch0 = 0b11110000u;
        uint8_t ch1 = 0b10000000u;
        uint8_t ch2 = 0b10000000u;
        uint8_t ch3 = 0b10000000u;

        ch3 |= (codepoint & 0b00111111u);
        codepoint >>= 6u;
        ch2 |= (codepoint & 0b00111111u);
        codepoint >>= 6u;
        ch1 |= (codepoint & 0b00111111u);
        codepoint >>= 6u;
        ch0 |= (codepoint & 0b00000111u);

        callback(ch0);
        callback(ch1);
        callback(ch2);
        callback(ch3);
    } else {
        //don't know this boi
        warning("The codepoint with the value ", codepoint, "can not be converted to a UTF8 sequence");
    }
}
void UTF::utf8ToCodepoint(const std::string& utf8, const std::function<void(uint32_t)>& callback) {
    auto length = utf8.length();
    const auto* cstr = reinterpret_cast<const uint8_t*>(utf8.c_str());
    uint32_t codepoint;
    for (int i = 0; i < length; ++i) {
        codepoint = cstr[i];
        if (codepoint & (uint32_t) BIT(7)) {
            if (codepoint & (uint32_t) BIT(6)) {
                if (codepoint & (uint32_t) BIT(5)) {
                    if (codepoint & (uint32_t) BIT(4)) {
                        //should be 4 byte symbol
                        if (i + 3 >= length) {
                            warning("The utf8 string is not long enough for 4 byte char.");
                            ++i;
                            continue;
                        }
                        codepoint = 0;
                        codepoint |= (cstr[i] & (uint32_t) 0b00000111);
                        codepoint <<= (uint32_t) 6;
                        codepoint |= (cstr[i + 1] & (uint32_t) 0b00111111);
                        codepoint <<= (uint32_t) 6;
                        codepoint |= (cstr[i + 2] & (uint32_t) 0b00111111);
                        codepoint <<= (uint32_t) 6;
                        codepoint |= (cstr[i + 2] & (uint32_t) 0b00111111);
                        i += 3;
                    } else {
                        //3 byte symbol
                        if (i + 2 >= length) {
                            warning("The utf8 string is not long enough for 3 byte char.");
                            ++i;
                            continue;
                        }
                        codepoint = 0;
                        codepoint |= (cstr[i] & (uint32_t) 0b00001111);
                        codepoint <<= (uint32_t) 6;
                        codepoint |= (cstr[i + 1] & (uint32_t) 0b00111111);
                        codepoint <<= (uint32_t) 6;
                        codepoint |= (cstr[i + 2] & (uint32_t) 0b00111111);
                        i += 2;
                    }
                } else {
                    // 2 byte symbol
                    if (i + 1 >= length) {
                        warning("The utf8 string is not long enough for 2 byte char.");
                        ++i;
                        continue;
                    }
                    codepoint = 0;
                    codepoint |= (cstr[i] & (uint32_t) 0b00011111);
                    codepoint <<= (uint32_t) 6;
                    codepoint |= (cstr[i + 1] & (uint32_t) 0b00111111);
                    i += 1;
                }
            } else {
                warning("An unused multibytechar was found in the utf8 string.");
            }
        }
        callback(codepoint);
    }
}

}