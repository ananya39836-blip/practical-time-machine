#include "constants.cpp"
#include "types.cpp"
#include "math_utils.cpp"
#include "relativistic_model.cpp"
#include "clock_noise.cpp"
#include "kalman_filter.cpp"
#include "lidar_geodesy.cpp"
#include "computer_vision.cpp"
#include "spacetime_digital_twin.cpp"
#include "blinded_audit.cpp"
#include "oop_observatory.cpp"
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

void test_oop_architecture() {
    std::cout << "[TEST] Running UML OOP Runtime Polymorphism (2 Virtual Classes) Tests...";
    assert(TimeDilationSensor::getSensorCount() == 0);
    assert(MetricAuditor::getAuditorCount() == 0);
    {
        RubidiumAtomicClock rb(101, "SRS-PRS10", 100.0, 2.0e-12, 25.0);
        OpticalLatticeClock sr(102, "Sr-87 Lattice", 452.6, 813.42, 50.0);
        LidarGeodesyStation lidar(103, "Leica-P50", 100.0, 2500, 0.263);

        assert(TimeDilationSensor::getSensorCount() == 3);

        // Runtime polymorphism via Virtual Class 1 (TimeDilationSensor*)
        TimeDilationSensor* sensorArr[3] = { &rb, &sr, &lidar };
        assert(sensorArr[1]->computeFractionalShift() > 4.9e-14);
        assert(sensorArr[1]->getMeasurementUncertainty() < 1.1e-18);
        assert(sensorArr[2]->getMeasurementUncertainty() < 1.0e-18);

        {
            SpacetimeObservatory obs("UnitTest Observatory");
            for (int i = 0; i < 3; ++i) {
                assert(obs.attachSensor(sensorArr[i]));
            }
            assert(obs.getSensorCount() == 3);

            // Composition: allocates 3 TelemetryLog objects via new
            obs.recordTelemetry(259200.0);
            assert(obs.getLogCount() == 3);
            assert(obs.getLog(1) != nullptr);
            assert(obs.getLog(1)->isVerified());
            assert(obs.getLog(1)->getAdvanceNs() > 12.0);

            // Runtime polymorphism via Virtual Class 2 (MetricAuditor*)
            StandardGumAuditor gum("TEST-GUM-01", &obs, 1.0e-11);
            BlindedCryptoAuditor crypto("TEST-BLIND-02", &obs, 47.825, 0.10);
            assert(MetricAuditor::getAuditorCount() == 2);

            MetricAuditor* auditors[2] = { &gum, &crypto };
            assert(auditors[0]->getAuditProtocol() == "ISO-GUM-NASA-SP-2016");
            assert(auditors[1]->getAuditProtocol() == "REQ-SYS-020-BLINDED");
            assert(auditors[0]->verifySensorCompliance(*sensorArr[1]));
            assert(auditors[1]->verifySensorCompliance(*sensorArr[2]));
        } // ~SpacetimeObservatory() deletes composed logs, preserves aggregated sensors

        assert(TimeDilationSensor::getSensorCount() == 3);
        assert(MetricAuditor::getAuditorCount() == 0);
    } // Stack sensors destroyed -> virtual ~TimeDilationSensor() decrements static count
    assert(TimeDilationSensor::getSensorCount() == 0);
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
    test_oop_architecture();

    std::cout << "\n=======================================================\n";
    std::cout << "   ALL C++ UNIT TESTS PASSED WITH 100% SUCCESS RATE\n";
    std::cout << "=======================================================\n\n";
    return 0;
}
