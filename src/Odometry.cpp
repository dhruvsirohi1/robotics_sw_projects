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

    void RobotOdometry::pushEncoderData(double delta_s_left, double delta_s_right) {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        queue_.emplace(delta_s_left, delta_s_right);
        queue_cv_.notify_one();
    }

    Pose2d RobotOdometry::getPose() const {
        std::lock_guard<std::mutex> lock(pose_mutex_);
        return pose_;
    }

    void RobotOdometry::updateThreadLoop() {
        while (running_) {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            queue_cv_.wait(lock, [&] { return !queue_.empty() || !running_; });

            while (!queue_.empty()) {
                auto [delta_s_left, delta_s_right] = queue_.front();
                queue_.pop();
                lock.unlock();

                double delta_s = (delta_s_left + delta_s_right) / 2.0;
                double delta_theta = (delta_s_right - delta_s_left) / wheel_base_;

                double theta_mid;
                {
                    std::lock_guard<std::mutex> pose_lock(pose_mutex_);
                    theta_mid = pose_.theta_ + delta_theta / 2.0;

                    pose_.x_ += delta_s * std::cos(theta_mid);
                    pose_.y_ += delta_s * std::sin(theta_mid);
                    pose_.theta_ += delta_theta;

                    // Normalize theta to [-pi, pi]
                    if (pose_.theta_ > M_PI)
                        pose_.theta_ -= 2 * M_PI;
                    else if (pose_.theta_ < -M_PI)
                        pose_.theta_ += 2 * M_PI;
                }
                lock.lock();
            }
        }
    }
}