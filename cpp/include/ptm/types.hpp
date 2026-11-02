#ifndef PTM_TYPES_HPP
#define PTM_TYPES_HPP

#include <vector>
#include <string>
#include <cstdint>

namespace ptm {

// 3D Vector representation
struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vec3() = default;
    Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

// Single observation sample from Data Acquisition
struct ObservationSample {
    std::string utc_timestamp;
    double epoch_seconds = 0.0;
    double raw_phase_sec = 0.0;
    double temp_core_station_a = 0.0;
    double temp_core_station_b = 0.0;
    double temp_ambient = 0.0;
    double barometric_press_hpa = 0.0;
    double mag_field_total_ut = 0.0;
    uint8_t lock_status = 0;
    int integrity_flag = 0;
};

// Allan Deviation Result
struct AllanDeviationResult {
    std::vector<double> taus;
    std::vector<double> adev;
    std::vector<double> errors;
};

// LiDAR Point Cloud Metadata and Result
struct LidarScanResult {
    int total_points = 0;
    int tower_points = 0;
    int terrain_points = 0;
    double measured_elevation_m = 0.0;
    double elevation_uncertainty_mm = 0.0;
    double delta_phi_potential = 0.0;
    double theoretical_fractional_shift = 0.0;
    bool status = false;
};

// Computer Vision Tracking Result
struct VisionTrackingResult {
    std::vector<double> time_s;
    std::vector<double> jitter_x_nm;
    std::vector<double> jitter_y_nm;
    double rms_jitter_nm = 0.0;
    double fringe_contrast = 0.0;
    double fringe_phase_rad = 0.0;
    double closed_loop_rate_hz = 0.0;
    double optical_alignment_index = 0.0;
    bool status = false;
};

// 3D Spacetime Geodesic Result
struct GeodesicRayResult {
    std::vector<Vec3> trajectory;
    std::vector<double> g00_field;
    double shapiro_delay_ps = 0.0;
    double riemann_curvature_scalar = 0.0;
    double proper_time_dilation_s = 0.0;
    bool status = false;
};

// Validation & Statistical Report
struct ValidationReport {
    int total_records = 0;
    int valid_records = 0;
    double completeness_pct = 0.0;
    int outliers_flagged = 0;
    double measured_shift = 0.0;
    double expected_shift = 0.0;
    double combined_uncertainty_uc = 0.0;
    double signal_to_noise_ratio = 0.0;
    double z_score = 0.0;
    double durbin_watson_d = 0.0;
    bool status = false;
};

// Blinded Audit Verification Result
struct BlindedAuditResult {
    double secret_injected_phase_ps = 0.0;
    double recovered_phase_ps = 0.0;
    double recovery_error_ps = 0.0;
    double secret_fractional_shift = 0.0;
    double recovered_fractional_shift = 0.0;
    double acceptance_threshold_ps = 0.0;
    bool passed = false;
};

} // namespace ptm

#endif // PTM_TYPES_HPP
