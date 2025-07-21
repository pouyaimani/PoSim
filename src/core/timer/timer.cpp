#include "timer.h"
#include <plog/Log.h>

/*
A single-use or periodic timer that triggers a callback after a set duration.
Managed centrally by TimerHandler.
*/

void _TimerBase::start(int milliseconds, Callback callback) noexcept
{
    isStoped.store(false);
    durationMs = milliseconds;
    // Ensure clean start of timer
    cxxTimer.reset();
    cxxTimer.start();
    // Set up the callback
    this->callback = std::move(callback);
}

void _TimerBase::stop() noexcept
{
    isStoped.store(true);
}

bool _TimerBase::isRunning() const noexcept
{
    return (isStoped.load() == false);
}

bool _TimerBase::check()
{
    if (!isStoped.load() && (cxxTimer.count<cxxtimer::ms>() > durationMs)) {
    try {
        callback();
    } catch (const std::exception&) {
        // Will be provided soon
    }

        cxxTimer.reset();
        cxxTimer.start();
        return true;
    }
    return false;
}

Timer::Timer()
{
    timerBase.reset(new _TimerBase);
    // Push timer in the Timer Handler
    TimerHandler::instance().append(timerBase);
}

Timer::~Timer()
{
    stop();
    timerBase->isExpired.store(true);
}

void Timer::start(int milliseconds, Callback callback) noexcept
{
    timerBase->start(milliseconds, std::move(callback));
}

void Timer::stop() noexcept
{
    timerBase->stop();
}

bool Timer::isRunning() const noexcept
{
    return timerBase->isRunning();
}

void Timer::singleShot(int milliseconds, Callback callback)
{
    auto timerBase = std::make_shared<_TimerBase>();
    timerBase->isSingleShot.store(true);
    TimerHandler::instance().append(timerBase);
    timerBase->start(milliseconds, callback);
}

bool Timer::check()
{
    return timerBase->check();
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

void TimerHandler::append(std::shared_ptr<_TimerBase> timer) noexcept
{
    std::lock_guard<std::mutex> lock(timerListMutex);
    // Push timer in the timerList
    timerList.push_back(timer);
}

void TimerHandler::remove(std::shared_ptr<_TimerBase> timer) noexcept
{
    std::lock_guard<std::mutex> lock(timerListMutex);
    auto it = std::find(timerList.begin(), timerList.end(), timer);
    if (it != timerList.end()) {
        // Remove the timer pointer from the timerList
        timerList.erase(it);
    }
}

void TimerHandler::run() noexcept
{
    list<std::shared_ptr<_TimerBase>> deletionList;
    /* Snapshot is used to make a copy of TimerList for pulling out user callable 
     * under the mutex lock
     */
    list<std::shared_ptr<_TimerBase>> snapshot;

    {
        std::unique_lock<std::mutex> lock(timerListMutex);
        snapshot = timerList;
    }

    for (auto& timer : snapshot) {
        if (!timer->isExpired.load()) {
            if (timer->check()) {
                if (timer->isSingleShot.load()) {
                    timer->isExpired.store(true);
                }
            }
        }
    }

    {
        std::unique_lock<std::mutex> lock(timerListMutex);
        timerList.remove_if([](const std::shared_ptr<_TimerBase>& timer) {
            return timer->isExpired.load();
        });
    }
}