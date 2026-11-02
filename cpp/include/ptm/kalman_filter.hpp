#ifndef PTM_KALMAN_FILTER_HPP
#define PTM_KALMAN_FILTER_HPP

#include "math_utils.hpp"
#include <vector>

namespace ptm {

class RelativisticKalmanFilter {
public:
    double dt;
    Matrix F; // 3x3 state transition
    Matrix H; // 1x3 measurement matrix
    Matrix Q; // 3x3 process noise covariance
    Matrix R; // 1x1 measurement noise covariance
    Matrix x; // 3x1 state estimate: [phase (s), freq_offset (s/s), drift (s/s^2)]^T
    Matrix P; // 3x3 estimation error covariance

    explicit RelativisticKalmanFilter(double time_step_sec = 1.0, double q_wfm = 1e-22, double q_rwfm = 1e-28);

    // Runs a prediction and measurement update step, returning estimated phase and fractional frequency
    void update(double measurement_phase_sec, double& estimated_phase, double& estimated_freq);

    // Get current estimated drift rate
    double get_estimated_drift() const;

    // Reset filter state
    void reset();
};

} // namespace ptm

#endif // PTM_KALMAN_FILTER_HPP
