/*
 * 2DGameNode.cpp
 *
 *  Created on: 25.12.2019
 *      Author: florian
 */

#include "2DGameNode.h"
#include "SpriteNode.h"
#include "TextureNode.h"
#include "ScriptNode.h"
#include "../../Console.h"
#include "../../math/mat.h"
#include "../../render/api/Render.h"
#include "../../render/GraphicsContext.h"
#include "../../Application.h"

namespace Zoe{
//-----------------------------------------------------
//Node:
//-----------------------------------------------------

MainNode2D::MainNode2D():width(1), height(1), depth(1){
}

MainNode2D::~MainNode2D(){
}

void MainNode2D::tick(double delta) {
	//iterate with index, because vector can change!
	for (unsigned int i = 0; i<nodes.size(); ++i) {
		nodes[i]->tick(delta);
	}
}

void MainNode2D::draw(mat4x4 mat) {
	mat = mat * scale3D(2/width, -2/height, -1/depth) * translate3D(-width/2, -height/2, 0);
	for (std::shared_ptr<Node>& child : nodes) {
		child->draw(mat);
	}
}

void MainNode2D::init(XMLNode& node) {
	std::string value;
	if (!(value = node.attributes["width"]).empty()) {
		width = fromString<float>(value);
	}
	if (!(value = node.attributes["height"]).empty()) {
		height = fromString<float>(value);
	}
	if (!(value = node.attributes["depth"]).empty()) {
		depth = fromString<float>(value);
	}
}

void MainNode2D::startInit(std::shared_ptr<MainNode2D> node,XMLNode& xmlNode){
	self = node;
	createChildren(xmlNode);
	init(xmlNode);
}

//-----------------------------------------------------
//Layer:
//-----------------------------------------------------

void Layer2D::init(){
	registerNode<SpriteNode>("Sprite");
	registerNode<TextureNode>("Texture");
	registerNode<ScriptNode>("Script");
}

Layer2D::Layer2D(XMLNode& node): Layer("Game 2D"),lastTick(std::chrono::steady_clock::now()){
	setSubscribedEvents(EventCategory::EventCategoryInput|EventCategory::EventCategoryApplication);
	if(node.name=="MainNode2D"){
		topNode = std::make_shared<MainNode2D>();
		topNode->startInit(topNode, node);
	}else{
		error("Can not create Layer2D! Root element must have \"MainNode2D\" as tag");
	}
}

Layer2D::Layer2D(const File& file): Layer("Game 2D"),lastTick(std::chrono::steady_clock::now()){
	setSubscribedEvents(EventCategory::EventCategoryInput|EventCategory::EventCategoryApplication);
	XMLNode node = readXML(file);
	if(node.name=="MainNode2D"){
		topNode = std::make_shared<MainNode2D>();
		topNode->startInit(topNode, node);
	}else{
		error("Can not create Layer2D! Root element must have \"MainNode2D\" as tag");
	}
}

Layer2D::~Layer2D(){
}

void Layer2D::onEvent(Event& event){
	static std::function<bool(AppRenderEvent&)> onRenderEvent = std::bind(&Layer2D::onRender, this, std::placeholders::_1);
	static std::function<bool(AppTickEvent&)> onTickEvent = std::bind(&Layer2D::onTick, this, std::placeholders::_1);

	EventDispatcher dispatcher(event);

	dispatcher.dispatch(onRenderEvent);
	dispatcher.dispatch(onTickEvent);
}

bool Layer2D::onRender(AppRenderEvent& renderEvent){
	static std::shared_ptr<Render> renderer = Application::getContext().getRender();
	renderer->setAlphaEnabled(true);
	renderer->setClearColor(1, 1, 1, 1);
	renderer->clear();
	topNode->draw(scale3D(1, 1, 1));

	return false;
}

bool Layer2D::onTick(AppTickEvent& tickEvent){
	std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
	double delta = (std::chrono::duration_cast<std::chrono::nanoseconds>(now-lastTick).count())/1000000000.0f;
	lastTick = now;
	topNode->tick(delta);
	return false;
}

}
