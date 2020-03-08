//
// Created by florian on 12.02.20.
//

#pragma once

#include "../Core.h"
#include "CommonUI.h"

namespace Zoe {

    class DLL_PUBLIC Rectangle : public Component {
    public:
        Rectangle(const Rect &rect, const Color &color);

        ~Rectangle();

        void draw() override;

    private:
        Rect rect;
        Color color;
    };

}
