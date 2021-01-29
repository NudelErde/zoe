//
// Created by Florian on 14.11.2020.
//

#pragma once
#include <thread>
#include <functional>
#include <concepts>
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
     * Adds a function. The function will be executed once. The Scheduler is a cooperative Scheduler so the function
     * should not take to long since it will block other tasks from being executed.
     * @param function the added function
     */
    static void addTask(const std::function<void()>& function);

    template<typename T>
    static void addCoroutine(const T& object) {
        std::shared_ptr<T> thing = std::make_shared<T>(std::move(object));
        Task task = thing->operator()();
        addCoroutineContainer(CoroutineContainer(std::move(task), std::move(thing)));
    }

    /**
     * Sets a bool so that execute leaves the loop at the end of this iteration.
     */
    static void exit();

    /**
     * Executes the tasks in this thread. This function returns after exit is called. Only one thread should call execute.
     * If another thread executes this function, it prints an error and returns with no further effect.
     */
    static void execute();

    struct CoroutineContainer{
        std::unique_ptr<Task> task;
        std::shared_ptr<void> callableObject;

        CoroutineContainer() = default;
        CoroutineContainer(Task&& task, std::shared_ptr<void> callableObject);
        CoroutineContainer(CoroutineContainer&&) noexcept = default;
        CoroutineContainer& operator=(CoroutineContainer&&) noexcept  = default;
    };

    [[nodiscard]] static std::shared_ptr<void> getCurrentCoroutineCallableObject();
    static void addCoroutineContainer(CoroutineContainer&& cc);
private:

    static inline std::shared_ptr<void> currentCoroutineCallableObject;
};

}
