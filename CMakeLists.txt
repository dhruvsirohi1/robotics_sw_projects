cmake_minimum_required(VERSION 3.31)
project(robotics_sw_stuff)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(robot_localization
        src/odometry.cpp
)

# ---- GoogleTest Setup ----
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(test)
