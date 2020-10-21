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

/**
 * A FileError is thrown if some error occurred while performing operations on a Path, File or Directory.
 */
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

/**
 * A Path encapsulates a pathname. It can implicitly converted to Directory and File.
 * The Path points to a virtual directory ore file if the pathname starts with `virtual/`.
 * A virtual filepath should start with `virtual/{project name}`.
 */
class DLL_PUBLIC Path {
public:
    /**
     * Creates a Path by a specified pathname.
     * @param name the specified pathname
     */
    Path(const std::string & name);

    /**
     * Checks if this path corresponds to an existing file or directory.
     * @returns `true` if this file or directory exists
     */
    [[nodiscard]] bool exists() const;

    /**
     * Checks if this path corresponds to an existing file.
     * @return `true` if this path corresponds to a file
     */
    [[nodiscard]] bool isFile() const;

    /**
     * Check if this path corresponds to an existing directory.
     * @return `true` if this path corresponds to a directory
     */
    [[nodiscard]] bool isDirectory() const;

    /**
     * Gets the parent directory of this path.
     * @return the parent directory
     */
    [[nodiscard]] Directory getParent() const;

    /**
     * Gets the absolute pathname of this path.
     * @return the absolute pathname
     */
    [[nodiscard]] std::string getAbsolutePath();

    /**
     * Gets the absolute pathname of this path.
     * @return the absolute pathname
     */
    [[nodiscard]] std::string getAbsolutePath() const;

    /**
     * Gets the path this object was created with.
     * @return the pathname
     */
    [[nodiscard]] inline const std::string &getPath() const {
        return m_path;
    }

protected:
    /**
     * The path of this file.
     */
    std::string m_path;

    /**
     * The absolute path of this file.
     */
    std::string absolutPath;

    /**
     * is `true` if the file is virtual.
     */
    bool m_isVirtual;
};

/**
 * Encapsulates a path that is guaranteed to be a directory or that could be a directory
 */
class DLL_PUBLIC Directory : public Path {
public:
    /**
     * Creates a directory from a specified path.
     * @param path the specified path
     */
    Directory(const Path & path);

    /**
     * Creates a directory from a specified pathname.
     * @param name the specified pathname
     */
    Directory(const std::string & name);

    /**
     * Returns the files and directories in this directory.
     * @return
     */
    [[nodiscard]] std::vector<Path> getFiles() const;

    /**
     * Creates this directory. Creates parent directories if needed.
     */
    void create() const;

    /**
     * Deletes this directory and the content of this directory.
     */
    void remove() const;
};

/**
 * Encapsulates a path that is guaranteed to be a file or that could be a file
 */
class DLL_PUBLIC File : public Path {
public:
    /**
     * Creates a file from a specified path.
     * @param path the specified path
     */
    File(const Path & path);

    /**
     * Creates a file from a specified pathname.
     * @param name the specified pathname
     */
    File(const std::string & name);

    /**
     * Creates an IStream for this file. The default is to open this stream in text mode.
     * @param binary `true` if this stream should be opened in binary mode
     * @return the new IStream
     */
    [[nodiscard]] std::unique_ptr<std::istream> createIStream(bool binary = false) const;

    /**
     * Creates an IOStream for this file. The default is to open this stream in text mode.
     * @param binary `true` if this stream should be opened in binary mode
     * @return the new IOStream
     */
    [[nodiscard]] std::unique_ptr<std::iostream> createIOStream(bool binary = false) const;

    /**
     * Reads the complete content of this file. The byte array is returned. The size is writen to the size_t pointed by
     * the `size` parameter.
     * If size is `nullptr` the size is not writen anywhere.
     * @param size a pointer to size_t where the filesize is written
     * @return
     */
    [[nodiscard]] std::unique_ptr<uint8_t[]> getContent(size_t * size = nullptr) const;

    /**
     * Creates this file. Creates parent directories if needed.
     */
    void create() const;

    /**
     * Clears the content of this file.
     */
    void clear() const;

    /**
     * Removes this file.
     */
    void remove() const;
};

/**
 * Implementation of streambuf for virtual files.
 * This class contains the logic for writing and reading bytes to and from a file.
 * @see https://en.cppreference.com/w/cpp/io/basic_streambuf
 */
class VirtualStreambuf : public std::streambuf {
public:
    using std::streambuf::traits_type;

    /**
     * Creates a stream to the specified file.
     * @param file the specified file
     * @param write `true` if writing should be allowed
     * @param read  `true` if reading should be allowed
     */
    VirtualStreambuf(File file, bool write, bool read);

    /**
     * Destructs the stream object
     */
    ~VirtualStreambuf() override;

    /**
     * Repositions the next pointer in the input sequence, output sequence, or both, using relative addressing.
     * @return the absolute position of the position indicator
     * @see https://en.cppreference.com/w/cpp/io/basic_streambuf/pubseekoff
     */
    pos_type seekoff(off_type, std::ios_base::seekdir, std::ios_base::openmode) override;

    /**
     * Repositions the next pointer in the input sequence, output sequence, or both using absolute addressing.
     * @returns the absolute position of the position indicator
     * @see https://en.cppreference.com/w/cpp/io/basic_streambuf/pubseekpos
     */
    pos_type seekpos(pos_type, std::ios_base::openmode) override;

    /**
     * Writes characters to the associated output sequence from the put area.
     * Empties the putArea.
     * This method is called if the putArea is full.
     * @returns Returns unspecified value not equal to `Traits::eof()` on success, `Traits::eof()` on failure
     * @see https://en.cppreference.com/w/cpp/io/basic_streambuf/overflow
     */
    int_type overflow(int_type) override;

    /**
     * Reads characters from the associated input sequence to the get area.
     * Ensures that at least one character is available in the input area.
     * This method is called if the getArea is empty.
     * @returns The value of the character pointed to by the get pointer after the call on success, or `Traits::eof()` otherwise
     * @see https://en.cppreference.com/w/cpp/io/basic_streambuf/underflow
     */
    int_type underflow() override;

    /**
     * Synchronizes the buffers with the associated character sequence.
     * Writes the output to the file and re-read the content in the input buffer.
     * @returns Returns `0` on success, `-1` otherwise
     * @see https://en.cppreference.com/w/cpp/io/basic_streambuf/pubsync
     */
    int sync() override;

private:
    File file;
    char getArea[128]{};
    char putArea[128]{};
    pos_type pos;
    bool write;
    bool read;
};

/**
 * Implementation of istream for virtual files.
 */
class VirtualIStream : public std::istream {
public:
    /**
     * Creates a VirtualIStream for the given file. The file must be virtual.
     * @param file the specified file
     */
    explicit VirtualIStream(const File & file);

    /***
     * Destructs the istream and releases all attached resources.
     */
    ~VirtualIStream() override;
};

/**
 * Implementation of ostream for virtual files.
 */
class VirtualOStream : public std::ostream {
public:
    /**
     * Creates a VirtualOStream for the given file. The file must be virtual.
     * @param file the specified file
     */
    explicit VirtualOStream(const File & file);

    /**
     * Destructs the ostream and releases all attached resources.
     */
    ~VirtualOStream() override;
};

/**
 * Implementation of iostream for virtual files.
 */
class VirtualIOStream : public std::iostream {
public:
    /**
     * Creates a VirtualIOStream for the given file. The file must be virtual.
     * @param file the specified file
     */
    explicit VirtualIOStream(const File & file);

    /**
     * Destructs the iostream and releases all attached resources.
     */
    ~VirtualIOStream() override;
};

}