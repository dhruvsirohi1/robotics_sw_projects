cmake_minimum_required(VERSION 3.31)
project(robotics_sw_stuff)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(robot_localization
        src/odometry.cpp
)

#add_library(circular_buffer)
# Tell CMake to use pkg-config
find_package(OpenCV REQUIRED)

add_executable(CameraCalibration include/camera_calibration/CameraCalibration.cpp)
target_link_libraries(CameraCalibration ${OpenCV_LIBS})
target_compile_definitions(CameraCalibration PRIVATE
        CALIB_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/images/calibration"
)

add_executable(CalibrationCompare include/camera_calibration/CompareCalibration.cpp)
target_link_libraries(CalibrationCompare ${OpenCV_LIBS})

# Tell CMake to find libusb
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

# Add include directories
include_directories(${LIBUSB_INCLUDE_DIRS})

# Add linker directories (for .dylib/.a files)
link_directories(${LIBUSB_LIBRARY_DIRS})
add_executable(UsbDebug include/usb_debug/UsbDebug.cpp)
target_link_libraries(UsbDebug PRIVATE ${LIBUSB_LIBRARIES})

# ---- GoogleTest Setup ----
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(test)
