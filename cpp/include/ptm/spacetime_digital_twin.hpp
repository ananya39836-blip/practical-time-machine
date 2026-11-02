#ifndef PTM_SPACETIME_DIGITAL_TWIN_HPP
#define PTM_SPACETIME_DIGITAL_TWIN_HPP

#include "types.hpp"
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

#endif // PTM_SPACETIME_DIGITAL_TWIN_HPP
