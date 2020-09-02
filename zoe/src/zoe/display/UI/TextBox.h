//
// Created by Florian on 02.09.2020.
//

#pragma once

#include "../../core/Core.h"
#include "../Component.h"
#include "../../render/Font.h"

namespace Zoe {

class DLL_PUBLIC TextBox : public BaseComponent{
public:
    TextBox();

    inline void setText(const std::string& str) {text = str;}
    inline const std::string& getText() {return text;}

protected:
    void onDraw(const Camera& camera) override;
    void onUpdate(double time) override;
    void onInputEvent(Event &event) override;
    void fill(const XMLNode &node) override;
    void postFill() override;

private:
    std::string text;
    Font font;
    vec4 color{};
};

}

