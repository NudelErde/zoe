/*
 * ScriptNode.h
 *
 *  Created on: 24.12.2019
 *      Author: florian
 */

#pragma once

#include "Node.h"

namespace chaiscript{
class ChaiScript;
}

namespace Zoe {

class ScriptNode: public Node {
public:
	ScriptNode();
	~ScriptNode();

	void tick(double delta) override;
	void draw(mat4x4 mat) override;

	void init(XMLNode& node) override;
private:
	std::shared_ptr<chaiscript::ChaiScript> script;
	std::function<void(double)> tickFunction;
	std::function<void()> initFunction;
	unsigned int tickCount;
};

}

