add_executable(test_odometry test_odometry.cpp)
target_link_libraries(test_odometry
        robot_localization
        gtest_main
)

add_executable(test_circular_buffer test_circular_buffer.cpp)
target_link_libraries(test_circular_buffer gtest_main)
target_include_directories(test_circular_buffer PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(test_odometry)
gtest_discover_tests(test_circular_buffer)