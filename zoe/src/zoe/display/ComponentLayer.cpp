//
// Created by Florian on 15.07.2020.
//

#include "ComponentLayer.h"
#include "../core/Application.h"
#include "../render/api/RenderTarget.h"
#include "../core/Input.h"
#include "Camera.h"

namespace Zoe {

static struct {
    std::shared_ptr<Shader> imageCopy;
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;
    bool init = false;
} data;

ComponentLayer::ComponentLayer() : ComponentLayer(1600, 900) {}

ComponentLayer::ComponentLayer(const unsigned int& width, const unsigned int& height) : Layer("ComponentLayer"),
                                                                                        width(width),
                                                                                        height(height) {
    setSubscribedEvents((unsigned int) EventCategory::Application | (unsigned int) EventCategory::Input);
    timeOfLastTick = std::chrono::steady_clock::now();

    if (!data.init) {
        data.init = true;
        float vertexData[]{
                -1, -1, 0, 0,
                1, -1, 1, 0,
                1, 1, 1, 1,
                -1, 1, 0, 1
        };
        data.vertexBuffer = Application::getContext().getVertexBuffer(false);
        data.vertexBuffer->setData(vertexData, sizeof(float) * 16);

        unsigned int indexData[]{
                0, 1, 2, 2, 3, 0
        };
        data.indexBuffer = Application::getContext().getIndexBuffer(false);
        data.indexBuffer->setData(indexData, 6);

        std::shared_ptr<VertexBufferLayout> layout = Application::getContext().getVertexBufferLayout();
        layout->push_float(2);
        layout->push_float(2);

        data.vertexArray = Application::getContext().getVertexArray();
        data.vertexArray->set(data.vertexBuffer, data.indexBuffer, layout);

        data.imageCopy = Application::getContext().getShader(File("virtual/zoe/display/TextureCopyShader.glsl"));
    }
    target = Application::getContext().getRenderTarget(width, height);

    render = Application::getContext().getRender();
    render->setAlphaEnabled(true);
    render->setClearColor(0, 0, 0, 0);
    render->setRenderTarget(target);
    render->setViewport(0, 0, width, height);

    displayRender = Application::getContext().getRender();
    displayRender->setRenderTarget(Application::getContext().getDefaultRenderTarget());
    displayRender->setAlphaEnabled(true);
}

ComponentLayer::~ComponentLayer() = default;

void ComponentLayer::onEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<AppRenderEvent>([this](AppRenderEvent& eve) { this->onDrawEvent(eve); });
    dispatcher.dispatch<AppTickEvent>([this](AppTickEvent& eve) { this->onTickEvent(eve); });
    dispatcher.dispatch<AppUpdateEvent>([this](AppUpdateEvent& eve) { this->onUpdateEvent(eve); });
    if (event.isInCategory(EventCategory::Input)) {
        inputEvent(event);
    }
}

void ComponentLayer::fill(const XMLNode& node) {

    if (node.attributes.count("defaultCamera") > 0) {
        defaultCameraID = node.attributes.at("defaultCamera");
    }
}

void ComponentLayer::postFill() {
    if (!defaultCameraID.empty()) {
        auto cam = getChildByIDAndType<Camera>(defaultCameraID);
        if (cam) {
            setCamera(cam);
        }
    }
}

void ComponentLayer::onUpdate(double time) {

}

void ComponentLayer::onDraw(const Camera&) {

}

void ComponentLayer::onInputEvent(Event& event) {

}

void ComponentLayer::onDrawEvent(AppRenderEvent& event) {
    render->clear();
    if (camera) {
        camera->setRender(render);
        draw(*camera);
    }
    data.imageCopy->setTexture("u_texture", *target->getColorAttachment());
    displayRender->draw(*data.vertexArray, *data.imageCopy);
}

void ComponentLayer::onUpdateEvent(AppUpdateEvent& event) {
    double duration = (std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - timeOfLastPhysicsTick).count() / 1000.0f);
    physicsUpdate(duration);
    timeOfLastPhysicsTick = std::chrono::steady_clock::now();
}

void ComponentLayer::onTickEvent(AppTickEvent& event) {
    double duration = (std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - timeOfLastTick).count() / 1000.0f);
    update(duration);
    timeOfLastTick = std::chrono::steady_clock::now();
}

bool ComponentLayer::isKeyPressed(int keycode) {
    return Input::isKeyPressed(keycode);
}

bool ComponentLayer::isMouseButtonPressed(int button) {
    return Input::isMouseButtonPressed(button);
}

vec2 ComponentLayer::getMousePosition() {
    vec2 pos = Input::getMousePosition();
    pos.x *= (float) width;
    pos.y *= (float) height;
    mat4x4 invViewMatrix = getCamera()->getInvViewMatrix();
    return (invViewMatrix.inverse() * vec4({pos.x, pos.y, 0, 1})).xy();
}

std::weak_ptr<BaseComponent> ComponentLayer::getFocusedObject() {
    return focusedObject;
}

void ComponentLayer::setFocusedObject(const std::weak_ptr<BaseComponent>& component) {
    focusedObject = component;
}
void ComponentLayer::load(const File& file) {
    load(readXML(file));
}
void ComponentLayer::load(const XMLNode& node) {
    init(node);
}
void ComponentLayer::add(const std::shared_ptr<BaseComponent>& component) {
    //if you don't understand (copy-pasted this stuff lul): https://stackoverflow.com/a/45507610
    if (!layer.owner_before(std::weak_ptr<BaseComponent>{}) && !std::weak_ptr<BaseComponent>{}.owner_before(layer)) {
        layer = std::dynamic_pointer_cast<ComponentLayer>(shared_from_this());
    }
    BaseComponent::add(component);
}

}