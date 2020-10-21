//
// Created by Florian on 10.07.2020.
//

#pragma once

#include "../Component.h"

namespace Zoe{

/**
 * A Sprite or ComponentGroup combines multiple other components in one element.
 */
class Sprite: public BaseComponent{
public:

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
    void onInputEvent(Event &event) override;
    /**
     * Fills this component with the information in the specified xml node.
     * @param node the xml node
     */
    void fill(const XMLNode &node) override;
    /**
     * Completes initialization. This method is called after all children are initialized. It is used to extract information for children.
     */
    void postFill() override;
private:

};

typedef Sprite ComponentGroup;

}