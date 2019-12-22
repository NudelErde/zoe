/*
 * XMLParser.h
 *
 *  Created on: 21.12.2019
 *      Author: florian
 */

#pragma once

#include <vector>
#include <map>

#include "../Core.h"
#include "../File.h"

namespace Zoe{

struct DLL_PUBLIC XMLNode{
	std::string name;
	std::string content;
	std::vector<XMLNode> children;
	std::map<std::string,std::string> attributes;
};

static DLL_PUBLIC XMLNode readXML(const File& file);

}

