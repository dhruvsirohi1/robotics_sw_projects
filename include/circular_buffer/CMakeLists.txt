project(circular_buffer)

add_library(circular_buffer INTERFACE)
target_include_directories(circular_buffer INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Optional: Test support
enable_testing()
add_subdirectory(test)