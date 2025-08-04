//
// Created by Dhruv Sirohi on 8/4/25.
//

#ifndef ODOMETRY_H
#define ODOMETRY_H

#include "Pose2d.h"

class RobotOdometry {
public:
    explicit RobotOdometry(double wheel_base);

    void update(double delta_s_left, double delta_s_right);
    [[nodiscard]] Pose2d getPose() const;

private:
    double wheel_base_;
    Pose2d pose_;
};
#endif //ODOMETRY_H
