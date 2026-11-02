#ifndef PTM_LIDAR_GEODESY_HPP
#define PTM_LIDAR_GEODESY_HPP

#include "types.hpp"
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

#endif // PTM_LIDAR_GEODESY_HPP
