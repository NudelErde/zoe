/*
 * File.cpp
 *
 *  Created on: 18.11.2019
 *      Author: florian
 */

#include "File.h"
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>
#include <utility>
#include <filesystem>
#include <string>
#include <regex>
#include <functional>
#include <set>
#include "Console.h"
#include "String.h"

//TODO: test this shit

namespace Zoe {

struct VirtualFileData { //TODO: pack executable and zip for some files? runtime created files to temp file if to big to save resources?
    std::map<std::string, std::vector<char>> virtualDataMap;
    std::map<std::string, std::vector<std::string>> virtualDirectoryMap;
    std::set<std::string> isFile;
    std::set<std::string> isDirectory;
    VirtualFileData(){
        isDirectory.insert("virtual/");
    }
};

static std::shared_ptr<VirtualFileData> fileData = std::make_shared<VirtualFileData>();

static bool isVirtualByPath(const std::string &path) {
    return path.rfind("virtual/", 0) == 0;
}

Path::Path(const std::string &path) : m_path(path), m_isVirtual(isVirtualByPath(path)), absolutPath() {}

bool Path::exists() const {
    if (!m_isVirtual) {
        std::error_code error;
        bool val = std::filesystem::exists(m_path, error);
        if (error) {
            throw FileError(m_path, error.message(), FileError::FileErrorCode::FilesystemError);
        }
        return val;
    } else {
        return fileData->isDirectory.count(getAbsolutePath()) || fileData->isFile.count(getAbsolutePath());
    }
}

bool Path::isFile() const {
    if (!m_isVirtual) {
        std::error_code error;
        bool val = std::filesystem::is_regular_file(m_path, error);
        if (error) {
            throw FileError(m_path, error.message(), FileError::FileErrorCode::FilesystemError);
        }
        return val;
    } else {
        return fileData->isFile.count(getAbsolutePath());
    }
}

bool Path::isDirectory() const {
    if (!m_isVirtual) {
        std::error_code error;
        bool val = std::filesystem::is_directory(m_path, error);
        if (error) {
            throw FileError(m_path, error.message(), FileError::FileErrorCode::FilesystemError);
        }
        return val;
    } else {
        return fileData->isDirectory.count(getAbsolutePath());
    }
}

Directory Path::getParent() const {
    if (!m_isVirtual) {
        return Directory(std::filesystem::path(m_path).parent_path().string());
    } else {
        std::string absPath = getAbsolutePath();
        std::size_t found = absPath.find_last_of('/');
        return Directory(absPath.substr(0, found + 1));
    }
}

std::string Path::getAbsolutePath() const {
    if (!absolutPath.empty()) {
        return absolutPath;
    }
    if (!m_isVirtual) {
        std::error_code error;
        std::string val = std::filesystem::absolute(m_path, error).string();
        if (error) {
            throw FileError(m_path, error.message(), FileError::FileErrorCode::FilesystemError);
        }
        return val;
    } else {
        if(m_path == "virtual/")
            return m_path;
        std::stringstream result;
        std::vector<std::string> args = split(m_path, '/');
        std::string lastArg;
        bool first = true;
        for (const auto &arg : args) {
            if (!lastArg.empty() && arg == "..") {
                lastArg.clear();
                continue;
            } else if (arg.empty() || arg == ".") {
                continue;
            } else {
                if (!lastArg.empty()) {
                    result << (first ? "" : "/") << lastArg;
                    first = false;
                }
                lastArg = arg;
            }
        }
        result << '/' << lastArg;
        return result.str();
    }
}

std::string Path::getAbsolutePath() {
    if (absolutPath.empty()) {
        absolutPath = std::as_const(*this).getAbsolutePath();
    }
    return absolutPath;
}

Directory::Directory(const Path &path) : Path(path) {
    if (isFile()) {
        throw FileError(path.getPath(),
                        "Directory object could not be constructed. " + path.getPath() + " points to File",
                        FileError::FileErrorCode::DirectoryConstructorWithFilePath);
    }
}

Directory::Directory(const std::string &path) : Directory(Path(path)) {}

std::vector<Path> Directory::getFiles() const {
    std::vector<Path> result;
    if (m_isVirtual) {
        for (const auto &entry : std::filesystem::directory_iterator(m_path))
            result.emplace_back(entry.path().string());
    } else {
        for (const auto &path : fileData->virtualDirectoryMap[getAbsolutePath()]) {
            result.emplace_back(path);
        }
    }
    return result;
}

void Directory::create() const {
    if (!m_isVirtual) {
        std::error_code error;
        std::filesystem::create_directories(m_path, error);
        if (error) {
            throw FileError(m_path, error.message(), FileError::FileErrorCode::FilesystemError);
        }
    } else if (!isDirectory()) {
        Directory d = getParent();
        d.create();
        fileData->virtualDirectoryMap[d.getAbsolutePath()].push_back(getAbsolutePath());
        fileData->isDirectory.insert(getAbsolutePath());
    }
}

void Directory::remove() const {
    if (!m_isVirtual) {
        std::error_code error;
        std::filesystem::remove_all(m_path, error);
        if (error) {
            throw FileError(m_path, error.message(), FileError::FileErrorCode::FilesystemError);
        }
    } else if (isDirectory()) {
        if(m_path == "virtual/") //never delete virtual files root
            return;
        for (const auto &file : getFiles()) {
            if (file.isFile()) {
                File(file).remove();
            } else if (file.isDirectory()) {
                Directory(file).remove();
            }
        }
        std::string absPath = getAbsolutePath();
        fileData->virtualDataMap.erase(absPath);
        fileData->isDirectory.erase(absPath);
    }
}

File::File(const Path &path) : Path(path) {
    if (isDirectory()) {
        throw FileError(path.getPath(),
                        "File object could not be constructed. " + path.getPath() + " points to Directory",
                        FileError::FileErrorCode::FileConstructorWithDirectoryPath);
    }
}

File::File(const std::string &path) : File(Path(path)) {}

std::unique_ptr<std::istream> File::createIStream(bool binary) const {
    if (!m_isVirtual) {
        std::ios_base::openmode openmode = std::ios_base::in;
        if (binary) {
            openmode |= std::ios_base::binary;
        }
        return std::make_unique<std::ifstream>(m_path, openmode);
    } else {
        create();
        return std::make_unique<VirtualIStream>(*this);
    }
}

std::unique_ptr<std::iostream> File::createIOStream(bool binary) const {
    if (!m_isVirtual) {
        std::ios_base::openmode openmode = std::ios_base::in | std::ios_base::out;
        if (binary) {
            openmode |= std::ios_base::binary;
        }
        return std::make_unique<std::fstream>(m_path, openmode);
    } else {
        create();
        return std::make_unique<VirtualIOStream>(*this);
    }
}

void File::create() const {
    if (!m_isVirtual) {
        std::ofstream ostream(m_path, std::ios_base::out);
    } else if (!isFile()) {
        getParent().create();
        std::string path = getAbsolutePath();
        fileData->isFile.insert(path);
        fileData->virtualDataMap[path] = std::vector<char>();
    }
}

void File::clear() const {
    if (!m_isVirtual) {
        std::ofstream ostream(m_path, std::ios_base::out | std::ios_base::trunc);
    } else if (isFile()) {
        fileData->virtualDataMap[getAbsolutePath()].clear();
    }
}

void File::remove() const {
    if (m_isVirtual) {
        std::error_code error;
        std::filesystem::remove(m_path, error);
        if (error) {
            throw FileError(m_path, error.message(), FileError::FileErrorCode::FilesystemError);
        }
    } else if (isFile()) {
        std::string path = getAbsolutePath();
        fileData->isFile.erase(path);
        fileData->virtualDataMap.erase(path);
    }
}

std::unique_ptr<uint8_t[]> File::getContent(size_t * size) const{
    std::unique_ptr<std::istream> inputStream = createIStream(true);
    inputStream->seekg(0, std::ios::end);
    size_t length = inputStream->tellg();
    if(size != nullptr){
        *size = length;
    }
    std::unique_ptr<uint8_t[]> res = std::make_unique<uint8_t[]>(length);
    inputStream->seekg(0, std::ios::beg);
    inputStream->read((char*)res.get(), length);
    return res;
}

FileError::FileError(std::string file, std::string what, FileErrorCode errorCode) : m_file(std::move(file)),
                                                                                    m_what(std::move(what)),
                                                                                    errorCode(errorCode) {
    m_what = "Error in file " + m_file + ": " + m_what;
}

const char *FileError::what() const noexcept {
    return m_what.c_str();
}

const std::string &FileError::getFile() const {
    return m_file;
}

VirtualStreambuf::VirtualStreambuf(File file, bool write, bool read) : file(std::move(file)) {
    setg(getArea, getArea + 128, getArea + 128);
    setp(putArea, putArea + 128);
    pos = 0;
    VirtualStreambuf::write = write;
    VirtualStreambuf::read = read;
}

VirtualStreambuf::~VirtualStreambuf() = default;

VirtualStreambuf::pos_type VirtualStreambuf::seekoff(VirtualStreambuf::off_type offset,
                                                     std::ios_base::seekdir dir, std::ios_base::openmode which) {
    if (dir == std::ios_base::beg) {
        seekpos(0 + offset, which);
    } else if (dir == std::ios_base::end) {
        seekpos(fileData->virtualDataMap[file.getAbsolutePath()].size() + offset, which);
    } else { //relative position
        pos += offset;
    }
    sync();
    return pos;
}

VirtualStreambuf::pos_type VirtualStreambuf::seekpos(VirtualStreambuf::pos_type position,
                                                     std::ios_base::openmode which) {
    pos = position;
    return pos;
}

VirtualStreambuf::int_type VirtualStreambuf::overflow(VirtualStreambuf::int_type ch) {
    if (write) {
        fileData->virtualDataMap[file.getAbsolutePath()].insert(
                fileData->virtualDataMap[file.getAbsolutePath()].begin() + pos, pbase(), pptr());
        pos += pptr() - pbase();
        if (!traits_type::eq_int_type(ch, traits_type::eof())) {
            fileData->virtualDataMap[file.getAbsolutePath()].push_back(traits_type::to_char_type(ch));
            pos += 1;
        }
        memset(putArea, 0, 128);
        setp(putArea, putArea + 128);
        return traits_type::not_eof(0);
    } else {
        return traits_type::eof();
    }
}

VirtualStreambuf::int_type VirtualStreambuf::underflow() {
    if (read) {
        size_t readSize = std::min(fileData->virtualDataMap[file.getAbsolutePath()].size() - pos, (size_t) 128);
        if (readSize == 0) {
            setg(getArea, nullptr, getArea + 128);
            return traits_type::eof();
        }
        memcpy(getArea, fileData->virtualDataMap[file.getAbsolutePath()].data() + pos, readSize);
        pos += readSize;
        setg(getArea, getArea, getArea + readSize);
        return traits_type::not_eof(*getArea);
    } else {
        return traits_type::eof();
    }
}

int VirtualStreambuf::sync() {
    if (read) {
        size_t readSize = std::min(fileData->virtualDataMap[file.getAbsolutePath()].size() - pos, (size_t) 128);
        memcpy(getArea, fileData->virtualDataMap[file.getAbsolutePath()].data() + pos, readSize);
    }
    if (write) {
        fileData->virtualDataMap[file.getAbsolutePath()].insert(
                fileData->virtualDataMap[file.getAbsolutePath()].begin() + pos, pbase(), pptr());
        pos += pptr() - pbase();
        memset(putArea, 0, 128);
        setp(putArea, putArea + 128);
    }
    return 0;
}

VirtualIStream::VirtualIStream(const File &file) : std::istream(new VirtualStreambuf(file, false, true)) {}

VirtualIStream::~VirtualIStream() {
    delete rdbuf();
}

VirtualOStream::VirtualOStream(const File &file) : std::ostream(new VirtualStreambuf(file, true, false)) {}

VirtualOStream::~VirtualOStream() {
    flush();
    delete rdbuf();
}

VirtualIOStream::VirtualIOStream(const File &file) : std::iostream(new VirtualStreambuf(file, true, true)) {}

VirtualIOStream::~VirtualIOStream() {
    flush();
    delete rdbuf();
}
}
