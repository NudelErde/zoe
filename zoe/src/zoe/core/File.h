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

namespace Zoe {

class Directory;

class File;


class DLL_PUBLIC FileError : public std::exception {
public:
    enum class FileErrorCode {
        UNKNOWN,
        FileConstructorWithDirectoryPath,
        DirectoryConstructorWithFilePath,
        FilesystemError
    };

    FileError(std::string file, std::string what, FileErrorCode errorCode);

    [[nodiscard]] const char *what() const override;

    [[nodiscard]] const std::string &getFile() const;

    [[nodiscard]] inline const FileErrorCode& getErrorCode() const {
        return errorCode;
    }

private:
    std::string m_file, m_what;
    FileErrorCode errorCode;
};

class DLL_PUBLIC Path {
public:
    Path(const std::string &);

    [[nodiscard]] bool exists() const;

    [[nodiscard]] bool isFile() const;

    [[nodiscard]] bool isDirectory() const;

    [[nodiscard]] Directory getParent() const;

    [[nodiscard]] std::string getAbsolutePath();
    [[nodiscard]] std::string getAbsolutePath() const;

    [[nodiscard]] inline const std::string &getPath() const {
        return m_path;
    }

protected:
    std::string m_path;
    std::string absolutPath;
    bool m_isVirtual;
};

class DLL_PUBLIC Directory : public Path {
public:
    Directory(const Path &);

    Directory(const std::string &);

    [[nodiscard]] std::vector<Path> getFiles() const;

    void create() const;

    void remove() const;
};

class DLL_PUBLIC File : public Path {
public:
    File(const Path &);

    File(const std::string &);

    [[nodiscard]] std::unique_ptr<std::istream> createIStream(bool binary = false) const;

    [[nodiscard]] std::unique_ptr<std::iostream> createIOStream(bool binary = false) const;

    void create() const;

    void clear() const;

    void remove() const;
};

}