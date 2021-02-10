//
// Created by Florian on 14.11.2020.
//

#include "Task.h"
#include "Scheduler.h"
#include "Console.h"

namespace Zoe {

bool Task::resume() {

    handle.resume();
    return handle.promise().shouldRepeat && !handle.done();
}
Task::Task(std::coroutine_handle<Task::promise_type> handle) : handle(handle) {}

void SwitchToSync::await_suspend(std::coroutine_handle<Task::promise_type> h) {
    Scheduler::addCoroutineContainer(Scheduler::CoroutineContainer(h.promise().get_return_object(),
                                                                   Scheduler::getCurrentCoroutineCallableObject()));
}

bool SwitchToSync::await_ready() {
    return std::this_thread::get_id() == Scheduler::getThreadID();
}

void SwitchToSync::await_resume() {}

bool SwitchToAsync::await_ready() {
    { return std::this_thread::get_id() != Scheduler::getThreadID(); }
}

void SwitchToAsync::await_suspend(std::coroutine_handle<Task::promise_type> h) {
    h.promise().shouldRepeat = false;
    std::thread thread([h]() {
        h.resume();
    });
    thread.detach();
}
void SwitchToAsync::await_resume() {}

Task Task::promise_type::get_return_object() {
    return Task(std::coroutine_handle<Task::promise_type>::from_promise(*this));
}
std::suspend_always Task::promise_type::initial_suspend() {
    return {};
}
std::suspend_always Task::promise_type::final_suspend() {
    return {};
}
void Task::promise_type::return_void() {

}
void Task::promise_type::unhandled_exception() {
    throw;
}
std::suspend_always Task::promise_type::yield_value(bool repeat) {
    shouldRepeat = repeat;
    return {};
}
WaitTime::WaitTime(std::chrono::time_point<std::chrono::steady_clock> time) : endpoint(time) {}

bool WaitTime::await_ready() {
    return std::chrono::steady_clock::now() >= endpoint;
}
void WaitTime::await_suspend(std::coroutine_handle<Task::promise_type> h) {
    h.promise().shouldRepeat = false;
    bool insertTaskInMain = std::this_thread::get_id() == Scheduler::getThreadID();
    std::shared_ptr<void> coroutineContextObject = Scheduler::getCurrentCoroutineCallableObject();
    std::thread thread([this, h, insertTaskInMain, coroutineContextObject]() {
        std::this_thread::sleep_until(endpoint);
        if (insertTaskInMain) {
            Scheduler::addCoroutineContainer(
                    Scheduler::CoroutineContainer(h.promise().get_return_object(), coroutineContextObject));
        } else {
            h.resume();
        }
    });
    thread.detach();
}
void WaitTime::await_resume() {}


}
