//
// Created by Dhruv Sirohi on 9/21/25.
//
// CameraCalibrationSkeleton.cpp

#include <filesystem>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <__filesystem/filesystem_error.h>

namespace fs = std::filesystem;

struct Point3d {
    double x;
    double y;
    double z;
};

int main() {
    // ---------------------------
    // Step 1: Define calibration parameters
    // ---------------------------
    // (board width, height, square size, number of images, etc.)
    float board_width = 9.0f; // cm
    float board_height = 6.5f;
    float square_size = 0.9f;
    int n_images = 0;

    // ---------------------------
    // Step 2: Prepare 3D object points
    // ---------------------------
    // (generate coordinates for checkerboard corners in real-world space)
    std::vector<cv::Point3f> coordinates3d;
    int n_squares_hor = board_width / square_size - 1;
    int n_squares_vert = board_height / square_size - 1;

    std::cout << "Board Size (x, y): " << n_squares_hor << ", " << n_squares_vert << std::endl;
    cv::Size boardSize(n_squares_hor, n_squares_vert);

    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;

    for (int i = 0; i < n_squares_vert; i++) {
        for (int j = 0; j < n_squares_hor; j++) {
            coordinates3d.emplace_back(j * square_size, i * square_size, 0);
        }
    }

    std::cout << "Num corner points: " << coordinates3d.size() << std::endl;
    // ---------------------------
    // Step 3: For each calibration image
    // ---------------------------
    //   - Load image
    //   - Convert to grayscale
    //   - Find chessboard corners
    //   - Refine corner locations
    //   - Store corresponding 2D (image) and 3D (world) points
    std::string images_path = CALIB_DATA_DIR;

    try {
        for (const auto &entry : fs::directory_iterator(images_path)) {
            std:: string filename = entry.path().string();
            if (entry.path().extension() == ".jpg" || entry.path().extension() == ".jpeg") {
                std::cout << "Loading image: " << filename << std::endl;
                cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

                // Vector to store detected corners
                std::vector<cv::Point2f> detected_corners;

                bool found = cv::findChessboardCorners(image, boardSize, detected_corners,
                    cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

                if (found) {
                    objectPoints.push_back(coordinates3d);
                    imagePoints.push_back(detected_corners);

                    cv::drawChessboardCorners(image, boardSize, detected_corners, found);
                    cv::imshow("Corners", image);
                    cv::waitKey(500);
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cout << "Filesystem Error: " << e.what() << std::endl;
    }
    std::cout << "Done finding corners...\n";
    cv::destroyAllWindows();
    // ---------------------------
    // Step 4: Run calibration
    // ---------------------------
    // Use cv::calibrateCamera(objectPoints, imagePoints, imageSize, ...)
    cv::Mat cameraMat, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;

    cv::calibrateCamera(objectPoints, imagePoints, boardSize,
        cameraMat, distCoeffs, rvecs, tvecs);

    // ---------------------------
    // Step 5: Save calibration results
    // ---------------------------
    // Store camera matrix and distortion coefficients to a file
    cv::FileStorage fs("calibration_result.xml", cv::FileStorage::WRITE);
    fs << "CameraMatrix" << cameraMat;
    fs << "DistortionCoeffs" << distCoeffs;
    fs.release();

    std::cout << "Calibration complete. Results saved to calibration_results.xml" << std::endl;


    // ---------------------------
    // Step 6: (Optional) Undistort test image
    // ---------------------------

    return 0;
}
