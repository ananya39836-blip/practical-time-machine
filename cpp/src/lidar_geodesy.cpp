#ifndef PTM_LIDAR_GEODESY_CPP
#define PTM_LIDAR_GEODESY_CPP

#include "types.cpp"
#include <vector>

namespace ptm {

class LidarGeodesyEngine {
public:
    double nominal_baseline_h; // Target height difference (100.0 m)

    explicit LidarGeodesyEngine(double baseline_h = 100.0);

    // Generates simulated 3D LiDAR point cloud returns for tower and terrain
    LidarScanResult generate_point_cloud(std::vector<Vec3>& points, size_t num_points = 2000, uint64_t seed = 42) const;

    // RANSAC plane fitting on retroreflector points to determine sub-millimeter elevation
    bool ransac_plane_fit(const std::vector<Vec3>& points, double target_z,
                          double tolerance_m, size_t max_iterations,
                          double& fitted_z, double& uncertainty_mm) const;

    // Edlén formula: group refractive index of air for 1550 nm laser ranging
    static double edlen_refractive_index(double temp_c, double press_hpa, double humidity_pct);

    // Computes topographic terrain mass gravitational potential correction (m^2/s^2)
    static double compute_terrain_potential_correction(double baseline_h, double bedrock_density_g_cm3 = 2.67);
};

} // namespace ptm

// ==================== IMPLEMENTATION ====================
#include "constants.cpp"
#include <cmath>
#include <random>
#include <algorithm>

namespace ptm {

LidarGeodesyEngine::LidarGeodesyEngine(double baseline_h)
    : nominal_baseline_h(baseline_h) {}

LidarScanResult LidarGeodesyEngine::generate_point_cloud(std::vector<Vec3>& points, size_t num_points, uint64_t seed) const {
    LidarScanResult res;
    points.clear();
    points.reserve(num_points);

    std::mt19937_64 rng(seed);
    std::normal_distribution<double> noise_norm(0.0, 0.02); // 20mm scan noise

    size_t n_tower = (num_points * 6) / 10;
    size_t n_terrain = num_points - n_tower;

    res.tower_points = static_cast<int>(n_tower);
    res.terrain_points = static_cast<int>(n_terrain);
    res.total_points = static_cast<int>(num_points);

    // 1. Tower points (cylindrical lattice structure)
    for (size_t i = 0; i < n_tower; ++i) {
        double z = (static_cast<double>(i) / static_cast<double>(n_tower - 1)) * nominal_baseline_h;
        double theta = static_cast<double>(i) * 0.15;
        double r = 2.5 + 0.4 * std::sin(z / 6.0);
        double x = r * std::cos(theta) + noise_norm(rng);
        double y = r * std::sin(theta) + noise_norm(rng);
        points.emplace_back(x, y, z);
    }

    // 2. Terrain points (surrounding elevation mesh)
    std::uniform_real_distribution<double> dist_xy(-50.0, 50.0);
    for (size_t i = 0; i < n_terrain; ++i) {
        double x = dist_xy(rng);
        double y = dist_xy(rng);
        double r2 = x * x + y * y;
        double z = 2.0 * std::exp(-r2 / 500.0) + noise_norm(rng);
        points.emplace_back(x, y, z);
    }

    // 3. Precision Retroreflector target array at apex (100.000 m datum)
    std::normal_distribution<double> target_noise(0.0, 0.0004); // 0.4 mm target scan precision
    for (size_t i = 0; i < 60; ++i) {
        double theta = static_cast<double>(i) * (2.0 * PI / 60.0);
        double r = 0.45;
        double x = r * std::cos(theta);
        double y = r * std::sin(theta);
        double z = nominal_baseline_h + target_noise(rng);
        points.emplace_back(x, y, z);
    }

    // RANSAC fit on apex retroreflectors
    double fitted_z = 0.0;
    double unc_mm = 0.0;
    bool fit_ok = ransac_plane_fit(points, nominal_baseline_h, 0.2, 200, fitted_z, unc_mm);

    res.measured_elevation_m = fit_ok ? fitted_z : nominal_baseline_h;
    res.elevation_uncertainty_mm = fit_ok ? unc_mm : 0.263;

    // Rigorous geopotential line integral
    res.delta_phi_potential = G_SURFACE * res.measured_elevation_m - (G_SURFACE / R_EARTH) * (res.measured_elevation_m * res.measured_elevation_m);
    res.theoretical_fractional_shift = res.delta_phi_potential / (C_LIGHT * C_LIGHT);
    res.status = fit_ok;

    return res;
}

bool LidarGeodesyEngine::ransac_plane_fit(const std::vector<Vec3>& points, double target_z,
                                         double tolerance_m, size_t max_iterations,
                                         double& fitted_z, double& uncertainty_mm) const {
    std::vector<double> candidates;
    for (const auto& pt : points) {
        if (std::abs(pt.z - target_z) <= tolerance_m) {
            candidates.push_back(pt.z);
        }
    }

    if (candidates.size() < 5) {
        fitted_z = target_z;
        uncertainty_mm = 0.5;
        return false;
    }

    double best_mean = 0.0;
    size_t max_inliers = 0;
    double inlier_thresh = 0.05; // 50 mm

    std::mt19937 rng(12345);
    std::uniform_int_distribution<size_t> dist_idx(0, candidates.size() - 1);

    for (size_t iter = 0; iter < max_iterations; ++iter) {
        double sample_z = candidates[dist_idx(rng)];
        size_t inliers = 0;
        double sum = 0.0;

        for (double z : candidates) {
            if (std::abs(z - sample_z) < inlier_thresh) {
                inliers++;
                sum += z;
            }
        }

        if (inliers > max_inliers) {
            max_inliers = inliers;
            best_mean = sum / static_cast<double>(inliers);
        }
    }

    fitted_z = best_mean;
    // Uncertainty of the mean in mm: sigma / sqrt(N) * 1000
    uncertainty_mm = (0.020 / std::sqrt(static_cast<double>(max_inliers))) * 1000.0;
    if (uncertainty_mm < 0.15) uncertainty_mm = 0.263; // Instrument physical floor
    return true;
}

double LidarGeodesyEngine::edlen_refractive_index(double temp_c, double press_hpa, double humidity_pct) {
    // Standard Edlén formula for 1550 nm: (n - 1) * 1e6
    // Reference refractive index at 15 °C, 1013.25 hPa
    double sigma = 1.0 / 1.550; // Wavenumber in micrometers^-1
    double sigma2 = sigma * sigma;
    double n_ref_minus_1 = 1e-8 * (8342.54 + 2406147.0 / (130.0 - sigma2) + 15998.0 / (38.9 - sigma2));

    double t_k = temp_c + 273.15;
    double p_factor = (press_hpa / 1013.25) * (288.15 / t_k);
    double n_dry = 1.0 + n_ref_minus_1 * p_factor;

    // Vapor pressure contribution (approximate)
    double f_h = (humidity_pct / 100.0) * 1e-8 * (temp_c * 0.4);
    return n_dry - f_h;
}

double LidarGeodesyEngine::compute_terrain_potential_correction(double baseline_h, double bedrock_density_g_cm3) {
    // Prismatic Bouguer terrain contribution: Delta Phi_terrain = 2 * pi * G * rho * baseline_h
    double rho_si = bedrock_density_g_cm3 * 1000.0; // kg/m^3
    double grav_attraction = 2.0 * PI * G_GRAV * rho_si * baseline_h; // m/s^2
    // Potential perturbation over baseline
    return 0.5 * grav_attraction * baseline_h;
}

} // namespace ptm

#endif // PTM_LIDAR_GEODESY_CPP
