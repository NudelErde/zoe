//
// Created by Florian on 01.11.2020.
//

#pragma once

#include "Core.h"
#include <string>
#include <functional>

namespace Zoe {

/**
 * UTF contains static methods for utf8 conversions
 */
class UTF {
public:
    /**
     * Converts a string of codepoints to a string of utf8 characters.
     * @param codepoint the input string
     * @return a utf8 string
     */
    static std::string codepointToUTF8(const std::string& codepoint);

    /**
     * Converts a string of utf8 characters to a string of codepoints.
     * @param utf8 the input string
     * @return a codepoint string
     */
    static std::string utf8ToCodepoint(const std::string& utf8);

    /**
     * Converts a string of codepoints to a string of utf8 characters. The callback is called for every byte in the resulting string.
     * @param codepoint the input string
     * @param callback the callback
     */
    static void codepointToUTF8(const std::string& codepoint, const std::function<void(uint8_t)>& callback);

    /**
     * Converts a string of utf8 characters to a string of codepoints. The callback is called for every codepoint in the resulting string.
     * @param utf8 the input string
     * @param callback the callback
     */
    static void utf8ToCodepoint(const std::string& utf8, const std::function<void(uint32_t)>& callback);

    /**
     * A codepoints to a string of utf8 characters. The callback is called for every byte in the resulting string.
     * @param codepoint the input codepoint
     * @param callback the callback
     */
    static void codepointToUTF8(char32_t codepoint, const std::function<void(uint8_t)>& callback);
};

/**
 * UTF8String is a string of utf8 characters. It has methods to modify the characters at specified indices.
 * The std::string can only modify bytes with doesn't work for utf8 strings.
 */
class UTF8String {
public:

    /**
     * Creates an empty UTF8String
     */
    UTF8String() = default;

    /**
     * Creates an UTF8String from an existing std::string.
     */
    UTF8String(std::string);

    /**
     * Implicitly converts this UTF8String to an std::string.
     */
    operator std::string&();

    /**
     * Implicitly converts this UTF8String to an std::string.
     */
    operator const std::string&() const;

    /**
     * Converts this string to an UTF32 string / string of codepoints.
     * @return the UTF32 string
     * @see UTF::utf8ToCodepoint
     */
    [[nodiscard]] std::string asUTF32() const;

    /**
     * Converts this string to an UTF32 string / string of codepoints. The callback is called for every codepoint in the resulting string.
     * @param callback the callback
     * @see UTF::utf8ToCodepoint
     */
    void asUTF32(const std::function<void(uint32_t)>& callback) const;

    /**
     * Calculates the count of utf8 characters in this string. A utf8 character can use multiple bytes. To get the size in byte use length.
     * @return the count of utf8 characters
     * @see UTF8String::length
     */
    [[nodiscard]] std::string::size_type charCount() const;

    /**
     * Insert an utf32 character at the specified index. The character is converted to an utf8 character.
     * @param index the position where the character is inserted
     * @param u32char the inserted character
     */
    void insert(unsigned int index, char32_t u32char);

    /**
     * Removes a character at the specified index.
     * @param index the position of the removed character
     */
    void remove(unsigned int index);

    /**
     * Returns the size of this string in bytes.
     * @return the size in bytes
     * @see https://en.cppreference.com/w/cpp/string/basic_string/size
     */
    [[nodiscard]] inline auto length() const noexcept { return string.length(); }

    /**
     * Returns the size of this string in bytes.
     * @return the size in bytes
     * @see https://en.cppreference.com/w/cpp/string/basic_string/size
     */
    [[nodiscard]] inline auto size() const noexcept { return string.size(); }

public:

    /**
     * Creates a UTF8String from an utf32 string.
     * @param utf32 the utf32 string
     * @return the UTF8String
     */
    inline static UTF8String fromUTF32(const std::string& utf32) { return UTF::codepointToUTF8(utf32); }

private:
    std::string string;
};

}
