//
// Created by Dhruv Sirohi on 8/4/25.
//

#include "robot_localization/Odometry.h"

namespace robot_localization {

    RobotOdometry::RobotOdometry(double wheel_base) :
    wheel_base_(wheel_base), running_(true), pose_() {
        thread_= std::thread(&RobotOdometry::updateThreadLoop, this);
    }

    RobotOdometry::~RobotOdometry() {
        running_ = false;
        queue_cv_.notify_all();
        if (thread_.joinable()) {
            thread_.join();
        }
    }

}