#ifndef PTM_DATA_VALIDATOR_HPP
#define PTM_DATA_VALIDATOR_HPP

#include "types.hpp"
#include <string>
#include <vector>

namespace ptm {

class ExperimentalDataValidator {
public:
    std::string csv_path;
    double baseline_h;
    double expected_shift;

    explicit ExperimentalDataValidator(const std::string& filename = "simulated_experiment_72h.csv",
                                       double elevation_m = 100.0);

    // Loads observation data from CSV file
    bool load_csv_data(std::vector<double>& timestamps, std::vector<double>& raw_phases,
                       std::vector<double>& kalman_phases);

    // Applies Median Absolute Deviation (MAD) spike filter (threshold = 4.5)
    std::vector<bool> apply_mad_filter(const std::vector<double>& raw_phases,
                                      int& outliers_count, double& completeness_pct,
                                      double threshold = 4.5) const;

    // Estimates relativistic shift using harmonic diurnal de-trending & OLS/GLS
    ValidationReport estimate_relativistic_shift(const std::vector<double>& timestamps,
                                                const std::vector<double>& raw_phases,
                                                const std::vector<double>& kalman_phases,
                                                const std::vector<bool>& valid_mask) const;

    // Validates synthetic or empirical dataset and outputs validation summary
    ValidationReport validate_experiment();
};

} // namespace ptm

#endif // PTM_DATA_VALIDATOR_HPP
