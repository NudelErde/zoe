/*
 * ApplicationEvent.h
 *
 *  Created on: 05.04.2019
 *      Author: florian
 */

#pragma once

#include "Event.h"
#include "../core/Core.h"

namespace Zoe {

/**
 * An event that indicates that an application tick occurred. Ticks happen in fixed intervals.
 */
class DLL_PUBLIC AppTickEvent: public Event {

public:
    /**
     * Constructs an AppTickEvent.
     */
	AppTickEvent() {
	}
    /**
     * Destructs the MouseMovedEvent.
     */
	~AppTickEvent(){}

	EVENT_CLASS_TYPE(AppTick);
	EVENT_CLASS_CATEGORY(EventCategoryApplication);

};

/**
 * An event that indicates that an application update occurred. Updates are made when the status can change.
 */
class DLL_PUBLIC AppUpdateEvent: public Event {

public:
    /**
     * Constructs an AppUpdateEvent.
     */
	AppUpdateEvent() {
	}
    /**
     * Destructs the AppUpdateEvent.
     */
	~AppUpdateEvent(){}
	EVENT_CLASS_TYPE(AppUpdate);
	EVENT_CLASS_CATEGORY(EventCategoryApplication);

};

/**
 * An event that indicates that a new Frame should be rendered.
 */
class DLL_PUBLIC AppRenderEvent: public Event {

public:
    /**
     * Constructs an AppRenderEvent.
     */
	AppRenderEvent() {
	}
    /**
     * Destructs the AppRenderEvent.
     */
	~AppRenderEvent(){}
	EVENT_CLASS_TYPE(AppRender);
	EVENT_CLASS_CATEGORY(EventCategoryApplication);

};

}
