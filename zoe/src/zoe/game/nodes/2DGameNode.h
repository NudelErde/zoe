/*
 * 2DGameNode.h
 *
 *  Created on: 24.12.2019
 *      Author: florian
 */

#pragma once

#include "../../Layer.h"
#include "../../core/XMLParser.h"
#include "Node.h"

namespace Zoe{

class MainNode2D: public Node{
public:
	MainNode2D();
	~MainNode2D();

	void tick(double delta) override;
	void draw(mat4x4 mat) override;

	void init(XMLNode& node) override;
private:
	void startInit(std::shared_ptr<MainNode2D> node,XMLNode& xmlNode);
	friend class Layer2D;
	float width, height, depth;
};

class DLL_PUBLIC Layer2D:public Layer{
public:
	Layer2D(XMLNode& topNode);
	Layer2D(const File& file);
	~Layer2D();

	void onEvent(Event& event);
	bool onRender(AppRenderEvent& renderEvent);
	bool onTick(AppTickEvent& tickEvent);

	static void init();
private:
	std::shared_ptr<MainNode2D> topNode;
	std::chrono::time_point<std::chrono::steady_clock> lastTick;
};


}


