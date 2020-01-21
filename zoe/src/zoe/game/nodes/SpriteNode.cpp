/*
 * SpriteNode.cpp
 *
 *  Created on: 25.12.2019
 *      Author: florian
 */

#include "SpriteNode.h"
#include "../../Console.h"

namespace Zoe {

SpriteNode::SpriteNode() :
		pos( { 0, 0, 0 }) {
}

SpriteNode::~SpriteNode() {
}

void SpriteNode::tick(double delta) {
	//iterate with index, because vector can change!
	for (unsigned int i = 0; i<nodes.size(); ++i) {
		nodes[i]->tick(delta);
	}
}

void SpriteNode::draw(mat4x4 mat) {
	mat = mat * translate3D(pos.x, pos.y, pos.z);
	for (std::shared_ptr<Node>& child : nodes) {
		child->draw(mat);
	}
}

void SpriteNode::init(XMLNode& node) {
	std::string value = "";
	if (!(value = node.attributes["x"]).empty()) {
		pos.x = fromString<float>(value);
	}
	if (!(value = node.attributes["y"]).empty()) {
		pos.y = fromString<float>(value);
	}
	if (!(value = node.attributes["z"]).empty()) {
		pos.z = fromString<float>(value);
	}
}

void SpriteNode::setByKey(std::string key, std::string value) {
}

void SpriteNode::setByKey(std::string key, double value) {
	if(key == "x"){
		pos.x = value;
	}else if(key == "y"){
		pos.y = value;
	}else if(key == "z"){
		pos.z = value;
	}
}

std::string SpriteNode::getStringByKey(std::string key) {
	return "";
}

double SpriteNode::getDoubleByKey(std::string key) {
	if(key == "x"){
		return pos.x;
	}else if(key == "y"){
		return pos.y;
	}else if(key == "z"){
		return pos.z;
	}
	return 0;
}

}
