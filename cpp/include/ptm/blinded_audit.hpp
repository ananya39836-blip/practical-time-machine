#ifndef PTM_BLINDED_AUDIT_HPP
#define PTM_BLINDED_AUDIT_HPP

#include "types.hpp"
#include <vector>
#include <string>

namespace ptm {

struct RealWorldDatasetBenchmark {
    std::string experiment_id;
    std::string campaign_name;
    double baseline_m = 0.0;
    std::string clock_technology;
    double theoretical_shift = 0.0;
    double measured_shift = 0.0;
    double uncertainty = 0.0;
    double agreement_sigma = 0.0;
    bool validated = false;
};

class BlindedAuditEngine {
public:
    // Executes REQ-SYS-020 Cryptographic Blinded Injection & Recovery Audit
    static BlindedAuditResult run_blinded_injection_test(double secret_phase_ps = 47.825,
                                                         double acceptance_threshold_ps = 0.10);

    // Ingests and benchmarks against certified real-world observational datasets
    static std::vector<RealWorldDatasetBenchmark> get_real_world_benchmarks();
};

} // namespace ptm

#endif // PTM_BLINDED_AUDIT_HPP
