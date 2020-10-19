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

/**
 * A Layer can be added to the Applications LayerStack. A Layer handles events if the event is not already handled.
 * @see LayerStack
 */
class DLL_PUBLIC Layer {
friend class LayerStack;
public:

    /**
     * Creates a new Layer with the specified name.
     * @param name the specified name
     */
	explicit Layer(std::string name);

	/**
	 * Destructs the Layer.
	 */
	virtual ~Layer();

	/**
	 * Called when the Layer is inserted in the LayerStack.
	 */
	virtual void onAttach() {}

	/**
	 * Called when the Layer is removed from the LayerStack.
	 */
	virtual void onDetach() {}

	/**
	 * Called when a subscribed event is triggered.
	 * @param event the triggered event
	 */
	virtual void onEvent(Event& event) {}

	/**
	 * Returns the name of the Layer.
	 * @return the name of the Layer
	 */
	[[nodiscard]] inline const std::string& getName() const {
		return name;
	}

	/**
	 * Returns the flag that describes which events the Layer has subscribed to.
	 * @return the subscribed events
	 */
	[[nodiscard]] inline unsigned int getSubscribedEvents() const {
		return this->subscribedEvents;
	}

    /**
     * Specifies the flag that describes which events the Layer has subscribed to.
     * @param events the subscribed events
     */
	inline void setSubscribedEvents(unsigned int events){
		this->subscribedEvents = events;
	}

protected:
	std::string name;
	bool enabled = true;
	unsigned int subscribedEvents = static_cast<unsigned int>(EventCategory::None);
};

} /* namespace Zoe */
