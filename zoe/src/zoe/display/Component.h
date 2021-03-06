//
// Created by Florian on 10.07.2020.
//

#pragma once

#include "../core/Core.h"
#include "../render/api/Render.h"
#include "../core/XMLParser.h"
#include "../event/CommonEvent.h"

namespace Zoe {

class ComponentLayer;

class Camera;

/**
 * The component system is used to display game and UI. It is based on nodes. Every component can have children and
 * every component can have a parent. The top most node is a ComponentLayer.
 * A component can be drawn, updated and handle input events. Not every component implements all of these functions.
 *
 * The ComponentLayer can be filled using an XMLFile. This is the preferred way to construct the components.
 * All Components should be constructable using an XMLFile and directly through code.
 *
 * Register components through the BaseComponent::registerComponent methods. If the templated version of this method is used,
 * the template argument must inherit form BaseComponent. If the method with two arguments is used, the std::function must
 * return a std::shared_ptr to an Object that inherits from BaseComponent. Only registered components can be used when
 * filling a ComponentLayer with an XMLFile. Unregistered components can only be used if the component is added through code.
 *
 * Every node should be allocated as a shared pointer.
 */
class DLL_PUBLIC BaseComponent : public std::enable_shared_from_this<BaseComponent> {
public:

    /**
     * Draws this component and all child components on the specified camera.
     * @param camera the specified camera
     */
    void draw(const Camera& camera);

    /**
     * Updates this component and all child components with the specified time in seconds since the last update.
     * @param time the time since the last update
     */
    void update(double time);

    /**
     * Updates this component and all child components with the specified time in seconds since the last physics update.
     * The physics update should be used to update the components position and is part of the physics system.
     * @param time the time since the last update
     */
    void physicsUpdate(double time);

    /**
     * Send the specified input event to this component and all child components.
     * @param event the specified input event
     */
    void inputEvent(Event& event);

    /**
     * Add a component as a child to this component. If the component already has a parent, the component is not added
     * and a warning is logged.
     * @param component the new child component.
     */
    virtual void add(const std::shared_ptr<BaseComponent>& component);
    //virtual void load() = 0;
    //virtual void unload() = 0;

    /**
     * Returns the absolut position of this component.
     * @returns the absolut position
     */
    vec3 getWorldPosition() const;

    /**
     * Returns the position of this component relative to the parent component.
     * @return
     */
    inline const vec3& getPosition() const { return position; }

    /**
     * Specifies the position of this component relative to the parent component.
     * @param pos
     */
    inline void setPosition(const vec3& pos) { position = pos; }

    /**
     * Returns a weak pointer to the parent component. It is not guaranteed to point to an existing object.
     * @return
     */
    inline const std::weak_ptr<BaseComponent>& getParent() { return parent; }

    /**
     * Returns a vector of pointers to the children of this component.
     * @return
     */
    inline const std::vector<std::shared_ptr<BaseComponent>>& getChildren() { return children; }

    /**
     * Returns `true` if the component is focused.
     * @return `true` if the component is focused
     */
    [[nodiscard]] bool hasFocus() const;

    /**
     * Specifies the focus. All other components in this layer lose focus when this object receives focus.
     * @param val the new focus value
     */
    void setFocus(bool val);

    /**
     * Returns `true` if this component is visible.
     * @return `true` if this component is visible
     */
    [[nodiscard]] inline bool isVisible() const {
        return visible;
    }

    /**
     * Specifies the visibility of this component.
     * @param v `true` if this component will be visible
     */
    inline void setVisible(const bool& v) {
        visible = v;
    }

    /**
     * Returns a pointer to the layer in which this component is located.
     * @return a pointer to the layer
     */
    inline std::shared_ptr<ComponentLayer> getLayer() { return layer.lock(); }

    /**
     * Returns a pointer to the child with the specified id.
     * @param componentID the specified id
     * @return the child or null if not found
     */
    std::shared_ptr<BaseComponent> getChildByID(const std::string& componentID);

    /**
     * Returns a pointer to the child with the specified id and type.
     * @tparam T the specified type
     * @param componentID the specified id
     * @return the child or null if not found
     */
    template<typename T>
    std::shared_ptr<T> getChildByIDAndType(const std::string& componentID) {
        std::vector<std::shared_ptr<BaseComponent>> childVector;
        std::vector<std::shared_ptr<BaseComponent>> grandChildVector = children;
        bool repeat = true;
        while (repeat) {
            childVector = grandChildVector;
            grandChildVector.clear();
            repeat = false;
            for (const auto& child: childVector) {
                if (child->id == componentID) {
                    if (auto ptr = std::dynamic_pointer_cast<T>(child)) {
                        return ptr;
                    }
                }
                const auto& grandchildren = child->getChildren();
                if (!grandchildren.empty()) {
                    repeat = true;
                    grandChildVector.insert(grandChildVector.end(), grandchildren.begin(), grandchildren.end());
                }
            }
        }
        return std::shared_ptr<T>();
    }

protected:

    /**
     * Fills this component with the information in the specified xml node.
     * @param node the xml node
     */
    virtual void fill(const XMLNode& node) = 0;

    /**
     * Completes initialization. This method is called after all children are initialized. It is used to extract information for children.
     */
    virtual void postFill() = 0;

    /**
     * Draws this component on the specified camera.
     * @param camera the specified camera
     */
    virtual void onDraw(const Camera& camera) = 0;

    /**
     * Updates this component. This function is called in a specific interval.
     * @param delta the time in seconds since the last update
     */
    virtual void onUpdate(double delta) = 0;

    /**
     * Updates this component. This function is called as fast as possible. It shouldn't take long to execute this function.
     * @param delta the time in seconds since the last update
     */
    virtual void onPhysicsUpdate(double delta);

    /**
     * Handles input events.
     * @param event the input event
     */
    virtual void onInputEvent(Event& event) = 0;

protected:
    /**
     * The position in 3D space of the component.
     */
    vec3 position{};
private:
    std::vector<std::shared_ptr<BaseComponent>> children;
    std::weak_ptr<BaseComponent> parent;
    std::weak_ptr<ComponentLayer> layer;

    std::string id;
    bool isFocused;
    bool visible = true;

    void init(const XMLNode& node);

    friend class ComponentLayer;

public:
    /**
     * Registers a class that inherits from BaseComponent. This class must have a default constructor.
     * @tparam T type of the specified Component
     * @param name the name used in XMLFiles
     * @see BaseComponent::createComponent
     */
    template<typename T>
    inline static void registerComponent(const std::string& name) {
        registerComponent(name, []() { return std::make_shared<T>(); });
    }

    /**
     * Registers a function to create an object that inherits from BaseComponent.
     * @param name the name used in XMLFiles
     * @param func the function used to create the new object
     * @see BaseComponent::createComponent
     */
    static void registerComponent(const std::string& name, const std::function<std::shared_ptr<BaseComponent>()>& func);

    /**
     * Creates a component using the given name.
     * @param name the name of the class
     * @returns a shared_ptr to the created object
     * @see BaseComponent::registerComponent
     */
    static std::shared_ptr<BaseComponent> createComponent(const std::string& name);

    /**
     * Checks if a specific class name was registered using the BaseComponent::registerComponent functions.
     * @param name the specific name
     * @returns `true` if the class name is registered
     */
    static bool hasComponentConstructor(const std::string& name);

    /**
     * Initializes the component system and adds the default components. Is called by Application::Application
     */
    static void init();
};
}