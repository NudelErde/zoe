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

namespace Zoe{

static std::unordered_map<std::string, std::shared_ptr<std::string>> virtualFileContentMap;

File::File(const std::string& name): m_path(name) {
	if(getVirtualFileContent(name)){
		m_virtual = true;
	}else{
		m_virtual = false;
	}
}

File::~File() {

}

std::istream* File::getInputStream() const {
	if(m_virtual){
		return new std::stringstream(*getVirtualFileContent(m_path));
	}else{
		return new std::ifstream(m_path,std::ios::in);
	}
}

void registerVirtualFile(const std::string& path,
		const std::string& content) {
	virtualFileContentMap[path] = std::make_shared<std::string>(content);
}

std::shared_ptr<std::string> getVirtualFileContent(const std::string& path) {
	return virtualFileContentMap[path];
}



}

