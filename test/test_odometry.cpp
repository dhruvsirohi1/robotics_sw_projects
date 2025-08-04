//
// Created by Dhruv Sirohi on 8/4/25.
//
#include <gtest/gtest.h>
#include "robot_localization/odometry.h"
#include <chrono>
#include <thread>

using namespace robot_localization;

// Small delay to allow background thread to consume data
void wait_for_processing() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
TEST(OdometryTest, StraightLineMovement) {
    RobotOdometry odom(0.5); // 0.5 meter wheel base
    for (int i = 0; i < 10; ++i) {
        odom.pushEncoderData(0.1, 0.1); // Move straight
    }
    wait_for_processing();

    Pose2d pose = odom.getPose();
    EXPECT_NEAR(pose.x_, 1.0, 1e-2);
    EXPECT_NEAR(pose.y_, 0.0, 1e-2);
    EXPECT_NEAR(pose.theta_, 0.0, 1e-2);
}

TEST(OdometryTest, InPlaceRotation) {
    RobotOdometry odom(0.5);
    for (int i = 0; i < 10; ++i) {
        odom.pushEncoderData(-0.05, 0.05); // Rotate in place
    }
    wait_for_processing();

    Pose2d pose = odom.getPose();
    EXPECT_NEAR(pose.x_, 0.0, 1e-2);
    EXPECT_NEAR(pose.y_, 0.0, 1e-2);
    EXPECT_NEAR(pose.theta_, 2.0, 1e-1);
}

TEST(OdometryTest, MixedMovement) {
    RobotOdometry odom(0.5);
    odom.pushEncoderData(0.1, 0.1);  // forward
    odom.pushEncoderData(0.05, 0.1); // slight right turn
    wait_for_processing();

    Pose2d pose = odom.getPose();
    EXPECT_GT(pose.x_, 0.1);
    EXPECT_GT(pose.y_, 0.0);
    EXPECT_GT(pose.theta_, 0.0);
}