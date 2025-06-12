#include <iostream>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Logger.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>


int main() {

    plog::init<plog::TxtFormatter>(plog::debug, plog::streamStdOut);
    PLOG_INFO << "Welcome to PoSim - POS Emulator using LVGL and C++17!";
    return 0;
}
