#include <iostream>
#include <plog/Log.h>
#include <plog/Logger.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include "src/core/timer/timer.h"
#include "src/core/eventloop/eventloop.h"
#include "src/core/stateMachine/event.h"
#include "src/core/stateMachine/core.h"
#include "src/states/startup.h"

int main() {

    plog::init<plog::TxtFormatter>(plog::debug, plog::streamStdOut);
    PLOG_INFO << "Welcome to PoSim - POS Emulator using LVGL and C++17!";
    StateMachine::Core::instance().init(new StartUp(nullptr));
    EventLoop::instance().registerChecker([]() noexcept {
        TimerHandler::instance().run();
    });
    StateMachine::Core::instance().registerCb([]() noexcept {
        EventLoop::instance().runCycle();
    });

    StateMachine::Core::instance().exec();
    return 0;
}
