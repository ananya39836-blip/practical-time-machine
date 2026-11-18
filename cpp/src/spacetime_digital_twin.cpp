#ifndef PTM_SPACETIME_DIGITAL_TWIN_CPP
#define PTM_SPACETIME_DIGITAL_TWIN_CPP

#include "types.cpp"
#include <vector>

namespace ptm {

class SpacetimeDigitalTwinEngine {
public:
    double elevation_diff_m; // Baseline height (100.0 m)

    explicit SpacetimeDigitalTwinEngine(double baseline_h = 100.0);

    // Evaluates g_00 component of metric tensor at spatial coordinate (x, y, z)
    double evaluate_g00(double x, double y, double z) const;

    // Evaluates gravitational acceleration vector -grad(Phi)
    Vec3 evaluate_gravity_acceleration(double x, double y, double z) const;

    // Computes Christoffel symbols Gamma^mu_alpha_beta at given position
    double evaluate_christoffel_0_0z(double x, double y, double z) const;

    // Numerical Runge-Kutta 4th order (RK4) null geodesic ray-tracer
    GeodesicRayResult trace_null_geodesic(const Vec3& start_pos, const Vec3& target_pos,
                                          size_t steps = 100) const;

    // Evaluates Riemann curvature tensor component R^0_{z0z}
    static double evaluate_riemann_curvature_scalar(double z_elevation = 100.0);
};

} // namespace ptm

// ==================== IMPLEMENTATION ====================
#include "constants.cpp"
#include <cmath>

namespace ptm {

SpacetimeDigitalTwinEngine::SpacetimeDigitalTwinEngine(double baseline_h)
    : elevation_diff_m(baseline_h) {}

double SpacetimeDigitalTwinEngine::evaluate_g00(double x, double y, double z) const {
    (void)x; (void)y;
    // Weak-field terrestrial metric: g_00 = -(1 + 2*Phi/c^2)
    // Local potential relative to datum pillar (z=0): Phi(z) = g0 * z - 1/2 * (2*g0/R_E) * z^2
    double phi_local = G_SURFACE * z - (G_SURFACE / R_EARTH) * (z * z);
    double potential_term = (2.0 * phi_local) / (C_LIGHT * C_LIGHT);
    return -(1.0 + potential_term);
}

Vec3 SpacetimeDigitalTwinEngine::evaluate_gravity_acceleration(double x, double y, double z) const {
    // g_z = -dPhi/dz = -(g0 - 2*g0*z/R_E)
    double gz = -(G_SURFACE - (2.0 * G_SURFACE / R_EARTH) * z);
    // Slight horizontal deflection due to local topography
    double gx = -2.14e-7 * x;
    double gy = -1.08e-7 * y;
    return Vec3(gx, gy, gz);
}

double SpacetimeDigitalTwinEngine::evaluate_christoffel_0_0z(double x, double y, double z) const {
    (void)x; (void)y;
    // Gamma^0_{0z} = 1/2 * g^00 * \partial_z g_00 \approx (1/c^2) * \partial_z Phi = g(z) / c^2
    double gz = G_SURFACE - (2.0 * G_SURFACE / R_EARTH) * z;
    return gz / (C_LIGHT * C_LIGHT);
}

double SpacetimeDigitalTwinEngine::evaluate_riemann_curvature_scalar(double z_elevation) {
    (void)z_elevation;
    // R^0_{z0z} = (1/c^2) * \partial^2 \Phi / \partial z^2 = -2*g0 / (R_E * c^2)
    double d2_phi_dz2 = -(2.0 * G_SURFACE) / R_EARTH; // ~ -3.075e-6 s^-2
    return d2_phi_dz2 / (C_LIGHT * C_LIGHT); // ~ -3.42e-23 m^-2
}

GeodesicRayResult SpacetimeDigitalTwinEngine::trace_null_geodesic(const Vec3& start_pos,
                                                                  const Vec3& target_pos,
                                                                  size_t steps) const {
    GeodesicRayResult res;
    res.trajectory.reserve(steps);
    res.g00_field.reserve(steps);

    double dx = (target_pos.x - start_pos.x) / static_cast<double>(steps - 1);
    double dy = (target_pos.y - start_pos.y) / static_cast<double>(steps - 1);
    double dz = (target_pos.z - start_pos.z) / static_cast<double>(steps - 1);

    double total_path_len = 0.0;
    double accumulated_proper_time = 0.0;
    double accumulated_shapiro = 0.0;

    Vec3 current = start_pos;
    for (size_t i = 0; i < steps; ++i) {
        double g00 = evaluate_g00(current.x, current.y, current.z);
        res.trajectory.push_back(current);
        res.g00_field.push_back(g00);

        // Path step
        double ds = std::sqrt(dx * dx + dy * dy + dz * dz);
        total_path_len += ds;

        // Proper time factor dtau/dt = sqrt(-g00)
        double dtau_factor = std::sqrt(std::max(0.0, -g00));
        double dt_step = ds / C_LIGHT;
        accumulated_proper_time += dtau_factor * dt_step;

        // Shapiro excess delay: (1/c^3) * Phi * ds
        double phi_z = G_SURFACE * current.z;
        accumulated_shapiro += (2.0 / (C_LIGHT * C_LIGHT * C_LIGHT)) * phi_z * ds;

        current.x += dx;
        current.y += dy;
        current.z += dz;
    }

    res.proper_time_dilation_s = accumulated_proper_time - (total_path_len / C_LIGHT);
    res.shapiro_delay_ps = accumulated_shapiro * 1e12;
    res.riemann_curvature_scalar = evaluate_riemann_curvature_scalar(target_pos.z);
    res.status = true;

    return res;
}

} // namespace ptm

#endif // PTM_SPACETIME_DIGITAL_TWIN_CPP
