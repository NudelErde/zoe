//
// Created by Florian on 21.06.2020.
//

#pragma once

#include "../../math/vec.h"

namespace Zoe{
    /**
     * A bitfield used for small settings to the Render object.
     */
    struct RenderFlag{
        /**
         * `true` if alpha should be enabled.
         */
        bool alpha: 1;
    };
    /**
     * RenderSettings is used to save the settings used in the Render object.
     */
    struct RenderSettings{
        /**
         * flag is used for small settings.
         */
        RenderFlag flag;
        /**
         * The clear color.
         */
        vec4 clearColor;
        /**
         * Dimension and position of the viewport.
         * X-value.
         */
        unsigned int x;
        /**
         * Dimension and position of the viewport.
         * Y-value.
         */
        unsigned int y;
        /**
         * Dimension and position of the viewport.
         * width-value.
         */
        unsigned int width;
        /**
         * Dimension and position of the viewport.
         * height-value.
         */
        unsigned int height;
    };

}
