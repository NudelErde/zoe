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

uint8_t* File::getByteArray(size_t* size) const{
	std::istream* stream = getInputStream();
	stream->seekg( 0, std::ios::end );
	size_t len = stream->tellg();
	if(size!=0){
		*size = len;
	}
	char* ret = new char[len];
	stream->seekg(0, std::ios::beg);
	stream->read(ret, len);
	return (uint8_t*)ret;
}

std::string File::getName() const{
	return m_path;
}

void registerVirtualFile(const std::string& path,
		const std::string& content) {
	virtualFileContentMap[path] = std::make_shared<std::string>(content);
}

std::shared_ptr<std::string> getVirtualFileContent(const std::string& path) {
	return virtualFileContentMap[path];
}



}

