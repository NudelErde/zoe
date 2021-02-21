//
// Created by Florian on 21.02.2021.
//

#pragma once

#include "../Component.h"

namespace Zoe {

class Rectangle : public BaseComponent {
public:
    Rectangle();

    [[nodiscard]] inline const vec4& getColor() const { return color; }
    inline void setColor(const vec4& color) { Rectangle::color = color; }

    [[nodiscard]] inline const vec2& getSize() const { return size; }
    inline void setSize(const vec2& size) { Rectangle::size = size; }
protected:
    /**
     * Draws this component on the specified camera.
     * @param camera the specified camera
     */
    void onDraw(const Camera& camera) override;
    /**
     * Updates this component.
     * @param time the time in seconds since the last update
     */
    void onUpdate(double time) override;
    /**
     * Handles input events.
     * @param event the input event
     */
    void onInputEvent(Event& event) override;
    /**
     * Fills this component with the information in the specified xml node.
     * @param node the xml node
     */
    void fill(const XMLNode& node) override;
    /**
     * Completes initialization. This method is called after all children are initialized. It is used to extract information for children.
     */
    void postFill() override;
private:
    vec4 color;
    vec2 size;
};

}
