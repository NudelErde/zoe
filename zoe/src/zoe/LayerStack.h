/*
 * LayerStack.h
 *
 *  Created on: 11.04.2019
 *      Author: florian
 */

#pragma once

#include "Core.h"
#include "Layer.h"
#include "event/CommonEvent.h"

namespace Zoe {

class LayerStack {
public:
	LayerStack();
	virtual ~LayerStack();

	void pushLayer(Layer* layer);
	void pushOverlay(Layer* layer);
	void popLayer(Layer* layer);
	void popOverlay(Layer* layer);

	inline std::vector<Layer*>::iterator begin() {
		return layers.begin();
	}
	inline std::vector<Layer*>::iterator end() {
		return layers.end();
	}

	template<typename T>
	void dispatchEvent(T& event) {
		if (event.isHandled())
			return;
		if (event.isInCategory(EventCategoryApplication)) {
			for (auto it = end()-1; it >= begin(); it--) {
				Layer& layer = *(Layer*) (*it);
				if (layer.subscribedEvents & event.getCategoryFlags()) {
					layer.onEvent(event);
					if (event.isHandled())
						return;
				}
			}
		} else {
			for (auto it = begin(); it != end(); it++) {
				Layer& layer = *(Layer*) (*it);
				if (layer.subscribedEvents & event.getCategoryFlags()) {
					layer.onEvent(event);
					if (event.isHandled())
						return;
				}
			}
		}
	}

private:
	std::vector<Layer*> layers;
	std::vector<Layer*>::iterator layerInsert;
};

} /* namespace Zoe */
