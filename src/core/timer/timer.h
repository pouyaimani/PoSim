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
    void start(int milliseconds, Callback callback) noexcept;

    // Stop the timer
    void stop() noexcept;
    bool isRunning() const noexcept;
    static void singleShot(int milliseconds, Callback callback);

private:
    cxxtimer::Timer cxxTimer;
    bool isStoped {true};
    long long durationMs;
    Callback callback;
    bool isSingleShot {false};
    bool check();
    friend class TimerHandler;
};

class TimerHandler {

private:
    TimerHandler() = default;
    TimerHandler(const TimerHandler&) = delete;
    TimerHandler& operator=(const TimerHandler&) = delete;
    TimerHandler(TimerHandler&&) = delete;
    TimerHandler& operator=(TimerHandler&&) = delete;

    list<Timer*> timerList;
    std::mutex timerListMutex;

public:
    static TimerHandler & instance();
    void append(Timer* timer) noexcept;
    void remove(Timer* timer) noexcept;
    void run() noexcept;
};

#endif