//
// Created by Florian on 14.11.2020.
//

#pragma once

#include <coroutine>
#include <thread>
#include <chrono>


namespace Zoe {

/**
 * If a function returns a Task it can be used as a coroutine. A coroutine can suspend it's execution an return to the caller.
 * A Task can be added to the Scheduler. If `co_yield true;` is executed the function suspends execution and another Task is executed.
 * It will be resumed after all other tasks finished or called `co_yield true;`.\n
 * If `co_yield false;` is executed the function finishes and another Task is executed. It won't be resumed.
 * The function can switch the thread in which it is executed.\n
 * `co_await SwitchToAsync();` continues it's execution on a thread that is not the main thread.
 * This can be used to perform IO operations or big calculations without blocking the rest of the program.\n
 * `co_await SwitchToSync();` continues it's execution on the main thread.
 * This can be used to ensure that the Zoe API is in a working state when interacting with it.
 * This avoids undefined behavior.\n
 * `co_await std::chrono::duration` can be used to wait for the specified time.
 * If the Task is executed in the main thread it will continue in the main thread.
 * If the Task is executed in another thread, it will continue on a thread that is not the main thread.\n
 */
class Task {
public:
    /**
     * The promise_type enables a function that returns a task as a coroutine.
     */
    struct promise_type;

    /**
     * Creates a Task from a coroutine handle. This should be called from Task::promise_type::get_return_object();
     * @param handle the coroutine handle
     */
    explicit Task(std::coroutine_handle<promise_type> handle);

    /**
     * Resumes the tasks execution. Returns true if it can be continued. It shouldn't be resumed when it called `co_yield false;` or it reached it's end.
     * @return `true` if it can be continued
     */
    bool resume();
private:
    std::coroutine_handle<promise_type> handle;
};

class WaitTime;

/**
 * The promise_type enables a function that returns a task as a coroutine.
 */
struct Task::promise_type {
    /**
     * Creates the return object of the call to the coroutine.
     * @return the return object of the call to the coroutine
     */
    Task get_return_object();

    /**
     * Suspends the task at the beginning. This method should only be called by the coroutine implementation.
     * @return suspend_always
     */
    std::suspend_always initial_suspend();

    /**
     * Suspends the task at the end. This method should only be called by the coroutine implementation.
     * @return suspend_always
     */
    std::suspend_always final_suspend();

    /**
     * This function is called when the function returns. This method should only be called by the coroutine implementation.
     */
    void return_void();

    /**
     * This function is called when an unhandled exception occurred in the coroutine. This method should only be called by the coroutine implementation.
     */
    void unhandled_exception();

    /**
     * This function is called when `co_yield bool;` is called. This method should only be called by the coroutine implementation.
     * @param repeat `true` if this function should be repeated
     * @return suspend_always
     */
    std::suspend_always yield_value(bool repeat);

    /**
     * This function is called when `co_await std::chrono::duration;` is called. It converts the duration in a WaitTime object. This method should only be called by the coroutine implementation.
     * @param duration the specified duration
     * @return the co_awaited WaitTime object
     */
    WaitTime await_transform(std::chrono::milliseconds duration);

    /**
     * `true` if the coroutine should be resumed.
     */
    bool shouldRepeat;
};


/**
 * Is used to change the executing thread.
 * `co_await SwitchToSync();` continues it's execution on the main thread.
 * This can be used to ensure that the Zoe API is in a working state when interacting with it.
 * This avoids undefined behavior.
 */
class SwitchToSync {
public:
    /**
     * Checks if the executing thread is already the main thread. This method should only be called by the coroutine implementation.
     * @return `true` if the executing thread is the main thread
     */
    bool await_ready();

    /**
     * Suspends the execution of the coroutine and continues it on the main thread. This method should only be called by the coroutine implementation.
     * @param h the coroutine handle
     */
    void await_suspend(std::coroutine_handle<Task::promise_type> h);

    /**
     * Is called before the coroutine is resumed. This method should only be called by the coroutine implementation.
     */
    void await_resume();
};

/**
 * Is used to change the executing thread.
 * `co_await SwitchToAsync();` continues the coroutines execution on a thread that is not the main thread.
 * This can be used to perform IO operations or big calculations without blocking the rest of the program.
 */
class SwitchToAsync {
public:
    /**
     * Checks if the executing thread is not the main thread. This method should only be called by the coroutine implementation.
     * @return `true` if the executing thread is not the main thread
     */
    bool await_ready();

    /**
     * Suspends the execution of the coroutine and continues it on a non main thread. This method should only be called by the coroutine implementation.
     * @param h the coroutine handle
     */
    void await_suspend(std::coroutine_handle<Task::promise_type> h);

    /**
     * Is called before the coroutine is resumed. This method should only be called by the coroutine implementation.
     */
    void await_resume();
};

/**
 * Is used to suspends the coroutines execution for a specified duration.
 * `co_await std::chrono::duration` can be used to wait for the specified time.
 * If the Task is executed in the main thread it will continue in the main thread.
 * If the Task is executed in another thread, it will continue on a thread that is not the main thread.
 */
class WaitTime {
public:
    /**
     * Constructs the WaitTime object with a specified duration.
     * @param milliseconds the duration
     */
    explicit WaitTime(std::chrono::milliseconds milliseconds);

    /**
     * Check if the duration is already passed. This method should only be called by the coroutine implementation.
     * @return `true` if the duration is already passed
     */
    bool await_ready();

    /**
     * Suspends the execution of this coroutine and continues it after the specified duration. This method should only be called by the coroutine implementation.
     * @param h the coroutine handle
     */
    void await_suspend(std::coroutine_handle<Task::promise_type> h);

    /**
     * Is called before the coroutine is resumed. This method should only be called by the coroutine implementation.
     */
    void await_resume();
private:
    std::chrono::time_point<std::chrono::steady_clock> endpoint;
};

}