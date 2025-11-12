#include "utility.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string getTimeOfDayLocal()
{
    // Get current time as time_t
    std::time_t t = std::time(nullptr);

    // Convert to local time
    std::tm local_tm{};
#ifdef _WIN32
    localtime_s(&local_tm, &t);  // Windows
#else
    localtime_r(&t, &local_tm);  // Linux/macOS
#endif

    // Format as HH:MM:SS
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%H:%M:%S");
    return oss.str();
};