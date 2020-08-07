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
#include <filesystem>
#include <algorithm>

namespace Zoe {

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

}
