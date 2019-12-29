/*
 * TextureNode.h
 *
 *  Created on: 24.12.2019
 *      Author: florian
 */

#pragma once


#include "Node.h"
#include "../../render/api/Texture.h"

namespace Zoe{

class DLL_PUBLIC TextureNode: public Node{
public:
	TextureNode();
	~TextureNode();

	void tick(double delta) override;
	void draw(mat4x4 mat) override;

	void init(XMLNode& node) override;
	void setByKey(std::string key, std::string value) override;
	void setByKey(std::string key, double value) override;
	std::string getStringByKey(std::string key) override;
	double getDoubleByKey(std::string key) override;
private:
	vec3 pos;
	float width, height;
	std::shared_ptr<Texture> texture;
};

}
