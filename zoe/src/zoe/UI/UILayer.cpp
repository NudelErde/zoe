/*
 * UILayer.cpp
 *
 *  Created on: 12.11.2019
 *      Author: florian
 */

#include "UILayer.h"
#include "../render/api/Render.h"
#include "../render/api/RenderTarget.h"
#include "../Application.h"

namespace Zoe{

static constexpr const char* shaderSource =
R"(
#shader vertex
#version 130

layout(location=0) in vec2 position;
layout(location=1) in vec2 texPosition;

out vec2 v_pos;

void main(){
	gl_Position = vec4(position,0,1);
	v_pos = texPosition;
}

#shader fragment
#version 130

in vec2 v_pos;

out vec4 color;

uniform sampler2D tex;

void main(){
	color = texture2D(tex,v_pos);
}
)";

static struct {
    std::shared_ptr<Shader> shader;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;
    std::shared_ptr<VertexArray> vertexArray;
    bool init = false;
} data;

UILayer::UILayer(): Layer("UILayer") {
    if(!data.init){
        data.init = true;
        registerVirtualFile("zoe/UILayerShaderSource.glsl", shaderSource);
        data.shader = Application::getContext().getShader(File("zoe/UILayerShaderSource.glsl"));
        data.vertexBuffer = Application::getContext().getVertexBuffer(false);
        data.indexBuffer = Application::getContext().getIndexBuffer(false);
        float vertexData[] {
            -1,-1,0,0,
             1,-1,1,0,
             1, 1,1,1,
            -1, 1,0,1
        };
        unsigned int indexData[] {
            0,1,2,2,3,0
        };
        data.vertexBuffer->setData(vertexData, 16*sizeof(float));
        data.indexBuffer->setData(indexData, 6);

        auto layout = Application::getContext().getVertexBufferLayout();
        layout->push_float(2);
        layout->push_float(2);

        data.vertexArray = Application::getContext().getVertexArray();
        data.vertexArray->set(*data.vertexBuffer, *data.indexBuffer, *layout);
    }
	setSubscribedEvents(EventCategoryApplication|EventCategoryInput);
	renderTarget = Application::getContext().getRenderTarget(1600,900);
    render = Application::getContext().getRender();
    render->setRenderTarget(renderTarget);
    render->setClearColor(0,0,0,0);
    render->setViewport(0,0,1600,900);
    displayRender = Application::getContext().getRender();
    displayRender->setAlphaEnabled(true);
}

UILayer::~UILayer() = default;

//------------------
//------EVENTS------
//------------------

void UILayer::onEvent(Event& event) {
	std::function<bool(AppRenderEvent&)> onRenderEvent = std::bind(&UILayer::onRenderEvent, this, std::placeholders::_1);

	std::function<bool(KeyPressedEvent&)> onKeyPressedEvent = std::bind(&UILayer::onKeyPressedEvent, this, std::placeholders::_1);
	std::function<bool(KeyReleasedEvent&)> onKeyReleasedEvent = std::bind(&UILayer::onKeyReleasedEvent, this, std::placeholders::_1);

	std::function<bool(MouseButtonPressedEvent&)> onMouseButtonPressedEvent = std::bind(&UILayer::onMouseButtonPressedEvent, this, std::placeholders::_1);
	std::function<bool(MouseButtonReleasedEvent&)> onMouseButtonReleasedEvent = std::bind(&UILayer::onMouseButtonReleasedEvent, this, std::placeholders::_1);
	std::function<bool(MouseMovedEvent&)> onMouseMoveEvent = std::bind(&UILayer::onMouseMoveEvent, this, std::placeholders::_1);

	EventDispatcher dispatcher(event);
	dispatcher.dispatch(onRenderEvent);

	dispatcher.dispatch(onKeyPressedEvent);
	dispatcher.dispatch(onKeyReleasedEvent);

	dispatcher.dispatch(onMouseButtonPressedEvent);
	dispatcher.dispatch(onMouseButtonReleasedEvent);
	dispatcher.dispatch(onMouseMoveEvent);
}

bool UILayer::onKeyPressedEvent(KeyPressedEvent& event) {
	for(const auto& func: keyPressedListeners){
	    func(event);
	}
	return false;
}

bool UILayer::onKeyReleasedEvent(KeyReleasedEvent& event) {
    for(const auto& func: keyReleasedListeners){
        func(event);
    }
    return false;
}

bool UILayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& event) {
    for(const auto& func: mousePressedListeners){
        func(event);
    }
    return false;
}

bool UILayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& event) {
    for(const auto& func: mouseReleasedListeners){
        func(event);
    }
    return false;
}

bool UILayer::onMouseMoveEvent(MouseMovedEvent& event) {
    for(const auto& func: mouseMoveListeners){
        func(event);
    }
    return false;
}

//----------------------
//------COMPONENTS------
//----------------------

void UILayer::addEllipse(const float& x, const float& y, const float& w,
		const float& h, const Color& color) {
	Rectangle r {x,y,w,h,0};
	addComponent(new Ellipse(r,color));
}

void UILayer::addEllipse(const Rectangle& rect, const Color& color) {
	addComponent(new Ellipse(rect, color));
}

void UILayer::addRectangle(const float& x, const float& y, const float& w,
		const float& h, const Color& color) {
	throw std::runtime_error("not implemented");
}

void UILayer::addRectangle(const Rectangle& rect, const Color& color) {
	throw std::runtime_error("not implemented");
}

void UILayer::addText(const float& x, const float& y,
		const std::string& string, const Color& color, const Font& font) {
	Point p {x,y};
    addComponent(new Text(p, font, string, color));
}

void UILayer::addText(const Point& point, const std::string& string,
		const Color& color, const Font& font) {
    addComponent(new Text(point, font, string, color));
}

//

void UILayer::setColor(const Color& color) {
	this->color = color;
}

//

void UILayer::addEllipse(const float& x, const float& y, const float& w,
		const float& h) {
	Rectangle r = {x,y,w,h,0};
	addComponent(new Ellipse(r, color));
}

void UILayer::addEllipse(const Rectangle& rect) {
	addComponent(new Ellipse(rect, color));
}

void UILayer::addRectangle(const float& x, const float& y, const float& w,
		const float& h) {
	throw std::runtime_error("not implemented");
}

void UILayer::addRectangle(const Rectangle& rect) {
	throw std::runtime_error("not implemented");
}

void UILayer::addText(const float& x, const float& y,
		const std::string& string, const Font& font) {
	Point p {x,y};
	addComponent(new Text(p, font, string, color));
}

void UILayer::addText(const Point& point, const std::string& string, const Font& font) {
	addComponent(new Text(point, font, string, color));
}

//------------------
//------IMAGE-------
//------------------

void UILayer::addImage(const Rectangle& rect, const File& file){
	addComponent(new Image(rect, file));
}

void UILayer::addImage(const float& x, const float& y, const float& w, const float& h, const File& file){
	Rectangle r = {x,y,w,h,0};
	addComponent(new Image(r,file));
}

void UILayer::addImage(const Rectangle &rect, const std::shared_ptr<Texture>&texture) {
    addComponent(new Image(rect, texture));
}

void UILayer::addImage(const float& x, const float& y, const float& w, const float& h, const std::shared_ptr<Texture>& texture){
    Rectangle r = {x,y,w,h,0};
    addComponent(new Image(r,texture));
}


//------------------
//------RENDER------
//------------------

bool UILayer::onRenderEvent(AppRenderEvent& event) {

    render->clear();
	for(auto const& component: components) {
        render->push();
		component->draw(render);
        render->pop();
	}
	auto texture = renderTarget->getColorAttachment();
	data.shader->setTexture("tex", *texture);
	displayRender->draw(*data.vertexArray, *data.shader);
	return false;
}

void UILayer::setBackground(const Color &color) {
    background = color;
    displayRender->setClearColor(color.r,color.g,color.b,color.a);
}

void UILayer::addKeyPressedListener(std::function<void(const KeyPressedEvent &)> func) {
    keyPressedListeners.push_back(std::move(func));
}

void UILayer::addKeyReleasedListener(std::function<void(const KeyReleasedEvent &)> func) {
    keyReleasedListeners.push_back(std::move(func));
}

void UILayer::addMousePressedListener(std::function<void(const MouseButtonPressedEvent &)> func) {
    mousePressedListeners.push_back(std::move(func));
}

void UILayer::addMouseReleasedListener(std::function<void(const MouseButtonReleasedEvent &)> func) {
    mouseReleasedListeners.push_back(std::move(func));
}

void UILayer::addMouseMoveListener(std::function<void(const MouseMovedEvent &)> func) {
    mouseMoveListeners.push_back(std::move(func));
}

}
