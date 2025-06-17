#include "timer.h"
#include <plog/Log.h>
#include <mutex>

Timer::Timer() : isPeriodic{false}, isStoped{true}
{
    // Push timer in the Timer Handler
    TimerHandler::getInstance().append(this);
}

Timer::~Timer()
{
    TimerHandler::getInstance().remove(this);
}
void Timer::start(int milliseconds, Callback callback, bool periodic)
{
    isPeriodic = periodic;
    durationMs = milliseconds;
    cxxTimer.start();
    // Set up the callback
    this->callback = std::move(callback);
}

void Timer::stop()
{
    isStoped = true;
}

bool Timer::isRunning() const
{
    return (isStoped == false);
}

void Timer::check()
{
    if (!isStoped && (cxxTimer.count<cxxtimer::ms>() > durationMs)) {
        callback();
        if (isPeriodic) {
            cxxTimer.reset();
            cxxTimer.start();
        } else {
            stop();
        }
    }
}

TimerHandler &TimerHandler::getInstance()
{
    /*
    Use a local static variable, which is guaranteed to 
    be initialized only once in a thread-safe manner by the standard
    */
    static TimerHandler instance;
    return instance;
}

void TimerHandler::append(Timer* timer)
{
    std::lock_guard<std::mutex> lock(timerListMutex);
    // Push timer in the timerList
    timerList.push_back(timer);
    PLOG_DEBUG << "timerList appended. size = " << timerList.size();
}

void TimerHandler::remove(Timer* timer)
{
    std::lock_guard<std::mutex> lock(timerListMutex);
    auto it = std::find(timerList.begin(), timerList.end(), timer);
    if (it != timerList.end()) {
        // Remove the pointer from the timerList
        timerList.erase(it);
    }
    PLOG_DEBUG << "timerList removed. size = " << timerList.size();
}

void TimerHandler::run()
{
    std::lock_guard<std::mutex> lock(timerListMutex);
    for(auto timer : timerList) {
        timer->check();
    }
}