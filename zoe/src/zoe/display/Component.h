//
// Created by Florian on 10.07.2020.
//

#pragma once

#include "../core/Core.h"
#include "../render/api/Render.h"
#include "../core/XMLParser.h"
#include "../event/CommonEvent.h"
#include "Camera.h"

namespace Zoe {
class DLL_PUBLIC BaseComponent : public std::enable_shared_from_this<BaseComponent> {
public:
    BaseComponent();

    void draw(const Camera &camera);

    void update(double time);

    void inputEvent(Event &event);

    void add(const std::shared_ptr<BaseComponent> &component);
    //virtual void load() = 0;
    //virtual void unload() = 0;

    vec3 getWorldPosition();

    inline const vec3 &getPosition() const { return position; }

    inline void setPosition(const vec3 &pos) { position = pos; }

    inline const std::weak_ptr<BaseComponent> &getParent() { return parent; }

    inline const std::vector<std::shared_ptr<BaseComponent>> &getChildren() { return children; }

protected:

    virtual void fill(const XMLNode &node) = 0;

    virtual void postFill() = 0;

    virtual void onDraw(const Camera &camera) = 0;

    virtual void onUpdate(double time) = 0;

    virtual void onInputEvent(Event &event) = 0;

protected:
    vec3 position{};
private:
    std::vector<std::shared_ptr<BaseComponent>> children;
    std::weak_ptr<BaseComponent> parent;

    void init(const XMLNode &node);

    friend class ComponentLayer;

public:
    template<typename T>
    inline static void registerComponent(const std::string &name) {
        registerComponent(name, []() { return std::make_shared<T>(); });
    }

    static void registerComponent(const std::string &name, const std::function<std::shared_ptr<BaseComponent>()> &func);

    static std::shared_ptr<BaseComponent> createComponent(const std::string &name);

    static bool hasComponentConstructor(const std::string &name);

    static void init();
};
}