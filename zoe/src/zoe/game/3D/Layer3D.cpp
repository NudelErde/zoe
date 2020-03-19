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
        onRender();
        render->clear();
        for (const auto &object : objects) {
            object->draw(*camera);
        }
        return false;
    }

    bool Layer3D::tickEvent(AppTickEvent &event) {
        onTick();
        if (debugCamera) {
            if (Input::isKeyPressed(KEY_SPACE)) {
                camera->setPosition(camera->getPosition() + (vec3) {0, 0.1, 0});
            } else if (Input::isKeyPressed(KEY_LEFT_SHIFT)) {
                camera->setPosition(camera->getPosition() + (vec3) {0, -0.1, 0});
            } else if (Input::isKeyPressed(KEY_W)) {
                camera->setPosition(camera->getPosition() + camera->getDirection() * 0.1);
            } else if (Input::isKeyPressed(KEY_S)) {
                camera->setPosition(camera->getPosition() + camera->getDirection() * -0.1);
            } else if (Input::isKeyPressed(KEY_D)) {
                camera->setPosition(camera->getPosition() + camera->getDirection().crossProduct({0, 0.1, 0}));
            } else if (Input::isKeyPressed(KEY_A)) {
                camera->setPosition(camera->getPosition() + camera->getDirection().crossProduct({0, -0.1, 0}));
            } else if (Input::isKeyPressed(KEY_RIGHT)) {
                camera->setYaw(camera->getRotation().y - 0.01f);
            } else if (Input::isKeyPressed(KEY_LEFT)) {
                camera->setYaw(camera->getRotation().y + 0.01f);
            } else if (Input::isKeyPressed(KEY_UP)) {
                camera->setPitch(camera->getRotation().x + 0.01f);
            } else if (Input::isKeyPressed(KEY_DOWN)) {
                camera->setPitch(camera->getRotation().x - 0.01f);
            } else if (Input::isKeyPressed(KEY_E)) {
                camera->setRoll(camera->getRotation().z - 0.01f);
            } else if (Input::isKeyPressed(KEY_Q)) {
                camera->setRoll(camera->getRotation().z + 0.01f);
            }
        }
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

    void Layer3D::setDebugCamera(bool autoCam) {
        camera = std::make_shared<Camera>();
        camera->setProjectionMatrix(Zoe::perspective(1,100,70,16.0f/9.0f));
        camera->setPosition({0,0, 10});
        camera->setRotation(0,0,0);
        this->debugCamera = autoCam;
    }
}