#ifndef PTM_CONSTANTS_HPP
#define PTM_CONSTANTS_HPP

namespace ptm {

// Fundamental Physical Constants (CODATA 2022 / IERS Conventions)
constexpr double C_LIGHT = 299792458.0;              // Speed of light in vacuum (m/s)
constexpr double G_GRAV = 6.67430e-11;               // Newtonian gravitational constant (m^3 / kg / s^2)
constexpr double M_EARTH = 5.9722e24;                // Mass of Earth (kg)
constexpr double R_EARTH = 6378137.0;                // WGS84 Equatorial radius (m)
constexpr double OMEGA_EARTH = 7.2921150e-5;         // Earth rotation angular velocity (rad/s)
constexpr double G_SURFACE = 9.80665;                // Standard gravity acceleration (m/s^2)
constexpr double M_SUN = 1.98847e30;                 // Mass of Sun (kg)
constexpr double AU_DISTANCE = 1.495978707e11;       // Astronomical Unit (m)
constexpr double PI = 3.14159265358979323846;

// Experimental Baseline Parameters
constexpr double DEFAULT_BASELINE_H = 100.0000;      // Vertical baseline elevation difference (m)
constexpr double DEFAULT_LATITUDE_DEG = 45.0;        // Observatory geodetic latitude (degrees)
constexpr double THEORETICAL_GRADIENT = G_SURFACE / (C_LIGHT * C_LIGHT); // ~ 1.09115e-16 s/s per meter

} // namespace ptm

#endif // PTM_CONSTANTS_HPP
