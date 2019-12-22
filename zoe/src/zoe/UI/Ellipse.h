/*
 * Ellipse.h
 *
 *  Created on: 12.11.2019
 *      Author: florian
 */

#pragma once

#include "../Core.h"
#include "CommonUI.h"

namespace Zoe {

class DLL_PUBLIC Ellipse: public Component{
public:

	Ellipse(const Rectangle& rect, const Color& color);
	~Ellipse();

	void draw() override;

private:
	Rectangle rect;
	Color color;
};

}
