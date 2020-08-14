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
#include <vector>

namespace Zoe{

class Directory;
class File;

class DLL_PUBLIC Path {
public:
    Path(const std::string&);

    bool isFile() const;
    bool isDirectory() const;

    Directory getParent() const;
    std::string getAbsolutePath() const;
};

class DLL_PUBLIC Directory: public Path {
public:
    Directory(const Path&);
    Directory(const std::string&);

    std::vector<Path> getFiles() const;
};

class DLL_PUBLIC File: public Path {
public:
    File(const Path&);
    File(const std::string&);

    std::unique_ptr<std::istream> createIStream(bool binary = false) const;
    std::unique_ptr<std::iostream> createIOStream(bool binary = false);
};

}

//
// Created by Florian on 19.07.2020.
//
