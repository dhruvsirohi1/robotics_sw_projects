cmake_minimum_required(VERSION 3.31)
project(robotics_sw_stuff)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(robot_localization
        src/odometry.cpp
)

add_library(dexterity STATIC
        include/robot_localization/pose2d.h
        include/robot_localization/Odometry.h
        src/Odometry.cpp)
