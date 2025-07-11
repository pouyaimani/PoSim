#include <iostream>
#include <plog/Log.h>
#include <plog/Logger.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include "core/timer/timer.h"
#include "core/eventloop/eventloop.h"
#include "core/stateMachine/event.h"
#include "core/stateMachine/core.h"
#include "states/startup.h"
#include "gui/gui.h"

int main() {

    plog::init<plog::TxtFormatter>(plog::debug, plog::streamStdOut);
    PLOG_INFO << "Welcome to PoSim - POS Emulator using LVGL and C++17!";
    auto startUp = State::create<StartUp>(nullptr);
    ui::LVGL::instance().init();
    StateMachine::Core::instance().init(startUp);
    EventLoop::instance().registerChecker([]() noexcept {
        TimerHandler::instance().run();
    });
    StateMachine::Core::instance().registerCb([]() noexcept {
        EventLoop::instance().runCycle();
    });
    StateMachine::Core::instance().registerCb([]() noexcept {
        ui::LVGL::instance().handle();
    });

    StateMachine::Core::instance().exec();
    return 0;
}
