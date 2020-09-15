//
// Created by Florian on 21.06.2020.
//

#pragma once

#include "../../math/vec.h"

namespace Zoe{
    struct RenderFlag{
        bool alpha: 1;
    };
    struct RenderSettings{
        RenderFlag flag;
        vec4 clearColor;
        unsigned int x,y,width,height;
    };

}
