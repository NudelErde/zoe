/*
 * CommonUI.h
 *
 *  Created on: 12.11.2019
 *      Author: florian
 */

#pragma once

#include <functional>
#include "../math/vec.h"

namespace Zoe {

    typedef vec2 Point;

    struct Dimension {
        float width, height;
    };

    struct Rect {
        union {
            struct {
                float x, y;
            };
            Point pos;
        };
        union {
            struct {
                float width, height;

            };
            Dimension dimension;
        };
        float rotation;
    };

    struct ClickInfo {
        int button;
        union {
            struct {
                float x, y;
            };
            Point point;
        };
    };

    struct Color {
        union {
            vec4 vector;
            struct {
                float r, g, b, a;
            };
        };
    };

    typedef std::function<void(const ClickInfo &)> ClickCallback;

    class DLL_PUBLIC Component {
    public:

        virtual ~Component();

        virtual void draw() = 0;

    protected:
        Component();
    };

}


