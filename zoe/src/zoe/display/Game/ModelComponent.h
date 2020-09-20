//
// Created by Florian on 20.08.2020.
//

#pragma once

#include "../../core/Core.h"
#include "../Component.h"
#include "../../render/Model.h"

namespace Zoe {

/**
 * This class is used to wrap a Model in the XML component structure. It is not used to draw a Model.
 * A ModelComponent should be a direct child of a WorldObject. The ModelComponent indicates which Model the WorldObject should draw.
 */
class DLL_PUBLIC ModelComponent: public BaseComponent{
public:
    /**
     * Constructs an empty ModelComponent.
     */
    ModelComponent();

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

public:
    /**
     * Returns the model.
     * @returns model
     */
    inline const Model &getModel() const {
        return model;
    }

    /**
     * Sets the new model.
     * @param model the new model
     */
    inline void setModel(const Model &model) {
        ModelComponent::model = model;
    }

private:
    Model model;
};

}
