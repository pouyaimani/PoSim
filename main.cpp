#include <iostream>
#include <plog/Log.h>
#include <plog/Logger.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include "src/core/timer/timer.h"
int main() {

    plog::init<plog::TxtFormatter>(plog::debug, plog::streamStdOut);
    PLOG_INFO << "Welcome to PoSim - POS Emulator using LVGL and C++17!";
    {
        Timer t;
        t.start(1000, []() {
            PLOG_INFO << "timer time out";
        });
    }
    {
        Timer t;
        t.start(1000, []() {
            PLOG_INFO << "timer time out";
        });
        {
            Timer t;
            t.start(1000, []() {
                PLOG_INFO << "timer time out";
            });
            {
                Timer t;
                t.start(1000, []() {
                    PLOG_INFO << "timer time out";
                });
            }
        }
    }
    {
        Timer t;
        t.start(1000, []() {
            PLOG_INFO << "timer time out";
        });
    }
    while (1) {
        TimerHandler::getInstance().run();
    }
    
    return 0;
}
