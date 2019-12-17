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
	~Image();

	void draw() override;

private:
	Rectangle rect;
	Texture* texture;
};

}
