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

/**
 * The LayerStack stores the Layers.
 * @todo Rework this
 */
class DLL_PUBLIC LayerStack {
public:

    /**
     * Creates an empty LayerStack.
     */
	LayerStack();

	/**
	 * Destructs the LayerStack.
	 */
	~LayerStack();

	/**
	 * idk
	 * @param layer
	 */
	void pushLayer(const std::shared_ptr<Layer>& layer);

	/**
	 * idk
	 * @param layer
	 */
	void pushOverlay(const std::shared_ptr<Layer>& layer);

	/**
	 * idk
	 * @param layer
	 */
	void popLayer(const std::shared_ptr<Layer>& layer);

	/**
	 * idk
	 * @param layer
	 */
	void popOverlay(const std::shared_ptr<Layer>& layer);

	/**
	 * Returns an iterator at the top of the stack.
	 * @return the iterator
	 */
	inline std::vector<std::shared_ptr<Layer>>::iterator begin() {
		return layers.begin();
	}

	/**
	 * Returns an iterator at the bottom of the stack.
	 * @return the iterator
	 */
	inline std::vector<std::shared_ptr<Layer>>::iterator end() {
		return layers.end();
	}

    /**
     * Returns a reverse iterator at the bottom of the stack.
     * @return the iterator
     */
	inline std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() {
		return layers.rbegin();
	}

    /**
     * Returns a reverse iterator at the top of the stack.
     * @return the iterator
     */
	inline std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() {
		return layers.rend();
	}

	/**
	 * idk
	 * @tparam T
	 * @param event
	 */
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
