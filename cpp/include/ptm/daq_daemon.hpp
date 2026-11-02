#ifndef PTM_DAQ_DAEMON_HPP
#define PTM_DAQ_DAEMON_HPP

#include "types.hpp"
#include <string>
#include <vector>

namespace ptm {

enum class FSMState {
    STANDBY = 0,
    CALIBRATE = 1,
    ACQUIRE = 2,
    ANOMALY = 3,
    SAFE = 4
};

class TimeMachineDAQDaemon {
public:
    double elevation_m;
    double target_shift;
    std::string csv_path;
    double accumulated_phase;
    size_t sample_count;
    FSMState current_state;

    explicit TimeMachineDAQDaemon(double elevation = 100.0, const std::string& output_filename = "PTM_OBS_live.csv");

    // Initialize CSV storage with formal headers
    bool init_storage();

    // Acquire single measurement sample
    ObservationSample acquire_single_sample(double elapsed_seconds);

    // Run acquisition loop for N samples
    int run_acquisition_loop(size_t max_samples = 10, double interval_sec = 1.0);

    // State machine transitions
    void transition_to(FSMState next_state);
    std::string get_state_name() const;
};

} // namespace ptm

#endif // PTM_DAQ_DAEMON_HPP
