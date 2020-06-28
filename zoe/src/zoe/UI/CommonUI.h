/*
 * CommonUI.h
 *
 *  Created on: 12.11.2019
 *      Author: florian
 */

#pragma once

#include <functional>
#include "../math/vec.h"
#include "../render/api/Render.h"

namespace Zoe{

typedef vec2 Point;

struct Dimension{
	float width, height;
};

struct Rectangle{
	union{
		struct{
			float x,y;
		};
		Point pos;
	};
	union{
		struct{
			float width, height;

		};
		Dimension dimension;
	};
	float rotation;
};

struct ClickInfo{
	int button;
	union{
		struct{
			float x,y;
		};
		Point point;
	};
};

struct Color{
	union{
		struct{
			float r,g,b,a;
		};
        vec4 vector;
    };
};

typedef std::function<void(const ClickInfo&)> ClickCallback;

class DLL_PUBLIC Component{
public:

	virtual ~Component();

	virtual void draw(std::shared_ptr<Render> renderer)=0;

protected:
	Component();
};

}


