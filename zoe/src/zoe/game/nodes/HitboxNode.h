/*
 * HitboxNode.h
 *
 *  Created on: 03.01.2020
 *      Author: florian
 */

#pragma once

#include "../../Core.h"
#include "Node.h"
#include "../../core/XMLParser.h"

namespace Zoe{

class HitboxNode: public Node{
public:
	HitboxNode();
	~HitboxNode();

	void tick(double delta) override;
	void draw(mat4x4 mat) override;

	void init(XMLNode& node) override;
	void setByKey(std::string key, std::string value) override;
	void setByKey(std::string key, double value) override;
	std::string getStringByKey(std::string key) override;
	double getDoubleByKey(std::string key) override;
};

}


