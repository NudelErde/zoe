/*
 * Node.h
 *
 *  Created on: 24.12.2019
 *      Author: florian
 */

#pragma once

#include "../../Core.h"
#include "../../core/XMLParser.h"
#include "../../math/mat.h"
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <sstream>

namespace Zoe{

class DLL_PUBLIC Node{
public:
	Node();
	virtual ~Node(){};

	virtual void tick(double delta) = 0;
	virtual void draw(mat4x4 mat) = 0;

	inline std::shared_ptr<Node> getNode(unsigned int index) {return nodes[index];};
	inline std::shared_ptr<Node> getParent() {return parent.lock();}

	virtual void init(XMLNode& node) = 0;

	virtual void add(std::shared_ptr<Node> node);
	virtual void remove(unsigned int index);

	virtual void setByKey(std::string key, std::string value){};
	virtual void setByKey(std::string key, double value){};
	virtual std::string getStringByKey(std::string key){return "";};
	virtual double getDoubleByKey(std::string key){return 0;};
protected:
	void createChildren(XMLNode& node);
	std::vector<std::shared_ptr<Node>> nodes;
	std::weak_ptr<Node> self;
	std::weak_ptr<Node> parent;

	friend long _addChild(long ptr, std::string xml);
};

extern std::map<std::string,std::function<std::shared_ptr<Node>()>> constructors;
template<typename T>
DLL_PUBLIC void registerNode(std::string name){
	constructors[name] = [](){return std::make_shared<T>();};
}

inline DLL_PUBLIC std::function<std::shared_ptr<Node>()> getConstructor(std::string name){
	return constructors[name];
}

template<typename T>
inline T fromString(const std::string& str){
	std::stringstream stringstream(str);
	T t;
	stringstream >> t;
	return t;
}

}
