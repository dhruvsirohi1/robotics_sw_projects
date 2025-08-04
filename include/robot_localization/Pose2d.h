//
// Created by Dhruv Sirohi on 8/4/25.
//

#ifndef POSE2D_H
#define POSE2D_H

struct Pose2d {
    double x_;
    double y_;
    double theta_;

    explicit Pose2d(double x = 0.0, double y = 0.0, double theta = 0.0) :
    x_(x), y_(y), theta_(theta) {}
};

#endif //POSE2D_H
