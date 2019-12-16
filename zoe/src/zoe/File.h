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

namespace Zoe{

class DLL_PUBLIC File{
public:
	File(const std::string& name);
	~File();

	std::istream* getInputStream() const;
private:
	bool m_virtual;
	std::string m_path;
};

void registerVirtualFile(const std::string& path, const std::string& content);
std::shared_ptr<std::string> getVirtualFileContent(const std::string& path);

}
