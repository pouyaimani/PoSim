#include "eventloop.h"
#include <chrono>
#include <thread>

EventLoop& EventLoop::instance()
{
    static EventLoop instance;
    return instance;
}

void EventLoop::exec()
{
    running = true;
    while (running) {
        {
            std::lock_guard<std::mutex> lock(cbMutex);
            for (auto& cb : callbacks) {
                cb();  // Call each registered callback
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // loop interval
    }
}

void EventLoop::stop()
{
    running = false;
}

void EventLoop::registerCallback(Callback cb)
{
    std::lock_guard<std::mutex> lock(cbMutex);
    callbacks.push_back(std::move(cb));
}

void EventLoop::unregisterAll()
{
    std::lock_guard<std::mutex> lock(cbMutex);
    callbacks.clear();
}
