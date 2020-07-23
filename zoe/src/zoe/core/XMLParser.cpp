/*
 * XMLParser.cpp
 *
 *  Created on: 21.12.2019
 *      Author: florian
 */

#include "XMLParser.h"
#include <sstream>
#include <stack>
#include <algorithm>
#include <cctype>
#include <locale>
#include <utility>

namespace Zoe {

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

static inline std::string readString(std::string& in){
    const char* cstr = in.c_str();
    std::stringstream stringstream;
    const size_t length = in.length();
    bool escape = false;
    if(cstr[0] != '"')
        return "";
    for(size_t i = 1; i < length; ++i){
        if(cstr[i] == '\\'){
            escape = true;
        }else if(cstr[i] == '"' && !escape){
            in = in.substr(i+1);
            return stringstream.str();
        }else{
            stringstream << cstr[i];
            escape = false;
        }
    }
    in = "";
    return stringstream.str();
}

static std::string readTo(std::unique_ptr<std::istream>& stream, char ch){
	std::stringstream stringstream;
	int ret = 0;
	char c = 0;

	while( (ret = stream->get()) != EOF){
		c = (char) ret;
		if(c == ch){
			return stringstream.str();
		}else{
			stringstream << c;
		}
	}
	return stringstream.str();
}

static void parseNameAndAttributes(XMLNode& node, std::string nameAndAttributes){
	trim(nameAndAttributes);
	std::size_t found = nameAndAttributes.find(' ');
	if(found == std::string::npos){
		node.name = nameAndAttributes;
	}else {
		node.name = nameAndAttributes.substr(0, found);
		nameAndAttributes = nameAndAttributes.substr(found);
		while(true){
			trim(nameAndAttributes);
			if((found = nameAndAttributes.find('=')) == std::string::npos){
				break;
			}
			std::string key = nameAndAttributes.substr(0, found);
			trim(key);
			nameAndAttributes = nameAndAttributes.substr(found+1);
			trim(nameAndAttributes);
			std::string value = readString(nameAndAttributes);
			node.attributes[key] = value;
		}
	}
}

static XMLNode parse(std::unique_ptr<std::istream>& stream, std::string tag);

static void parseContent(XMLNode& node, std::unique_ptr<std::istream>& stream){
	std::string terminate = "/" + node.name;
	std::stringstream sstream;

	while(true){
		std::string contentElement = readTo(stream, '<');
		trim(contentElement);
		sstream << contentElement;
		std::string tag = readTo(stream, '>');
		if(tag == terminate){
			node.content = sstream.str();
			return;
		}
		XMLNode child = parse(stream,tag);
		node.children.push_back(child);
	}
}

static XMLNode parse(std::unique_ptr<std::istream>& stream, std::string tag){
	XMLNode top;
	parseNameAndAttributes(top, std::move(tag));
	parseContent(top, stream);
	return top;
}

static XMLNode parse(std::unique_ptr<std::istream>& stream){
	XMLNode top;
	readTo(stream, '<'); //find start;
	std::string nameAndAttributes = readTo(stream, '>');
	parseNameAndAttributes(top, nameAndAttributes);
	parseContent(top, stream);
	return top;
}



XMLNode readXML(const File& file) {
	std::unique_ptr<std::istream> stream = file.createIStream(false);

	return parse(stream);
}

}
