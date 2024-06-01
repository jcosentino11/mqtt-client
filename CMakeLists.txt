cmake_minimum_required(VERSION 3.0)

project(MqttClient VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(FLAG_BUILD_TESTS "Build unit tests" OFF)

include(cmake/clang-format.cmake)

include_directories(deps/include)

file(GLOB_RECURSE SOURCES src/*.cpp deps/src/*.cpp)

add_executable(mqtt ${SOURCES})
target_include_directories(mqtt PUBLIC include)

if(FLAG_BUILD_TESTS)
    add_subdirectory(test)
endif()
