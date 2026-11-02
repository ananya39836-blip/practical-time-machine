#ifndef PTM_CLOCK_NOISE_HPP
#define PTM_CLOCK_NOISE_HPP

#include "types.hpp"
#include <vector>

namespace ptm {

class ClockNoiseSimulator {
public:
    double fs;              // Sampling frequency (Hz)
    size_t n_samples;       // Number of samples
    double dt;              // Time step (s)
    std::vector<double> time_array;

    ClockNoiseSimulator(double sample_rate_hz = 1.0, double duration_seconds = 86400.0);

    // Generates simulated fractional frequency fluctuations y(t) and phase error x(t)
    // h0: White FM, h_minus1: Flicker FM, h_minus2: Random walk FM
    void generate_rubidium_noise(std::vector<double>& y_noise, std::vector<double>& x_phase_error,
                                 double h0 = 2e-21, double h_minus1 = 1e-23, double h_minus2 = 1e-27,
                                 uint64_t seed = 42) const;
};

// Computes standard Overlapping Allan Deviation from phase time series x(t)
AllanDeviationResult compute_overlapping_allan_deviation(const std::vector<double>& phase_data_sec,
                                                         double sample_interval_sec,
                                                         size_t num_tau_bins = 30);

} // namespace ptm

#endif // PTM_CLOCK_NOISE_HPP
