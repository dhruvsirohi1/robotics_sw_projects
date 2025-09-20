//
// Created by Dhruv Sirohi on 9/14/25.
//
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0); // open webcam
    if(!cap.isOpened()) {
        std::cerr << "Error: Cannot open webcam\n";
        return -1;
    }

    cv::Mat frame, hsv, mask;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Convert to HSV color space
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // Threshold: detect blue color
        cv::Scalar lower(100, 150, 50);  // HSV lower bound
        cv::Scalar upper(140, 255, 255); // HSV upper bound
        cv::inRange(hsv, lower, upper, mask);

        // Find contours of detected regions
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (auto &contour : contours) {
            if (cv::contourArea(contour) > 500) { // filter noise
                cv::Rect box = cv::boundingRect(contour);
                cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
            }
        }

        // Show results
        cv::imshow("Frame", frame);
        cv::imshow("Mask", mask);

        if (cv::waitKey(30) == 27) break; // ESC to exit
    }

    return 0;
}
