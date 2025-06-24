#include "timer.h"
#include <plog/Log.h>

/*
A single-use or periodic timer that triggers a callback after a set duration.
Managed centrally by TimerHandler.
*/

Timer::Timer()
{
    // Push timer in the Timer Handler
    TimerHandler::instance().append(this);
}

Timer::~Timer()
{
    TimerHandler::instance().remove(this);
}
void Timer::start(int milliseconds, Callback callback) noexcept
{
    isStoped = false;
    durationMs = milliseconds;
    // Ensure clean start of timer
    cxxTimer.reset();
    cxxTimer.start();
    // Set up the callback
    this->callback = std::move(callback);
}

void Timer::stop() noexcept
{
    isStoped = true;
}

bool Timer::isRunning() const noexcept
{
    return (isStoped == false);
}

void Timer::singleShot(int milliseconds, Callback callback)
{
    Timer *timer = new Timer;
    timer->isSingleShot = true;
    timer->start(milliseconds, callback);
}

bool Timer::check()
{
    if (!isStoped && (cxxTimer.count<cxxtimer::ms>() > durationMs)) {
        callback();
        cxxTimer.reset();
        cxxTimer.start();
        return true;
    }
    return false;
}

TimerHandler &TimerHandler::instance()
{
    /*
    Use a local static variable, which is guaranteed to 
    be initialized only once in a thread-safe manner by the standard
    */
    static TimerHandler instance;
    return instance;
}

void TimerHandler::append(Timer* timer) noexcept
{
    std::lock_guard<std::recursive_mutex> lock(timerListMutex);
    // Push timer in the timerList
    timerList.push_back(timer);
}

void TimerHandler::remove(Timer* timer) noexcept
{
    std::lock_guard<std::recursive_mutex> lock(timerListMutex);
    auto it = std::find(timerList.begin(), timerList.end(), timer);
    if (it != timerList.end()) {
        // Remove the timer pointer from the timerList
        timerList.erase(it);
    }
}

void TimerHandler::run() noexcept
{
    std::unique_lock<std::recursive_mutex> lock(timerListMutex);
    for (auto it = timerList.begin() ; it != timerList.end();) {
        auto& timer = *it;
        ++it;
        if (timer->check() && timer->isSingleShot) {
            // Delete memory of single shot timer.
            lock.unlock();
            delete timer;
            lock.lock();    
        }
    }
}