#ifndef TIMER_H
#define TIMER_H

#include <cxxtimer.h>
#include <list>
#include <mutex>

using namespace std;

class Timer {
public:
    using Callback = std::function<void()>;

    // Constructor
    Timer();

    // Destructor
    ~Timer();

    // Start the timer with a delay in milliseconds and a callback function
    void start(int milliseconds, Callback callback);

    // Stop the timer
    void stop();
    bool isRunning() const;

private:
    cxxtimer::Timer cxxTimer;
    bool isPeriodic;
    bool isStoped;
    long long durationMs;
    Callback callback;           // Callback function
    void check();
    friend class TimerHandler;
};

class TimerHandler {

private:
    TimerHandler() = default;
    list<Timer*> timerList;
    std::mutex timerListMutex;

public:
    static TimerHandler & getInstance();
    void append(Timer* timer);
    void remove(Timer* timer);
    void run();
};

#endif