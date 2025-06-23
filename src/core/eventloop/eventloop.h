/*
 * EventLoop.h
 * 
 * A lightweight, thread-safe, single-threaded event loop system for C++17.
 *
 * This class provides a centralized event loop similar to the one used in frameworks
 * like Qt, where components can register their callback functions to be executed 
 * periodically on the main thread. The event loop runs in a continuous loop, calling 
 * all registered callbacks at a regular interval.
 *
 * Features:
 * - Singleton design pattern to ensure only one event loop instance exists.
 * - Thread-safe callback registration using mutex.
 * - Suitable for use cases like timers, event dispatchers, or async pollers.
 *
 * Usage:
 * - Call `EventLoop::instance().exec();` in your `main()` to start the loop.
 * - Register callbacks using `registerCallback()` method.
 * - Stop the loop cleanly using the `stop()` method.
 *
 * Author: Pouya Imani
 * Date: June 18, 2025
 */

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>
#include <vector>
#include <atomic>
#include <mutex>

class EventLoop {
public:
    using Checker = std::function<void()>;

    static EventLoop& instance();

    void runCycle();
    void stop() noexcept;

    void registerChecker(Checker) noexcept;
    void unregisterAll() noexcept;

private:
    EventLoop() = default;
    std::atomic<bool> running{false};
    std::vector<Checker> checkers;
    std::recursive_mutex chMutex;
};

#endif
