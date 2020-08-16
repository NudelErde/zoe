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
#include "Console.h"
#include "String.h"

//TODO: test this shit

namespace Zoe {

struct VirtualFileData { //TODO: pack executable and zip for some files? runtime created files to temp file if to big to save resources?
    std::map<std::string, std::vector<uint8_t>> virtualDataMap;
    std::map<std::string, std::vector<std::string>> virtualDirectoryMap;
    std::map<std::string, bool> isFile;
    std::map<std::string, bool> isDirectory;
};

static std::shared_ptr<VirtualFileData> fileData;

static bool isVirtualByPath(const std::string &path) {
    return path.rfind("virtual/zoe/", 0) == 0;
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
        return fileData->isDirectory[getAbsolutePath()] || fileData->isFile[getAbsolutePath()];
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
        return fileData->isFile[getAbsolutePath()];
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
        return fileData->isDirectory[getAbsolutePath()];
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
    if(!absolutPath.empty()){
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
        std::stringstream result;
        std::vector<std::string> args = split(m_path, '/');
        std::string lastArg;
        bool first = true;
        for(const auto & arg : args){
            if (!lastArg.empty() && arg == "..") {
                lastArg.clear();
                continue;
            } else if (arg.empty() || arg == ".") {
                continue;
            } else {
                if (!lastArg.empty()) {
                    result << (first?"":"/") << lastArg;
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
    if(!absolutPath.empty()){
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
    } else {
        if (!isDirectory()) {
            Directory d = getParent();
            d.create();
            fileData->virtualDirectoryMap[d.getAbsolutePath()].push_back(getAbsolutePath());
            fileData->isDirectory[getAbsolutePath()] = true;
        }
    }
}

void Directory::remove() const {
    if (!m_isVirtual) {
        std::error_code error;
        std::filesystem::remove_all(m_path, error);
        if (error) {
            throw FileError(m_path, error.message(), FileError::FileErrorCode::FilesystemError);
        }
    } else {
        if (isDirectory()) {
            for (const auto &file : getFiles()) {
                if (file.isFile()) {
                    File(file).remove();
                }else if(file.isDirectory()){
                    Directory(file).remove();
                }
            }
            std::string absPath = getAbsolutePath();
            fileData->virtualDataMap.erase(absPath);
            fileData->isDirectory.erase(absPath);
        }
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
        return createIOStream(binary);
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
        //TODO: Implement iostream
    }
}

void File::create() const {
    if (!m_isVirtual) {
        std::ofstream ostream(m_path, std::ios_base::out);
    } else {
        if(!isFile()){
            getParent().create();
            std::string path = getAbsolutePath();
            fileData->isFile[path] = true;
            fileData->virtualDataMap[path] = std::vector<uint8_t>();
        }
    }
}

void File::clear() const {
    if (!m_isVirtual) {
        std::ofstream ostream(m_path, std::ios_base::out | std::ios_base::trunc);
    } else {
        if(isFile()){
            fileData->virtualDataMap[getAbsolutePath()].clear();
        }
    }
}

void File::remove() const {
    if (m_isVirtual) {
        std::error_code error;
        std::filesystem::remove(m_path, error);
        if (error) {
            throw FileError(m_path, error.message(), FileError::FileErrorCode::FilesystemError);
        }
    } else {
        if(isFile()){
            std::string path = getAbsolutePath();
            fileData->isFile.erase(path);
            fileData->virtualDataMap.erase(path);
        }
    }
}

FileError::FileError(std::string file, std::string what, FileErrorCode errorCode) : m_file(std::move(file)),
                                                                                    m_what(std::move(what)),
                                                                                    errorCode(errorCode) {
    m_what = "Error in file " + m_file + ": " + m_what;
}

const char *FileError::what() const {
    return m_what.c_str();
}

const std::string &FileError::getFile() const {
    return m_file;
}
}

/*
std::map<std::string, std::string> virtualFiles;

File::File(const std::string &path) : File(path, getFileTypeByPath(path)) {}

File::File(const std::string &path, const File::FileType &type, bool binary) {
    m_path = path;
    std::replace(m_path.begin(), m_path.end(), '\\', '/');
    m_type = type;
    m_binary = binary;
    if (type == FileType::VIRTUAL) {
        m_iostream = std::make_unique<std::stringstream>(virtualFiles[m_path]);
    } else if (type == FileType::FILE_SYSTEM) {
        if (m_path.find('/') != std::string::npos) {
            std::string dirPath = m_path.substr(0, m_path.find_last_of('/'));
            std::filesystem::create_directories(dirPath);
        }
        if(!isFile()){
            std::stringstream errorMessage;
            errorMessage << "File " << m_path << " could not be found";
            throw std::runtime_error(errorMessage.str());
        }
	std::ios_base::openmode openmode = std::fstream::in | std::fstream::out;
	if(binary){
	    openmode |= std::fstream::binary;
	}
        m_iostream = std::make_unique<std::fstream>(m_path, openmode);
    }
}

File::File(const File &other) {
    m_path = other.m_path;
    m_type = other.m_type;
    m_binary = other.m_binary;
    if (m_type == FileType::VIRTUAL) {
        m_iostream = std::make_unique<std::stringstream>(virtualFiles[m_path]);
    } else if (m_type == FileType::FILE_SYSTEM) {
        if (m_path.find('/') != std::string::npos) {
            std::string dirPath = m_path.substr(0, m_path.find_last_of('/'));
            std::filesystem::create_directories(dirPath);
        }
        if(!isFile()){
            std::stringstream errorMessage;
            errorMessage << "File " << m_path << " could not be found";
            throw std::runtime_error(errorMessage.str());
        }
	std::ios_base::openmode openmode = std::fstream::in | std::fstream::out;
	if(m_binary){
	    openmode |= std::fstream::binary;
	}
        m_iostream = std::make_unique<std::fstream>(m_path, openmode);
    }
}

File::~File() {
    if (m_type == FileType::VIRTUAL) {
	//save content of virtual file
        virtualFiles[m_path] = dynamic_cast<std::stringstream *>(m_iostream.get())->str();
    }
}

File &File::operator=(const File &other) {
    if (&other == this)
        return *this;

    if (m_type == FileType::VIRTUAL) {
	//save content of virtual file
        virtualFiles[m_path] = dynamic_cast<std::stringstream *>(m_iostream.get())->str();
    }

    m_path = other.m_path;
    m_type = other.m_type;
    m_binary = other.m_binary;
    if (m_type == FileType::VIRTUAL) {
        m_iostream = std::make_unique<std::stringstream>(virtualFiles[m_path]);
    } else if (m_type == FileType::FILE_SYSTEM) {
        if (m_path.find('/') != std::string::npos) {
            std::string dirPath = m_path.substr(0, m_path.find_last_of('/'));
            std::filesystem::create_directories(dirPath);
        }
	std::ios_base::openmode openmode = std::fstream::in | std::fstream::out;
	if(m_binary){
	    openmode |= std::fstream::binary;
	}
        m_iostream = std::make_unique<std::fstream>(m_path, openmode);
    }
    return *this;
}

File::FileType File::getFileTypeByPath(const std::string &path) {
    if (path.rfind("zoe/internal/", 0) == 0) {
        return FileType::VIRTUAL;
    } else {
        return FileType::FILE_SYSTEM;
    }
}

std::iostream &File::getIOStream() {
    return *m_iostream;
}

std::unique_ptr<uint8_t[]> File::getContent(size_t *size) const{
    std::unique_ptr<std::istream> stream = createIStream();
    stream->seekg(0, std::ios::end);
    size_t len = stream->tellg();
    if (size != nullptr) {
        *size = len;
    }
    std::unique_ptr<uint8_t[]> ret = std::make_unique<uint8_t[]>(len);
    stream->seekg(0, std::ios::beg);
    std::copy(std::istreambuf_iterator<char>(*stream), std::istreambuf_iterator<char>(), ret.get());
    return ret;
}

const std::string &File::getPath() const{
    return m_path;
}

std::unique_ptr<std::istream> File::createIStream(bool binary) const {
    if (m_type == FileType::VIRTUAL) {
        return std::make_unique<std::istringstream>(dynamic_cast<std::stringstream*>(m_iostream.get())->str());
    } else if (m_type == FileType::FILE_SYSTEM) {
        if (m_path.find('/') != std::string::npos) {
            std::string dirPath = m_path.substr(0, m_path.find_last_of('/'));
            std::filesystem::create_directories(dirPath);
        }
	std::ios_base::openmode openmode = std::ifstream::in;
	if(binary){
	    openmode |= std::ifstream::binary;
	}
        return std::make_unique<std::ifstream>(m_path, openmode);
    }
    throw std::runtime_error("istream could not be created. File has invalid type");
}

void File::createFile() {
    if(!isFile()){
        std::ofstream ofstream(m_path, std::fstream::out);
        ofstream.close();
    }
}

bool File::isFile() {
    return std::filesystem::exists(m_path);
}
*/