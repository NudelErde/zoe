//
// Created by Florian on 14.11.2020.
//

#include "Task.h"
#include "Scheduler.h"

bool Zoe::Task::resume() {
    handle.resume();
    return handle.promise().shouldRepeat && !handle.done();
}
Zoe::Task::Task(std::coroutine_handle<Zoe::Task::promise_type> handle) : handle(handle) {}

void Zoe::SwitchToSync::await_suspend(std::coroutine_handle<Task::promise_type> h) {
    Scheduler::addTask([h]() { h.resume(); });
}

bool Zoe::SwitchToSync::await_ready() {
    return std::this_thread::get_id() == Scheduler::getThreadID();
}

void Zoe::SwitchToSync::await_resume() {}

bool Zoe::SwitchToAsync::await_ready() {
    { return std::this_thread::get_id() != Scheduler::getThreadID(); }
}

void Zoe::SwitchToAsync::await_suspend(std::coroutine_handle<Task::promise_type> h) {
    h.promise().shouldRepeat = false;
    std::thread thread([h]() {
        h.resume();
    });
    thread.detach();
}
void Zoe::SwitchToAsync::await_resume() {}

Zoe::Task Zoe::Task::promise_type::get_return_object() {
    return Task(std::coroutine_handle<Zoe::Task::promise_type>::from_promise(*this));
}
std::suspend_always Zoe::Task::promise_type::initial_suspend() {
    return {};
}
std::suspend_always Zoe::Task::promise_type::final_suspend() {
    return {};
}
void Zoe::Task::promise_type::return_void() {

}
void Zoe::Task::promise_type::unhandled_exception() {

}
std::suspend_always Zoe::Task::promise_type::yield_value(bool repeat) {
    shouldRepeat = repeat;
    return {};
}
Zoe::WaitTime Zoe::Task::promise_type::await_transform(std::chrono::milliseconds duration) {
    return Zoe::WaitTime(duration);
}
Zoe::WaitTime::WaitTime(std::chrono::milliseconds time) : endpoint(time + std::chrono::steady_clock::now()) {}

bool Zoe::WaitTime::await_ready() {
    return std::chrono::steady_clock::now() >= endpoint;
}
void Zoe::WaitTime::await_suspend(std::coroutine_handle<Task::promise_type> h) {
    h.promise().shouldRepeat = false;
    bool insertTaskInMain = std::this_thread::get_id() == Scheduler::getThreadID();
    std::thread thread([this, h, insertTaskInMain]() {
        std::this_thread::sleep_until(endpoint);
        if(insertTaskInMain) {
            Scheduler::addTask(h.promise().get_return_object());
        } else {
            h.resume();
        }
    });
    thread.detach();
}
void Zoe::WaitTime::await_resume() {}
