cmake_minimum_required(VERSION 3.16)
project(PoSim VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DASSETS_DIR="${CMAKE_SOURCE_DIR}/assets")

add_subdirectory(src)