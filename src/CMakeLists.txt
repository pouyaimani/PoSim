# Tell LVGL exactly where the lv_conf.h is
set(LV_BUILD_CONF_PATH "${CMAKE_SOURCE_DIR}/include/lv_conf.h")

# SDL2
find_package(SDL2 REQUIRED)

include(FetchContent)

# LVGL: Download and make available
FetchContent_Declare(
  lvgl
  GIT_REPOSITORY https://github.com/lvgl/lvgl.git
  GIT_TAG release/v9.3
)

FetchContent_MakeAvailable(lvgl)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)

add_executable(posim ${SOURCE_FILES})

target_include_directories(posim PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${PNG_INCLUDE_DIR}
)

find_package(PNG REQUIRED)

# Link LVGL
target_link_libraries(posim PRIVATE lvgl ${SDL2_LIBRARIES} PNG::PNG)