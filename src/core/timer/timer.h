#ifndef TIMER_H
#define TIMER_H

#include <cxxtimer.h>
#include <list>
#include <mutex>

using namespace std;

class _TimerBase {
public:
    // Constructor
    _TimerBase() = default;

    // Destructor
    ~_TimerBase() {};
private:
    using Callback = std::function<void()>;
    using atomicBool = std::atomic<bool>;

    // Start the timer with a delay in milliseconds and a callback function
    void start(int milliseconds, Callback callback) noexcept;

    // Stop the timer
    void stop() noexcept;
    bool isRunning() const noexcept;
    cxxtimer::Timer cxxTimer;
    atomicBool isStoped {true};
    long long durationMs;
    Callback callback;
    atomicBool isSingleShot {false};
    bool check();
    atomicBool isExpired {false};
    std::mutex mtx;
    friend class TimerHandler;
    friend class Timer;
};

class Timer {
public:
    using Callback = std::function<void()>;

    // Constructor
    Timer();

    ~Timer();

    // Start the timer with a delay in milliseconds and a callback function
    void start(int milliseconds, Callback callback) noexcept;

    // Stop the timer
    void stop() noexcept;
    bool isRunning() const noexcept;
    static void singleShot(int milliseconds, Callback callback);

private:
    bool check();
    std::shared_ptr<_TimerBase> timerBase;
};

class TimerHandler {

private:
    TimerHandler() = default;
    TimerHandler(const TimerHandler&) = delete;
    TimerHandler& operator=(const TimerHandler&) = delete;
    TimerHandler(TimerHandler&&) = delete;
    TimerHandler& operator=(TimerHandler&&) = delete;

    list<std::shared_ptr<_TimerBase>> timerList;
    std::mutex timerListMutex;

public:
    static TimerHandler & instance();
    void append(std::shared_ptr<_TimerBase>) noexcept;
    void remove(std::shared_ptr<_TimerBase>) noexcept;
    void run() noexcept;
};

#endif