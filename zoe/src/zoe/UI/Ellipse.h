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
<<<<<<< HEAD:zoe/src/zoe/UI/Ellipse.h
=======

>>>>>>> 997f759dd6622a8743e6cb74fea594ad159692d4:zoe/src/zoe/UI/Ellipse.h
	Ellipse(const Rectangle& rect, const Color& color);
	~Ellipse();

	void draw() override;

private:
	Rectangle rect;
	Color color;
};

}
