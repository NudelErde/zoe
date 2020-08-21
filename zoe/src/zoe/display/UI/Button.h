//
// Created by Florian on 23.07.2020.
//

#pragma once

#include "../../core/Core.h"
#include "../Component.h"

namespace Zoe{

class DLL_PUBLIC Button: public BaseComponent {
public:

    Button();

    void setClickHandler(const std::function<void()>& handler);

    inline const vec2& getSize() const {return size;}
    inline void setSize(const vec2& size) {this->size = size;}

    const vec4 &getColor() const;
    void setColor(const vec4 &color);

protected:
    void onDraw(const Camera& camera) override;
    void onUpdate(double time) override;
    void onInputEvent(Event &event) override;
    void fill(const XMLNode &node) override;
    void postFill() override;

private:
    void onMouseClick(MouseButtonReleasedEvent& mbre);

private:
    std::function<void()> onClick;
    vec2 size{};
    vec4 color{};
};

}