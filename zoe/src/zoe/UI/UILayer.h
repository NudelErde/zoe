/*
 * UILayer.h
 *
 *  Created on: 12.11.2019
 *      Author: florian
 */

#pragma once

#include "Ellipse.h"
#include "Image.h"
#include "Rectangle.h"
#include "CommonUI.h"

#include "../Layer.h"
#include "../Core.h"
#include "../File.h"

namespace Zoe {

	class DLL_PUBLIC UILayer : public Layer {
	public:
		UILayer();

		virtual ~UILayer();

		virtual void onAttach() = 0;

		virtual void onDetach() = 0;

		void onEvent(Event &event) override;

	private:

		bool onRenderEvent(AppRenderEvent &event);

		bool onKeyPressedEvent(KeyPressedEvent &event);

		bool onKeyReleasedEvent(KeyReleasedEvent &event);

		bool onMouseButtonPressedEvent(MouseButtonPressedEvent &event);

		bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent &event);

		bool onMouseMoveEvent(MouseMovedEvent &event);

	public:

		inline void addComponent(Component *comp) { components.push_back(comp); }

		void addEllipse(const float &x, const float &y, const float &w, const float &h,
						const Color &color);

		void addEllipse(const Rect &rect, const Color &color);

		void addRectangle(const float &x, const float &y, const float &w,
						  const float &h, const Color &color);

		void addRectangle(const Rect &rect, const Color &color);

		void addText(const float &x, const float &y, const char *string,
					 const Color &color);

		void addText(const Point &point, const char *string, const Color &color);

		void addText(const float &x, const float &y, const std::string &string,
					 const Color &color);

		void addText(const Point &point, const std::string &string,
					 const Color &color);

		void setColor(const Color &color);

		void addEllipse(const float &x, const float &y, const float &w,
						const float &h);

		void addEllipse(const Rect &rect);

		void addRectangle(const float &x, const float &y, const float &w,
						  const float &h);

		void addRectangle(const Rect &rect);

		void addText(const float &x, const float &y, const char *string);

		void addText(const Point &point, const char *string);

		void addText(const float &x, const float &y, const std::string &string);

		void addText(const Point &point, const std::string &string);

		void addClickCallback(const float &x, const float &y, const float &w,
							  const float &h, const ClickCallback &callback);

		void addClickCallback(const Rect &rect, const ClickCallback &callback);

		void addImage(const Rect &rect, const File &file);

		void addImage(const float &x, const float &y, const float &w, const float &h, const File &file);

	private:
		std::vector<Component *> components;
		Color color;
	};

}
