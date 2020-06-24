/*
 * Image.h
 *
 *  Created on: 17.12.2019
 *      Author: florian
 */

#pragma once

#include "../Core.h"
#include "CommonUI.h"
#include "../File.h"
#include "../render/api/Texture.h"

namespace Zoe {

class DLL_PUBLIC Image: public Component{
public:
	Image(const Rectangle& rect, const File& file);
	Image(const Rectangle& rec, std::shared_ptr<Texture> texture);
	~Image();

	void draw(std::shared_ptr<Render> renderer) override;

private:
	Rectangle rect;
	std::shared_ptr<Texture> texture;
};

}
