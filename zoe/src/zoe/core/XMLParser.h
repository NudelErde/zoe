/*
 * XMLParser.h
 *
 *  Created on: 21.12.2019
 *      Author: florian
 */

#pragma once

#include <vector>
#include <map>

#include "Core.h"
#include "File.h"

namespace Zoe{

/**
 * An XMLNode is the parsed node.
 * ```
 * <Name attributeKey1="attributeValue1" attributeKey2="attributeKey2" ...>
 *     Content...
 *
 *     <Child1>
 *         ...
 *     </Child1>
 *     <Child2>
 *         ...
 *     </Child2>
 *     ...
 * </Name>
 * ```
 */
struct DLL_PUBLIC XMLNode{
    /**
     * The name of this node.
     */
	std::string name;
	/**
	 * The content of this node.
	 */
	std::string content;
	/**
	 * The children of this node.
	 */
	std::vector<XMLNode> children;

	/**
	 * The attributes to this node.
	 */
	std::map<std::string,std::string> attributes;
};

/**
 * Parses an XMLFile to a XMLNode.
 * @todo parse stuff like <test/>
 * @param file the file.
 * @return the top most node
 */
DLL_PUBLIC XMLNode readXML(const File& file);

}

