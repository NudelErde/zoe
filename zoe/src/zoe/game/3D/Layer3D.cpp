//
// Created by florian on 13.03.20.
//

#include "Layer3D.h"
#include "../../Application.h"
#include "../../Input.h"
#include "../../KeyCode.h"

namespace Zoe {

    Layer3D::Layer3D() : Layer("3DLayer") {
        setSubscribedEvents(EventCategoryApplication);
        render = Application::getContext().getRender();
    }

    Layer3D::~Layer3D() = default;

    void Layer3D::onEvent(Event &event) {
        std::function<bool(AppRenderEvent &)> onRenderEvent = std::bind(&Layer3D::renderEvent, this,
                                                                        std::placeholders::_1);
        std::function<bool(AppTickEvent &)> onTickEvent = std::bind(&Layer3D::tickEvent, this, std::placeholders::_1);
        EventDispatcher dispatcher(event);
        dispatcher.dispatch(onRenderEvent);
        dispatcher.dispatch(onTickEvent);
    }

    bool Layer3D::renderEvent(AppRenderEvent &event) {
        render->clear();
        onRender();
        for (const auto &object : objects) {
            object->draw(*camera);
        }
        return false;
    }

    bool Layer3D::tickEvent(AppTickEvent &event) {
        onTick();
        for (const auto &object : objects) {
            object->tick();
        }
        return false;
    }

    void Layer3D::add(const std::shared_ptr<Object3D> &object) {
        objects.push_back(object);
    }

    void Layer3D::onAttach() {}

    void Layer3D::onDetach() {}

    void Layer3D::onRender() {}

    void Layer3D::onTick() {}

}