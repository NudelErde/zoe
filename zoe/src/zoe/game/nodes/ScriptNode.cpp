/*
 * ScriptNode.cpp
 *
 *  Created on: 29.12.2019
 *      Author: florian
 */

#include "ScriptNode.h"
#include <chaiscript/chaiscript.hpp>
#include "Node.h"
#include "../../Console.h"
#include "../../KeyCode.h"
#include "../../Input.h"

namespace Zoe {

static const char* scriptInit =
		R"(
def debug(x){
	_debug(to_string(x));
}
def info(x){
	_info(to_string(x));
}
def warning(x){
	_warning(to_string(x));
}
def erro(x){
	_error(to_string(x));
}
def critical(x){
	_critical(to_string(x));
}

class Node{
	var _ptr;
	def Node(long ptr) {this._ptr = ptr;}

	def getNumber(key) {
		return _getDoubleByKey(this._ptr, to_string(key));
	}
	def getString(key) {
		return _getStringByKey(this._ptr, to_string(key));
	}
	def set(key, value) {
		_setByKey(this._ptr, to_string(key), value);
	}
	def getParent(){
		return Node(_getParentPointer(this._ptr));
	}
	def getChild(index){
		return Node(_getChildPointer(this._ptr, index));
	}
	def addChild(xml){
		return Node(_addChild(this._ptr, xml));
	}
	def removeChild(index){
		_remvoveChild(this._ptr, index);
	}
}
)";

static void _setByKey(long ptr, std::string key, std::string value) {
	Node* node = (Node*) (void*) (ptr);
	node->setByKey(key, value);
}
static void _setByKey(long ptr, std::string key, double value) {
	Node* node = (Node*) (void*) (ptr);
	node->setByKey(key, value);
}
static std::string _getStringByKey(long ptr, std::string key) {
	Node* node = (Node*) (void*) (ptr);
	return node->getStringByKey(key);
}
static double _getDoubleByKey(long ptr, std::string key) {
	Node* node = (Node*) (void*) (ptr);
	return node->getDoubleByKey(key);
}
static long _getParentPointer(long ptr) {
	Node* node = (Node*) (void*) (ptr);
	return (long) (&(*(node->getParent())));
}
static long _getChildPointer(long ptr, unsigned int index) {
	Node* node = (Node*) (void*) (ptr);
	return (long) (&(*(node->getNode(index))));
}
long _addChild(long ptr, std::string xml) {
	Node* node = (Node*) (void*) (ptr);
	registerVirtualFile("temp.xml", xml);
	XMLNode xmlNode = readXML(File("temp.xml"));
	std::function<std::shared_ptr<Node>()> constructor = getConstructor(
			xmlNode.name);
	if (constructor) {
		std::shared_ptr<Node> child = constructor();
		node->add(child);
		child->createChildren(xmlNode);
		child->init(xmlNode);
		return (long) (void*) (&(*child));
	}
	return 0;
}
static void _removeChild(long ptr, unsigned int index) {
	Node* node = (Node*) (void*) (ptr);
	node->remove(index);
}

static bool isKeyPressed(unsigned int keyCode) {
	return Input::isKeyPressed(keyCode);
}

static long time() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now().time_since_epoch()).count();
}

ScriptNode::ScriptNode() {
	tickCount = 0;
	script = std::make_shared<chaiscript::ChaiScript>();
	script->add(chaiscript::fun(static_cast<void (*)(std::string)>(&debug)), "_debug");
	script->add(chaiscript::fun(static_cast<void (*)(std::string)>(&info)), "_info");
	script->add(chaiscript::fun(static_cast<void (*)(std::string)>(&warning)), "_warning");
	script->add(chaiscript::fun(static_cast<void (*)(std::string)>(&error)), "_error");
	script->add(chaiscript::fun(static_cast<void (*)(std::string)>(&critical)), "_critical");
	script->add(
			chaiscript::fun(
					static_cast<void (*)(long, std::string,
							std::string)>(&_setByKey)), "_setByKey");
	script->add(
			chaiscript::fun(
					static_cast<void (*)(long, std::string,
							double)>(&_setByKey)), "_setByKey");
	script->add(chaiscript::fun(&_getStringByKey), "_getStringByKey");
	script->add(chaiscript::fun(&_getDoubleByKey), "_getDoubleByKey");
	script->add(chaiscript::fun(&_getParentPointer), "_getParentPointer");
	script->add(chaiscript::fun(&_getChildPointer), "_getChildPointer");
	script->add(chaiscript::fun(&_addChild), "_addChild");
	script->add(chaiscript::fun(&_removeChild), "_removeChild");
	script->add(chaiscript::fun(&time), "time");
	script->add(chaiscript::fun(&isKeyPressed), "isKeyPressed");
	for (auto& entry : keyMap) {
		script->add_global_const(chaiscript::const_var(entry.second),
				entry.first);
	}
	script->eval(scriptInit);
}

ScriptNode::~ScriptNode() {
}

void ScriptNode::tick(double delta) {
	if (tickCount == 0 && initFunction != 0) {
		initFunction();
	}
	++tickCount;
	if (tickFunction != 0) {
		tickFunction(delta);
	}
}

void ScriptNode::draw(mat4x4 mat) {
}

void ScriptNode::init(XMLNode& node) {
	std::stringstream sstream;
	//set parent node in script
	sstream << "global parent = Node(" << (long) (&(*getParent())) << ")"
			<< std::endl;
	script->eval(sstream.str());

	if (!node.content.empty()) {
		script->eval(node.content);
	}
	if (!node.attributes["src"].empty()) {
		script->eval(
				std::string(
						std::istreambuf_iterator<char>(
								*(File(node.attributes["src"]).getInputStream())),
						{ }));
	}
	try {
		tickFunction = script->eval<std::function<void(double)>>("tick");
	} catch (const std::exception& e) {
		tickFunction = 0;
	}
	try {
		initFunction = script->eval<std::function<void()>>("init");
	} catch (const std::exception& e) {
		initFunction = 0;
	}
}

}
