#ifndef PTM_RELATIVISTIC_MODEL_HPP
#define PTM_RELATIVISTIC_MODEL_HPP

#include "constants.hpp"
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

#endif // PTM_RELATIVISTIC_MODEL_HPP
