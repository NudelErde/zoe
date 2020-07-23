/*
 * Layer.h
 *
 *  Created on: 11.04.2019
 *      Author: florian
 */

#pragma once

#include "Core.h"
#include "../zpch.h"
#include "../event/CommonEvent.h"

namespace Zoe {

class DLL_PUBLIC Layer {
friend class LayerStack;
public:
	Layer(const std::string& name);
	virtual ~Layer();

	virtual void onAttach() {
	}
	virtual void onDetach() {
	}
	virtual void onEvent(Event& event) {
	}

	inline const std::string& getName() const {
		return name;
	}

	inline void setEnabled(bool enabled) {
		this->enabled = enabled;
	}
	inline bool isEnabled() const {
		return this->enabled;
	}

	inline int getSubscribedEvents() const {
		return this->subscribedEvents;
	}

	inline void setSubscribedEvents(int events){
		this->subscribedEvents = events;
	}

protected:
	std::string name;
	bool enabled = true;
	int subscribedEvents = EventCategory::EventCategoryNone;
};

} /* namespace Zoe */
