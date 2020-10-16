/*
 * LayerStack.h
 *
 *  Created on: 11.04.2019
 *      Author: florian
 */

#pragma once

#include "Core.h"
#include "Layer.h"
#include "../event/CommonEvent.h"

namespace Zoe {

class DLL_PUBLIC LayerStack {
public:
	LayerStack();
	virtual ~LayerStack();

	void pushLayer(const std::shared_ptr<Layer>& layer);
	void pushOverlay(const std::shared_ptr<Layer>& layer);
	void popLayer(const std::shared_ptr<Layer>& layer);
	void popOverlay(const std::shared_ptr<Layer>& layer);

	inline std::vector<std::shared_ptr<Layer>>::iterator begin() {
		return layers.begin();
	}
	inline std::vector<std::shared_ptr<Layer>>::iterator end() {
		return layers.end();
	}

	inline std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() {
		return layers.rbegin();
	}

	inline std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() {
		return layers.rend();
	}

	template<typename T>
	void dispatchEvent(T& event) {
		if (event.isHandled())
			return;
		if (event.isInCategory(EventCategory::Application)) {
			for (auto it = rbegin(); it != rend(); ++it) {
				Layer& layer = *(std::shared_ptr<Layer>) (*it);
				if (layer.subscribedEvents & event.getCategoryFlags()) {
					layer.onEvent(event);
					if (event.isHandled())
						return;
				}
			}
		} else {
			for (auto & it : *this) {
				Layer& layer = *(std::shared_ptr<Layer>) it;
				if (layer.subscribedEvents & event.getCategoryFlags()) {
					layer.onEvent(event);
					if (event.isHandled())
						return;
				}
			}
		}
	}

private:
	std::vector<std::shared_ptr<Layer>> layers;
	std::vector<std::shared_ptr<Layer>>::iterator layerInsert;
};

} /* namespace Zoe */
