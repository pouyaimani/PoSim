file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)

add_library(libSrc OBJECT ${SOURCE_FILES})

target_include_directories(libSrc PUBLIC ${CMAKE_SOURCE_DIR}/include)
