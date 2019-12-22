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

class DLL_PUBLIC File{
public:
	File(const std::string& name);
	~File();

	std::shared_ptr<std::istream> getInputStream() const;
	std::unique_ptr<uint8_t[]> getByteArray(size_t* size = 0) const;

	std::string getName() const;

	bool operator==(const File& other);
	bool operator!=(const File& other);
private:
	bool m_virtual;
	std::string m_path;
};

void registerVirtualFile(const std::string& path, const std::string& content);
std::shared_ptr<std::string> getVirtualFileContent(const std::string& path);

}
