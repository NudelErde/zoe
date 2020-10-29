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
class AppTickEvent : public EventTemplate<EventType::AppTick, EventCategory::Application> {

public:
    /**
     * Constructs an AppTickEvent.
     */
    AppTickEvent() {
    }
    /**
     * Destructs the MouseMovedEvent.
     */
    ~AppTickEvent() {}
};

/**
 * An event that indicates that an application update occurred. Updates are made when the status can change.
 */
class AppUpdateEvent : public EventTemplate<EventType::AppUpdate, EventCategory::Application> {

public:
    /**
     * Constructs an AppUpdateEvent.
     */
    AppUpdateEvent() {
    }
    /**
     * Destructs the AppUpdateEvent.
     */
    ~AppUpdateEvent() {}
};

/**
 * An event that indicates that a new Frame should be rendered.
 */
class AppRenderEvent : public EventTemplate<EventType::AppRender, EventCategory::Application> {

public:
    /**
     * Constructs an AppRenderEvent.
     */
    AppRenderEvent() {
    }
    /**
     * Destructs the AppRenderEvent.
     */
    ~AppRenderEvent() {}
};

}
