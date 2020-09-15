/*
 * LayerStack.cpp
 *
 *  Created on: 11.04.2019
 *      Author: florian
 */

#include "LayerStack.h"

namespace Zoe {

LayerStack::LayerStack() {
	layerInsert = layers.begin();
}

LayerStack::~LayerStack() = default;

void LayerStack::pushLayer(const std::shared_ptr<Layer>& layer) {
	layer->onAttach();
	layerInsert = layers.emplace(layerInsert, layer);
}
void LayerStack::pushOverlay(const std::shared_ptr<Layer>& layer) {
	layer->onAttach();
	layers.emplace_back(layer);
}

void LayerStack::popLayer(const std::shared_ptr<Layer>& layer) {
	layer->onDetach();
	auto it = std::find(layers.begin(), layers.end(), layer);
	if (it != layers.end()) {
		layers.erase(it);
		layerInsert--;
	}
}

void LayerStack::popOverlay(const std::shared_ptr<Layer>& layer) {
	layer->onDetach();
	auto it = std::find(layers.begin(), layers.end(), layer);
	if (it != layers.end()) {
		layers.erase(it);
	}
}

} /* namespace Zoe */
