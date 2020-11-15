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
static std::mutex listMutex;
static bool running = true;

std::thread::id Zoe::Scheduler::getThreadID() {
    return std::thread::id();
}
void Zoe::Scheduler::addTask(Zoe::Task task) {
    tasks.push_back(task);
}
void Zoe::Scheduler::addTask(const std::function<void()>& function) {
    functions.push_back(function);
}
void Zoe::Scheduler::execute() {
    std::unique_lock lock(executeLock, std::defer_lock_t());
    if (lock.try_lock()) {
        running = true;
        while (running) {
            {
                std::lock_guard lg(listMutex);
                for (auto& function: functions) {
                    function();
                }
                functions.clear();
                std::erase_if(tasks, [](Task t) -> bool {
                    bool shouldRepeat = t.resume();
                    return !shouldRepeat;
                });
            }
        }
    } else {
        error("Scheduler::execute should not be called twice");
    }
}
void Zoe::Scheduler::exit() {
    running = false;
}
void Zoe::Scheduler::addTask(const std::function<Task()>& task) {
    addTask(task());
}
