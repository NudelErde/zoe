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

namespace Zoe {

    static std::unordered_map<std::string, std::shared_ptr<std::string>> virtualFileContentMap;

    File::File(const std::string &name) : m_path(name) {
        if (getVirtualFileContent(name)) {
            m_virtual = true;
        } else {
            m_virtual = false;
        }
    }

    File::~File() {

    }

    std::shared_ptr<std::istream> File::getInputStream() const {

        if (m_virtual) {
            return std::make_shared<std::stringstream>(*getVirtualFileContent(m_path));
        } else {
            std::ifstream exist(m_path);
            if (exist.good()) {
                return std::make_shared<std::ifstream>(m_path, std::ios::in);
            } else {
                std::stringstream errorMessage;
                errorMessage << "Could not open file: " << m_path;
                throw std::runtime_error(errorMessage.str());
            }
        }
    }

    std::unique_ptr<uint8_t[]> File::getByteArray(size_t *size) const {
        std::shared_ptr<std::istream> stream = getInputStream();
        stream->seekg(0, std::ios::end);
        size_t len = stream->tellg();
        if (size != 0) {
            *size = len;
        }
        std::unique_ptr<uint8_t[]> ret = std::make_unique<uint8_t[]>(len);
        stream->seekg(0, std::ios::beg);
        stream->read((char *) (ret.get()), len);
        return ret;
    }

    std::string File::getName() const {
        return m_path;
    }

    void registerVirtualFile(const std::string &path,
                             const std::string &content) {
        virtualFileContentMap[path] = std::make_shared<std::string>(content);
    }

    std::shared_ptr<std::string> getVirtualFileContent(const std::string &path) {
        return virtualFileContentMap[path];
    }

    bool File::operator==(const File &file) {
        return m_path == file.m_path;
    }

    bool File::operator!=(const File &file) {
        return !(*this == file);
    }

}

