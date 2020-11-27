//
// Created by Florian on 11.07.2020.
//

#pragma once

#include "../../core/Core.h"
#include "../Component.h"
#include "../../render/Model.h"

namespace Zoe{

/**
 * A WorldObject contains a model and a light source
 */
class DLL_PUBLIC WorldObject: public BaseComponent{
public:
    /**
     * Constructs an empty WorldObject.
     */
    WorldObject();

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
    inline const Model &getModel() const {return model;}
    /**
     * Returns a reference to the model.
     * @returns a reference to the model
     */
    inline Model &getModel() {return model;}
    /**
     * Sets the new model.
     * @param model the new model
     */
    inline void setModel(const Model &model) {WorldObject::model = model;}

private:
    ///@todo create LightSource class
    //LightSource lightSource
    //bool hasLightSource
    Model model;
    bool init;

};

}