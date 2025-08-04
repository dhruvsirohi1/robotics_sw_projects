add_executable(test_odometry test_odometry.cpp)
target_link_libraries(test_odometry
        robot_localization
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_odometry)