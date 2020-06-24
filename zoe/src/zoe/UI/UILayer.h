/*
 * UILayer.h
 *
 *  Created on: 12.11.2019
 *      Author: florian
 */

#pragma once

#include "Ellipse.h"
#include "Image.h"
#include "CommonUI.h"
#include "Text.h"

#include "../Layer.h"
#include "../Core.h"
#include "../File.h"

namespace Zoe {

class DLL_PUBLIC UILayer: public Layer {
public:
	UILayer();
	~UILayer() override;

	void onAttach() override = 0;
	void onDetach() override = 0;
	void onEvent(Event& event) final;

private:

	bool onRenderEvent(AppRenderEvent& event);
	bool onKeyPressedEvent(KeyPressedEvent& event);
	bool onKeyReleasedEvent(KeyReleasedEvent& event);
	bool onMouseButtonPressedEvent(MouseButtonPressedEvent& event);
	bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
	bool onMouseMoveEvent(MouseMovedEvent& event);

private:

    std::shared_ptr<RenderTarget> renderTarget;
    std::shared_ptr<Render> render;
    std::shared_ptr<Render> displayRender;
public:

	inline void addComponent(Component* comp){components.push_back(comp);}

	void addEllipse(const float& x, const float& y, const float& w, const float& h,
			const Color& color);
	void addEllipse(const Rectangle& rect, const Color& color);
	void addRectangle(const float& x, const float& y, const float& w,
			const float& h, const Color& color);
	void addRectangle(const Rectangle& rect, const Color& color);
	void addText(const float& x, const float& y, const std::string& string,
			const Color& color, const Font& font);
	void addText(const Point& point, const std::string& string,
			const Color& color, const Font& font);

	void setColor(const Color& color);
	void addEllipse(const float& x, const float& y, const float& w,
			const float& h);
	void addEllipse(const Rectangle& rect);
	void addRectangle(const float& x, const float& y, const float& w,
			const float& h);
	void addRectangle(const Rectangle& rect);
	void addText(const float& x, const float& y, const std::string& string, const Font& font);
	void addText(const Point& point, const std::string& string, const Font& font);

	void addClickCallback(const float& x, const float& y, const float& w,
			const float& h, const ClickCallback& callback);
	void addClickCallback(const Rectangle& rect, const ClickCallback& callback);

    void addImage(const Rectangle& rect, const File& file);
    void addImage(const float& x, const float& y, const float& w, const float& h, const File& file);
    void addImage(const Rectangle& rect, const std::shared_ptr<Texture>& texture);
    void addImage(const float& x, const float& y, const float& w, const float& h, const std::shared_ptr<Texture>& texture);

private:
	std::vector<Component*> components;
	Color color{};
};

}
