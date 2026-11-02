#include "ptm/kalman_filter.hpp"

namespace ptm {

RelativisticKalmanFilter::RelativisticKalmanFilter(double time_step_sec, double q_wfm, double q_rwfm)
    : dt(time_step_sec), F(3, 3, 0.0), H(1, 3, 0.0), Q(3, 3, 0.0), R(1, 1, 0.0), x(3, 1, 0.0), P(3, 3, 0.0) {
    
    // F matrix
    F(0, 0) = 1.0; F(0, 1) = dt;  F(0, 2) = 0.5 * dt * dt;
    F(1, 0) = 0.0; F(1, 1) = 1.0; F(1, 2) = dt;
    F(2, 0) = 0.0; F(2, 1) = 0.0; F(2, 2) = 1.0;

    // H matrix (measuring phase)
    H(0, 0) = 1.0; H(0, 1) = 0.0; H(0, 2) = 0.0;

    // Process noise covariance Q
    Q(0, 0) = q_wfm * dt + q_rwfm * dt * dt * dt / 3.0;
    Q(0, 1) = q_rwfm * dt * dt / 2.0;
    Q(1, 0) = Q(0, 1);
    Q(1, 1) = q_rwfm * dt;
    Q(2, 2) = 1e-32; // Drift aging variance

    // Measurement noise R: TIC single-shot resolution (20 ps)^2
    R(0, 0) = 4.0e-22;

    reset();
}

void RelativisticKalmanFilter::reset() {
    x(0, 0) = 0.0;
    x(1, 0) = 0.0;
    x(2, 0) = 0.0;

    P(0, 0) = 1e-18;
    P(1, 1) = 1e-20;
    P(2, 2) = 1e-24;
}

void RelativisticKalmanFilter::update(double measurement_phase_sec, double& estimated_phase, double& estimated_freq) {
    // Predict: x_pred = F * x
    Matrix x_pred = F * x;

    // P_pred = F * P * F^T + Q
    Matrix P_pred = F * P * F.transpose() + Q;

    // Innovation: y_innov = z - H * x_pred
    double z = measurement_phase_sec;
    double pred_meas = H(0, 0) * x_pred(0, 0) + H(0, 1) * x_pred(1, 0) + H(0, 2) * x_pred(2, 0);
    double y_innov = z - pred_meas;

    // Innovation covariance: S = H * P_pred * H^T + R
    Matrix S_mat = H * P_pred * H.transpose() + R;
    double S_val = S_mat(0, 0);

    // Kalman gain: K = P_pred * H^T * (1 / S)
    Matrix H_T = H.transpose(); // 3x1
    Matrix K(3, 1);
    for (size_t i = 0; i < 3; ++i) {
        double p_ht = P_pred(i, 0) * H_T(0, 0) + P_pred(i, 1) * H_T(1, 0) + P_pred(i, 2) * H_T(2, 0);
        K(i, 0) = p_ht / S_val;
    }

    // State update: x = x_pred + K * y_innov
    for (size_t i = 0; i < 3; ++i) {
        x(i, 0) = x_pred(i, 0) + K(i, 0) * y_innov;
    }

    // Covariance update: P = (I - K * H) * P_pred
    Matrix I = Matrix::identity(3);
    Matrix KH = K * H; // 3x3
    P = (I - KH) * P_pred;

    estimated_phase = x(0, 0);
    estimated_freq = x(1, 0);
}

double RelativisticKalmanFilter::get_estimated_drift() const {
    return x(2, 0);
}

} // namespace ptm
