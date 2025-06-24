#include "eventloop.h"
#include <chrono>
#include <thread>

EventLoop& EventLoop::instance()
{
    static EventLoop instance;
    return instance;
}

void EventLoop::runCycle()
{
    {
        std::lock_guard<std::recursive_mutex> lock(chMutex);
        for (auto& ch : checkers) {
            ch();  // Call each registered callback
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // loop interval
}

void EventLoop::stop() noexcept
{
    running = false;
}

void EventLoop::registerChecker(Checker ch) noexcept
{
    std::lock_guard<std::recursive_mutex> lock(chMutex);
    checkers.push_back(std::move(ch));
}

void EventLoop::unregisterAll() noexcept
{
    std::lock_guard<std::recursive_mutex> lock(chMutex);
    checkers.clear();
}
