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
    /**
     * Error codes for class FileError
     */
    enum class FileErrorCode {
        UNKNOWN,
        FileConstructorWithDirectoryPath,
        DirectoryConstructorWithFilePath,
        FilesystemError
    };

    /**
     * Constructs FileError.
     * Errors from std::filesystem are caught and rethrown as FileErrors. In this case errorCode = FileErrorCode::FilesystemError.
     * errorCode is an element from FileErrorCode. It indicates what kind of error was thrown
     * @param file is the file from which the FileError was thrown
     * @param what is the value returned by what()
     * @param errorCode is an element from FileErrorCode
     */
    FileError(std::string file, std::string what, FileErrorCode errorCode);

    /**
     * Pointer to a null-terminated string with explanatory information. The pointer is guaranteed to be valid at least
     * until the exception object from which it is obtained is destroyed, or until a non-const member function on the
     * exception object is called.
     * @returns an explanatory string
     * @see https://en.cppreference.com/w/cpp/error/exception/what
     */
    [[nodiscard]] const char *what() const noexcept override;

    /**
     *
     * @returns name of the file from which the file FileError was thrown
     */
    [[nodiscard]] const std::string &getFile() const;

    /**
     * errorCode is an element from FileErrorCode. It indicates what kind of error was thrown
     * @returns the error code from the exception
     */
    [[nodiscard]] inline const FileErrorCode &getErrorCode() const {
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

    [[nodiscard]] std::unique_ptr<uint8_t[]> getContent(size_t * = nullptr) const;

    void create() const;

    void clear() const;

    void remove() const;
};

class VirtualStreambuf : public std::streambuf {
public:
    using std::streambuf::traits_type;

    VirtualStreambuf(File, bool write, bool read);

    ~VirtualStreambuf() override;

    pos_type seekoff(off_type, std::ios_base::seekdir, std::ios_base::openmode) override;

    pos_type seekpos(pos_type, std::ios_base::openmode) override;

    int_type overflow(int_type) override;

    int_type underflow() override;

    int sync() override;

private:
    File file;
    char getArea[128]{};
    char putArea[128]{};
    pos_type pos;
    bool write;
    bool read;
};

//TODO: only move no copy
class VirtualIStream : public std::istream {
public:
    explicit VirtualIStream(const File &);

    ~VirtualIStream() override;
};

class VirtualOStream : public std::ostream {
public:
    explicit VirtualOStream(const File &);

    ~VirtualOStream() override;
};

class VirtualIOStream : public std::iostream {
public:
    explicit VirtualIOStream(const File &);

    ~VirtualIOStream() override;
};

}