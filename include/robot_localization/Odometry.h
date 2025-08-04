//
// Created by Dhruv Sirohi on 8/4/25.
//

#ifndef ODOMETRY_H
#define ODOMETRY_H

#include "Pose2d.h"
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

namespace robot_localization {
    class RobotOdometry {
    public:
        explicit RobotOdometry(double wheel_base);
        ~RobotOdometry();

        void pushEncoderData(double delta_s_left, double delta_s_right);
        [[nodiscard]] Pose2d getPose() const;

    private:
        void updateThreadLoop();

        // Robot state
        double wheel_base_;
        mutable std::mutex pose_mutex_;
        Pose2d pose_;

        std::thread thread_;
        std::atomic<bool> running_;

        std::queue<std::pair<double, double>> queue_;
        std::mutex queue_mutex_;
        std::condition_variable queue_cv_;
    };
} // namespace robot_localization
#endif //ODOMETRY_H
