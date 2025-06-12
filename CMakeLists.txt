cmake_minimum_required(VERSION 3.16)
project(PoSim VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(posim ${SOURCES})

# You can link LVGL and other dependencies here later
# target_link_libraries(posim lvgl ...)
