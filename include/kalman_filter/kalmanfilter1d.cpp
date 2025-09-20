//
// Created by Dhruv Sirohi on 9/13/25.
//
// Simple 1D Kalman Filter Example
// Tracks position given noisy measurements
// Author: Dhruv Sirohi (Practice Script)

#include <iostream>
#include <vector>
#include <random>

// Kalman Filter for 1D position
class KalmanFilter {
public:
    KalmanFilter(double process_variance, double measurement_variance, double initial_estimate, double initial_error)
        : Q(process_variance), R(measurement_variance), x(initial_estimate), P(initial_error) {}

    // Prediction step (for constant position model, nothing changes)
    void predict() {
        P = P + Q;  // Increase uncertainty due to process noise
    }

    // Update step
    void update(double z) {
        // Compute Kalman Gain
        double K = P / (P + R);

        // Update estimate
        x = x + K * (z - x);

        // Update error covariance
        P = (1 - K) * P;
    }

    double getEstimate() const { return x; }

private:
    double Q;  // Process variance
    double R;  // Measurement variance
    double x;  // Current estimate
    double P;  // Estimation error covariance
};

int main() {
    // Simulation parameters
    double true_position = 50.0;   // Actual constant position
    double process_variance = 0.01; // Small process noise
    double measurement_variance = 4.0; // Large measurement noise
    double initial_guess = 0.0;
    double initial_error = 1.0;

    KalmanFilter kf(process_variance, measurement_variance, initial_guess, initial_error);

    // Random noise generator for measurements
    std::default_random_engine generator;
    std::normal_distribution<double> noise(0.0, 2.0); // stddev ~ sqrt(measurement_variance)

    for (int i = 0; i < 20; ++i) {
        // Simulated noisy measurement
        double measurement = true_position + noise(generator);

        // Kalman filter steps
        kf.predict();
        kf.update(measurement);

        std::cout << "Step " << i + 1
                  << " | Measurement: " << measurement
                  << " | Estimate: " << kf.getEstimate()
                  << std::endl;
    }

    return 0;
}
