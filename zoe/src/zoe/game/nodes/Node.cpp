/*
 * Node.cpp
 *
 *  Created on: 24.12.2019
 *      Author: florian
 */

#include "Node.h"
#include "../../Console.h"

namespace Zoe{

std::map<std::string,std::function<std::shared_ptr<Node>()>> constructors;

Node::Node(){
}

void Node::add(std::shared_ptr<Node> node){
	if(self.expired()){
		error("Can not add node to unattached node");
	}else{
		node->self = node;
		node->parent = self;
		nodes.push_back(node);
	}
}

void Node::remove(unsigned int index){
	nodes.erase(nodes.begin()+index);
}

void Node::createChildren(XMLNode& node){
	std::function<std::shared_ptr<Node>()> constructor;
	for(XMLNode& xmlChild: node.children){
		if((constructor = getConstructor(xmlChild.name)) != 0){
			std::shared_ptr<Node> child = constructor();
			add(child);
			child->createChildren(xmlChild);
			child->init(xmlChild);
		}
	}
}

}
