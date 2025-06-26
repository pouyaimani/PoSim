cmake_minimum_required(VERSION 3.16)
project(PoSim VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell LVGL exactly where the lv_conf.h is
set(LV_BUILD_CONF_PATH "${CMAKE_SOURCE_DIR}/include/lv_conf.h")

include(FetchContent)

# LVGL: Download and make available
FetchContent_Declare(
  lvgl
  GIT_REPOSITORY https://github.com/lvgl/lvgl.git
  GIT_TAG release/v9.3
)

FetchContent_MakeAvailable(lvgl)

# Include directories
include_directories(include)

add_subdirectory(src)

add_executable(posim main.cpp)

# Link sources
target_link_libraries(posim PUBLIC libSrc)
# Link LVGL
target_link_libraries(posim PRIVATE lvgl)
