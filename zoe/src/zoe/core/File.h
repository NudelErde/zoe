/*
 * File.h
 *
 *  Created on: 18.11.2019
 *      Author: florian
 */

#pragma once

#include "Core.h"
#include <iostream>
#include <memory>
#include <cinttypes>

namespace Zoe{

class DLL_PUBLIC File {
public:

    enum class FileType{
        VIRTUAL, FILE_SYSTEM
    };

    explicit File(const std::string& path);
    explicit File(const std::string& path, const FileType&, bool binary = false);
    File(const File&);
    ~File();

    File& operator=(const File&);

    std::iostream& getIOStream();
    std::unique_ptr<uint8_t[]> getContent(size_t* size = nullptr) const;
    [[nodiscard]] const std::string& getPath() const;

    [[nodiscard]] std::unique_ptr<std::istream> createIStream(bool binary = false) const;

private:
    std::unique_ptr<std::iostream> m_iostream;
    std::string m_path;
    FileType m_type;
    bool m_binary;
private:
    static FileType getFileTypeByPath(const std::string& path);
};

}

//
// Created by Florian on 19.07.2020.
//
