//
// Created by Florian on 14.11.2020.
//

#include "Scheduler.h"
#include <vector>
#include <mutex>
#include "Console.h"

static std::mutex executeLock;
static std::vector<std::function<void()>> functions;
static std::vector<Zoe::Task> tasks;
static std::vector<std::function<void()>> newFunctions;
static std::vector<Zoe::Task> newTasks;
static std::mutex listMutex;
static std::thread::id schedulerThreadID;
static bool running = true;

std::thread::id Zoe::Scheduler::getThreadID() {
    return schedulerThreadID;
}
void Zoe::Scheduler::addTask(Zoe::Task&& task) {
    std::lock_guard lg(listMutex);
    newTasks.push_back(std::move(task));
}
void Zoe::Scheduler::addTask(const std::function<void()>& function) {
    std::lock_guard lg(listMutex);
    newFunctions.push_back(function);
}
void Zoe::Scheduler::execute() {
    std::unique_lock lock(executeLock, std::defer_lock_t());
    if (lock.try_lock()) {
        schedulerThreadID = std::this_thread::get_id();
        running = true;
        while (running) {
            for (auto& function: functions) {
                function();
            }
            std::erase_if(tasks, [](Task& t) -> bool {
                bool shouldRepeat = t.resume();
                return !shouldRepeat;
            });
            {
                std::lock_guard lg(listMutex);
                functions = std::move(newFunctions);
                newFunctions = std::vector<std::function<void()>>();
                for(auto& task : newTasks) {
                    tasks.push_back(std::move(task));
                }
                newTasks.clear();
            }

        }
    } else {
        error("Scheduler::execute should not be called twice");
    }
}
void Zoe::Scheduler::exit() {
    running = false;
}
