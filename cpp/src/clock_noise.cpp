#ifndef PTM_CLOCK_NOISE_CPP
#define PTM_CLOCK_NOISE_CPP

#include "types.cpp"
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

// ==================== IMPLEMENTATION ====================
#include "constants.cpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <set>

namespace ptm {

ClockNoiseSimulator::ClockNoiseSimulator(double sample_rate_hz, double duration_seconds)
    : fs(sample_rate_hz), dt(1.0 / sample_rate_hz) {
    n_samples = static_cast<size_t>(duration_seconds * sample_rate_hz);
    if (n_samples < 2) n_samples = 2;
    time_array.resize(n_samples);
    for (size_t i = 0; i < n_samples; ++i) {
        time_array[i] = static_cast<double>(i) * dt;
    }
}

void ClockNoiseSimulator::generate_rubidium_noise(std::vector<double>& y_noise,
                                                 std::vector<double>& x_phase_error,
                                                 double h0, double h_minus1, double h_minus2,
                                                 uint64_t seed) const {
    y_noise.assign(n_samples, 0.0);
    x_phase_error.assign(n_samples, 0.0);

    std::mt19937_64 rng(seed);
    // White frequency noise standard deviation: sigma_y_white = sqrt(h0 / (2 * dt))
    double sigma_wfm = (h0 > 0.0) ? std::sqrt(h0 * fs / 2.0) : 2.0e-11;

    // Random walk frequency noise step
    double sigma_rwfm = (h_minus2 > 0.0) ? std::sqrt(h_minus2 * 2.0 * PI * PI * dt) : 1.0e-15;

    // Flicker frequency noise approximation (multi-scale autoregressive filter)
    double sigma_ffm = (h_minus1 > 0.0) ? std::sqrt(h_minus1 * 2.0 * std::log(2.0)) : 1.2e-12;

    std::normal_distribution<double> dist_wfm(0.0, sigma_wfm);
    std::normal_distribution<double> dist_rwfm(0.0, sigma_rwfm);
    std::normal_distribution<double> dist_ffm(0.0, sigma_ffm);

    double current_rw = 0.0;
    // 3-pole filter for 1/f flicker noise
    double f1 = 0.0, f2 = 0.0, f3 = 0.0;

    for (size_t i = 0; i < n_samples; ++i) {
        double w = dist_wfm(rng);
        current_rw += dist_rwfm(rng);

        // Barnes-Jarvis approximation for flicker noise
        double u = dist_ffm(rng);
        f1 = 0.998 * f1 + 0.002 * u;
        f2 = 0.98 * f2 + 0.02 * u;
        f3 = 0.8 * f3 + 0.2 * u;
        double flicker = 0.333 * (f1 + f2 + f3);

        y_noise[i] = w + current_rw + flicker;
    }

    // Accumulate phase error: x(t) = integral y(t) dt
    double cum_phase = 0.0;
    for (size_t i = 0; i < n_samples; ++i) {
        cum_phase += y_noise[i] * dt;
        x_phase_error[i] = cum_phase;
    }
}

AllanDeviationResult compute_overlapping_allan_deviation(const std::vector<double>& phase_data_sec,
                                                         double sample_interval_sec,
                                                         size_t num_tau_bins) {
    AllanDeviationResult result;
    size_t N = phase_data_sec.size();
    if (N < 4) return result;

    double tau0 = sample_interval_sec;
    size_t max_m = N / 4;
    if (max_m < 1) max_m = 1;

    std::set<size_t> m_set;
    for (size_t i = 0; i < num_tau_bins; ++i) {
        double frac = static_cast<double>(i) / static_cast<double>(num_tau_bins - 1);
        double log_m = frac * std::log10(static_cast<double>(max_m));
        size_t m = static_cast<size_t>(std::round(std::pow(10.0, log_m)));
        if (m >= 1 && m <= max_m) {
            m_set.insert(m);
        }
    }

    for (size_t m : m_set) {
        if (2 * m >= N) continue;

        double sum_sq = 0.0;
        size_t count = N - 2 * m;

        for (size_t i = 0; i < count; ++i) {
            double d2 = phase_data_sec[i + 2 * m] - 2.0 * phase_data_sec[i + m] + phase_data_sec[i];
            sum_sq += d2 * d2;
        }

        double denom = 2.0 * static_cast<double>(m * m) * (tau0 * tau0) * static_cast<double>(count);
        double sigma2 = sum_sq / denom;
        double adev_val = std::sqrt(std::max(0.0, sigma2));

        result.taus.push_back(static_cast<double>(m) * tau0);
        result.adev.push_back(adev_val);
        // Approximate 1-sigma uncertainty: adev / sqrt(N / 2m)
        double edof = static_cast<double>(count) / static_cast<double>(m);
        double err = (edof > 0.0) ? (adev_val / std::sqrt(edof)) : 0.0;
        result.errors.push_back(err);
    }

    return result;
}

} // namespace ptm

#endif // PTM_CLOCK_NOISE_CPP
