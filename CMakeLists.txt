cmake_minimum_required(VERSION 3.31)
project(dexterity)

set(CMAKE_CXX_STANDARD 20)

add_library(dexterity STATIC
        include/robot_localization/pose2d.h
        include/robot_localization/Odometry.h)
