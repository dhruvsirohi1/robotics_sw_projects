//
// Created by Dhruv Sirohi on 9/24/25.
//
// CameraCalibrationComparison.cpp
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

// ---------------------------
// Helper: Compute reprojection error
// ---------------------------
double computeReprojectionError(
    const std::vector<std::vector<cv::Point3f>>& objectPoints,
    const std::vector<std::vector<cv::Point2f>>& imagePoints,
    const cv::Mat& cameraMatrix,
    const cv::Mat& distCoeffs,
    const std::vector<cv::Mat>& rvecs,
    const std::vector<cv::Mat>& tvecs)
{
    double totalError = 0;
    size_t totalPoints = 0;

    for (size_t i = 0; i < objectPoints.size(); i++) {
        std::vector<cv::Point2f> projectedPoints;
        cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i],
                          cameraMatrix, distCoeffs, projectedPoints);

        double err = cv::norm(imagePoints[i], projectedPoints, cv::NORM_L2);
        totalError += err * err;
        totalPoints += objectPoints[i].size();
    }

    return std::sqrt(totalError / totalPoints);
}

// ---------------------------
// Main
// ---------------------------
int main() {
    // Dummy calibration data --------------------
    // Assume a single calibration pattern with 4 points
    std::vector<std::vector<cv::Point3f>> objectPoints(1);
    std::vector<std::vector<cv::Point2f>> imagePoints(1);

    // 3D points (checkerboard corners in world units)
    objectPoints[0] = {
        {0, 0, 0}, {1, 0, 0},
        {0, 1, 0}, {1, 1, 0}
    };

    // Simulated 2D detections for calibration A
    imagePoints[0] = {
        {100, 100}, {200, 100},
        {100, 200}, {200, 200}
    };

    // Camera intrinsics (dummy)
    cv::Mat cameraMatrix = (cv::Mat_<double>(3,3) <<
        800, 0, 320,
        0, 800, 240,
        0, 0, 1
    );

    cv::Mat distCoeffs = cv::Mat::zeros(5,1,CV_64F);

    // Dummy extrinsics
    std::vector<cv::Mat> rvecs(1), tvecs(1);
    rvecs[0] = (cv::Mat_<double>(3,1) << 0, 0, 0);
    tvecs[0] = (cv::Mat_<double>(3,1) << 0, 0, 5);

    // Compute reprojection error
    double error = computeReprojectionError(objectPoints, imagePoints,
                                            cameraMatrix, distCoeffs,
                                            rvecs, tvecs);

    std::cout << "Reprojection error (Calibration A): " << error << " pixels" << std::endl;

    // TODO: Repeat with another set of imagePoints/cameraMatrix for Calibration B
    // and compare errors

    return 0;
}
