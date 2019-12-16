/*
 * ApplicationEvent.h
 *
 *  Created on: 05.04.2019
 *      Author: florian
 */

#pragma once

#include "Event.h"
#include "../Core.h"

namespace Zoe {

class DLL_PUBLIC AppTickEvent: public Event {

public:
	AppTickEvent() {
	}
	~AppTickEvent(){}

	EVENT_CLASS_TYPE(AppTick);
	EVENT_CLASS_CATEGORY(EventCategoryApplication);

};

class DLL_PUBLIC AppUpdateEvent: public Event {

public:
	AppUpdateEvent() {
	}
	~AppUpdateEvent(){}
	EVENT_CLASS_TYPE(AppUpdate);
	EVENT_CLASS_CATEGORY(EventCategoryApplication);

};

class DLL_PUBLIC AppRenderEvent: public Event {

public:
	AppRenderEvent() {
	}
	~AppRenderEvent(){}
	EVENT_CLASS_TYPE(AppRender);
	EVENT_CLASS_CATEGORY(EventCategoryApplication);

};

}
