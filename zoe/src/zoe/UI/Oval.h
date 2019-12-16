/*
 * Oval.h
 *
 *  Created on: 12.11.2019
 *      Author: florian
 */

#pragma once

#include "../Core.h"
#include "CommonUI.h"

namespace Zoe {

class DLL_PUBLIC Oval: public Component{
public:

	Oval(const Rectangle& rect, const Color& color);
	~Oval();

	void draw() override;

private:
	Rectangle rect;
	Color color;
};

}
