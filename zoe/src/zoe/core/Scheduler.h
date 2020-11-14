//
// Created by Florian on 14.11.2020.
//

#pragma once
#include <thread>
#include <functional>
#include "Task.h"

namespace Zoe {

/**
 * The Scheduler executes Tasks and functions in the main thread. The main thread should call Scheduler::execute after
 * adding at least one task or starting another thread.
 */
class Scheduler {
public:
    /**
     * Returns the thread id of the thread that runs the execute function.
     * @return
     */
    static std::thread::id getThreadID();

    /**
     * Adds a Task. The task will be executed until it is finished or it executes `co_yield false;`. The Scheduler is a
     * cooperative Scheduler so the task has to call `co_yield true;` for other tasks to execute. If the task has an infinite while loop,
     * it should call `co_yield true;` at least once per iteration.
     * @param task the added task
     */
    static void addTask(Task task);

    /**
     * Adds a function. The function will be executed once. The Scheduler is a cooperative Scheduler so the function
     * should not take to long since it will block other tasks from being executed.
     * @param function the added function
     */
    static void addTask(const std::function<void()>& function);

    /**
     * Sets a bool so that execute leaves the loop at the end of this iteration.
     */
    static void exit();
public:

    /**
     * Executes the tasks in this thread. This function returns after exit is called. Only one thread should call execute.
     * If another thread executes this function, it prints an error and returns with no further effect.
     */
    static void execute();
};

}
