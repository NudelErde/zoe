//
// Created by Florian on 15.07.2020.
//

#pragma once

#include "../core/Core.h"
#include "../core/Layer.h"
#include "../core/File.h"

#include "Component.h"

namespace Zoe{

/**
 * Should only be constructed by a shared_ptr because BaseComponent inherits from std::enable_shared_from_this.\n
 * Implementation of Layer for the component system.
 */
class DLL_PUBLIC ComponentLayer: public Layer, public BaseComponent {
public:
    /**
     * Constructs empty ComponentLayer with a width of 1600 and a height of 900
     */
    ComponentLayer();
    /**
     * Constructs empty ComponentLayer with specified width and height
     * @param width width of the ComponentLayer
     * @param height height of the ComponentLayer
     */
    ComponentLayer(const unsigned int& width, const unsigned int& height);

    /**
     * Destructs ComponentLayer
     */
    ~ComponentLayer() override;

    /**
     * Load components from .xml file
     * @param file the .xml File
     */
    void load(const File& file);
    /**
     * Load components from parsed .xml file
     * @param node the XMLNode
     */
    void load(const XMLNode& node);

    /**
     * Returns the camera used for default Rendering
     * @returns the shared_ptr to the camera
     */
    inline const std::shared_ptr<Camera>& getCamera() {return camera;}

    /**
     * Sets the camera used for default Rendering
     * @param cam the shared_ptr to the camera
     */
    inline void setCamera(const std::shared_ptr<Camera>& cam) {camera = cam;}

    /**
     * Check if a specific keycode is pressed.
     * @param keycode the keycode to be checked
     * @returns `true` if the key represented by the keycode is pressed.
     * @see Input::isKeyPressed
     */
    bool isKeyPressed(int keycode);
    /**
     * Check if a specific mouse button is pressed.
     * @param button the mouse button to be checked
     * @returns `true` if the mouse button is pressed.
     * @see Input::isMouseButtonPressed
     */
    bool isMouseButtonPressed(int button);

    /**
     * Return the focused component in the active layer.
     * @return the focused component
     */
    std::weak_ptr<BaseComponent> getFocusedObject();

    /**
     * Sets the focused component in the active layer.
     * @param component the new focused component
     */
    void setFocusedObject(const std::weak_ptr<BaseComponent>& component);

    /**
     * Returns the position of the mouse in component coordinate space.\n
     * The return value of Input::getMousePosition is changed in such a way that it matches the camera transformation.
     * @returns the mouse position in component coordinate space.
     * @see Input::getMousePosition
     */
     vec2 getMousePosition();

     void add(const std::shared_ptr<BaseComponent> &component) override;

protected:
    /**
     * Fills this object with attributes from the XMLNode
     * @param node the node from which the attributes are extracted
     */
    void fill(const XMLNode &node) override;
    /**
     * Fills this object after initialization of all children to finish initialization
     */
    void postFill() override;
    /**
     * Does nothing
     * @param time time in seconds since last onUpdate call
     */
    void onUpdate(double time) override;
    /**
     * Does nothing
     * @param camera the camera used to request the draw
     */
    void onDraw(const Camera &camera) override;
    /**
     * Does nothing
     * @param event the input event that was raised
     */
    void onInputEvent(Event &event) override;

    /**
     * Handles events. Calls onDrawEvent and onTickEvent
     * @param event event to be handled
     */
    void onEvent(Event& event) final;

private:
    /**
     * Handles draw events
     */
    void onDrawEvent(AppRenderEvent& );
    /**
     * Handles tick events
     */
    void onTickEvent(AppTickEvent& );

private:
    std::chrono::time_point<std::chrono::steady_clock> timeOfLastTick;

    std::shared_ptr<Camera> camera;

    std::weak_ptr<BaseComponent> focusedObject;

    std::shared_ptr<RenderTarget> target;
    std::shared_ptr<Render> render;
    std::shared_ptr<Render> displayRender;

    unsigned int width, height;
};

}