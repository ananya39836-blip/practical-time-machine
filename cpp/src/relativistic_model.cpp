#ifndef PTM_RELATIVISTIC_MODEL_CPP
#define PTM_RELATIVISTIC_MODEL_CPP

#include "constants.cpp"
#include <cmath>

namespace ptm {

class RelativisticProperTimeModel {
public:
    double delta_h;      // Vertical baseline (meters)
    double latitude_rad; // Latitude (radians)

    explicit RelativisticProperTimeModel(double elevation_diff_m = DEFAULT_BASELINE_H, double latitude_deg = DEFAULT_LATITUDE_DEG);

    // Fractional frequency shift: Delta f / f = Delta Phi / c^2
    double gravitational_fractional_shift(double dh = -1.0) const;

    // Second-order geopotential shift accounting for vertical gravity gradient
    double rigorous_geopotential_shift(double dh = -1.0) const;

    // Total accumulated proper time dilation in seconds over duration_seconds
    double accumulated_time_dilation_seconds(double duration_seconds, double dh = -1.0) const;

    // Special relativistic kinematic time dilation factor (Lorentz gamma - 1)
    static double kinematic_dilation_factor(double velocity_m_s);

    // Lorentz factor gamma
    static double lorentz_factor(double velocity_m_s);

    // Relativistic Sagnac phase shift in seconds for an enclosed baseline area
    double sagnac_phase_shift(double baseline_area_m2) const;

    // Terrestrial Shapiro delay in seconds over vertical path
    double terrestrial_shapiro_delay(double dh = -1.0, double ppn_gamma = 1.0) const;

    // Solar limb Shapiro delay in seconds for interplanetary paths
    static double solar_shapiro_delay(double impact_param_m, double r_source_m, double r_receiver_m, double ppn_gamma = 1.0);
};

} // namespace ptm

// ==================== IMPLEMENTATION ====================
namespace ptm {

RelativisticProperTimeModel::RelativisticProperTimeModel(double elevation_diff_m, double latitude_deg)
    : delta_h(elevation_diff_m), latitude_rad(latitude_deg * PI / 180.0) {}

double RelativisticProperTimeModel::gravitational_fractional_shift(double dh) const {
    double h = (dh >= 0.0) ? dh : delta_h;
    return (G_SURFACE * h) / (C_LIGHT * C_LIGHT);
}

double RelativisticProperTimeModel::rigorous_geopotential_shift(double dh) const {
    double h = (dh >= 0.0) ? dh : delta_h;
    // Delta Phi = g0 * h - (g0 / R_E) * h^2
    double delta_phi = G_SURFACE * h - (G_SURFACE / R_EARTH) * (h * h);
    return delta_phi / (C_LIGHT * C_LIGHT);
}

double RelativisticProperTimeModel::accumulated_time_dilation_seconds(double duration_seconds, double dh) const {
    double frac = rigorous_geopotential_shift(dh);
    return frac * duration_seconds;
}

double RelativisticProperTimeModel::kinematic_dilation_factor(double velocity_m_s) {
    double gamma = lorentz_factor(velocity_m_s);
    return gamma - 1.0;
}

double RelativisticProperTimeModel::lorentz_factor(double velocity_m_s) {
    double beta = velocity_m_s / C_LIGHT;
    if (std::abs(beta) >= 1.0) {
        return 1e12; // Numerical safety cap
    }
    return 1.0 / std::sqrt(1.0 - beta * beta);
}

double RelativisticProperTimeModel::sagnac_phase_shift(double baseline_area_m2) const {
    // Delta t_sagnac = 4 * Omega * A * sin(lat) / c^2
    return (4.0 * OMEGA_EARTH * baseline_area_m2 * std::sin(latitude_rad)) / (C_LIGHT * C_LIGHT);
}

double RelativisticProperTimeModel::terrestrial_shapiro_delay(double dh, double ppn_gamma) const {
    double h = (dh >= 0.0) ? dh : delta_h;
    // Delta t_shapiro = (1 + gamma) / c^3 * integral(Phi dz)
    double delta_phi = G_SURFACE * h;
    return ((1.0 + ppn_gamma) / (C_LIGHT * C_LIGHT * C_LIGHT)) * delta_phi;
}

double RelativisticProperTimeModel::solar_shapiro_delay(double impact_param_m, double r_source_m, double r_receiver_m, double ppn_gamma) {
    if (impact_param_m <= 0.0) return 0.0;
    // Standard logarithmic solar Shapiro delay:
    // Delta t = (1 + gamma) * (G * M_sun / c^3) * ln( (r_s + x_s) * (r_r + x_r) / d^2 )
    double term1 = ((1.0 + ppn_gamma) * G_GRAV * M_SUN) / (C_LIGHT * C_LIGHT * C_LIGHT);
    double arg = (4.0 * r_source_m * r_receiver_m) / (impact_param_m * impact_param_m);
    if (arg <= 1.0) arg = 1.0001;
    return term1 * std::log(arg);
}

} // namespace ptm

#endif // PTM_RELATIVISTIC_MODEL_CPP
