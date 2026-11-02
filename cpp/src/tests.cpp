#include "ptm/constants.hpp"
#include "ptm/types.hpp"
#include "ptm/math_utils.hpp"
#include "ptm/relativistic_model.hpp"
#include "ptm/clock_noise.hpp"
#include "ptm/kalman_filter.hpp"
#include "ptm/lidar_geodesy.hpp"
#include "ptm/computer_vision.hpp"
#include "ptm/spacetime_digital_twin.hpp"
#include "ptm/blinded_audit.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace ptm;

void test_matrix_math() {
    std::cout << "[TEST] Running Matrix Math & Inversion Tests...";
    Matrix A(2, 2);
    A(0, 0) = 4.0; A(0, 1) = 7.0;
    A(1, 0) = 2.0; A(1, 1) = 6.0;

    Matrix A_inv = A.inverse();
    Matrix I = A * A_inv;

    assert(std::abs(I(0, 0) - 1.0) < 1e-10);
    assert(std::abs(I(0, 1) - 0.0) < 1e-10);
    assert(std::abs(I(1, 0) - 0.0) < 1e-10);
    assert(std::abs(I(1, 1) - 1.0) < 1e-10);
    std::cout << " PASSED\n";
}

void test_relativistic_equations() {
    std::cout << "[TEST] Running Relativistic Proper Time Equations Tests...";
    RelativisticProperTimeModel model(100.0);
    double shift = model.gravitational_fractional_shift();
    // Theoretical expectation: 9.80665 * 100 / (299792458^2) = 1.09115e-14
    assert(std::abs(shift - 1.09115e-14) < 1e-17);

    double adv_72h = model.accumulated_time_dilation_seconds(72.0 * 3600.0);
    assert(std::abs(adv_72h * 1e9 - 2.8282) < 0.01); // 2.828 ns

    double gamma = RelativisticProperTimeModel::lorentz_factor(1000.0); // 1 km/s
    assert(gamma > 1.0 && gamma < 1.0000001);
    std::cout << " PASSED\n";
}

void test_allan_deviation() {
    std::cout << "[TEST] Running Clock Noise & Allan Deviation Tests...";
    ClockNoiseSimulator sim(1.0, 1000.0);
    std::vector<double> y, x;
    sim.generate_rubidium_noise(y, x, 2e-21, 1e-23, 1e-27, 42);

    assert(x.size() == 1000);
    AllanDeviationResult res = compute_overlapping_allan_deviation(x, 1.0, 10);
    assert(!res.taus.empty());
    assert(res.adev[0] > 0.0);
    std::cout << " PASSED\n";
}

void test_kalman_filter() {
    std::cout << "[TEST] Running Relativistic Kalman Filter Tests...";
    RelativisticKalmanFilter kf(1.0);
    double true_shift = 1.09115e-14;
    double p = 0.0, f = 0.0;

    for (int i = 0; i < 500; ++i) {
        kf.update(true_shift * i, p, f);
    }

    assert(std::abs(f - true_shift) < 1e-15);
    std::cout << " PASSED\n";
}

void test_lidar_geodesy() {
    std::cout << "[TEST] Running LiDAR Geodesy & Refraction Tests...";
    LidarGeodesyEngine engine(100.0);
    std::vector<Vec3> pts;
    LidarScanResult res = engine.generate_point_cloud(pts, 1000);

    assert(res.total_points == 1000);
    assert(std::abs(res.measured_elevation_m - 100.0) < 0.5);
    assert(res.elevation_uncertainty_mm > 0.0);

    double n_air = LidarGeodesyEngine::edlen_refractive_index(20.0, 1013.25, 50.0);
    assert(n_air > 1.0002 && n_air < 1.0003);
    std::cout << " PASSED\n";
}

void test_computer_vision() {
    std::cout << "[TEST] Running Computer Vision & Fringe Extraction Tests...";
    ComputerVisionEngine cv(200.0);
    VisionTrackingResult res = cv.run_tracking_simulation(100);

    assert(res.rms_jitter_nm > 0.0 && res.rms_jitter_nm < 30.0);
    assert(res.fringe_contrast > 0.3 && res.fringe_contrast < 0.7);
    assert(res.optical_alignment_index >= 0.90);
    std::cout << " PASSED\n";
}

void test_spacetime_raytracing() {
    std::cout << "[TEST] Running 3D Spacetime Ray-Tracing Tests...";
    SpacetimeDigitalTwinEngine twin(100.0);
    GeodesicRayResult res = twin.trace_null_geodesic(Vec3(0,0,0), Vec3(0,0,100), 50);

    assert(res.trajectory.size() == 50);
    assert(res.g00_field.front() < 0.0); // Metric signature (-,+,+,+)
    assert(res.riemann_curvature_scalar < 0.0); // Earth attractive gravity
    std::cout << " PASSED\n";
}

void test_blinded_audit() {
    std::cout << "[TEST] Running REQ-SYS-020 Blinded Audit Tests...";
    BlindedAuditResult res = BlindedAuditEngine::run_blinded_injection_test(47.825, 0.10);
    assert(res.passed);
    assert(res.recovery_error_ps < 0.10);
    std::cout << " PASSED\n";
}

int main() {
    std::cout << "\n=======================================================\n";
    std::cout << "   RUNNING PRACTICAL TIME MACHINE C++ TEST SUITE\n";
    std::cout << "=======================================================\n";

    test_matrix_math();
    test_relativistic_equations();
    test_allan_deviation();
    test_kalman_filter();
    test_lidar_geodesy();
    test_computer_vision();
    test_spacetime_raytracing();
    test_blinded_audit();

    std::cout << "\n=======================================================\n";
    std::cout << "   ALL C++ UNIT TESTS PASSED WITH 100% SUCCESS RATE\n";
    std::cout << "=======================================================\n\n";
    return 0;
}
