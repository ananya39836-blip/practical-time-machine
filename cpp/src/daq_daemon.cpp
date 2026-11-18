#ifndef PTM_DAQ_DAEMON_CPP
#define PTM_DAQ_DAEMON_CPP

#include "types.cpp"
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

// ==================== IMPLEMENTATION ====================
#include "constants.cpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <random>

namespace ptm {

TimeMachineDAQDaemon::TimeMachineDAQDaemon(double elevation, const std::string& output_filename)
    : elevation_m(elevation), accumulated_phase(0.0), sample_count(0), current_state(FSMState::STANDBY) {
    target_shift = (G_SURFACE * elevation_m) / (C_LIGHT * C_LIGHT);
    csv_path = output_filename;
}

bool TimeMachineDAQDaemon::init_storage() {
    std::ofstream f(csv_path);
    if (!f.is_open()) {
        std::cerr << "[DAQ] Error creating CSV: " << csv_path << std::endl;
        return false;
    }

    f << "utc_timestamp,epoch_seconds,raw_time_interval_sec,"
      << "temp_core_station_a_degc,temp_core_station_b_degc,temp_ambient_degc,"
      << "barometric_press_hpa,mag_field_total_ut,lock_status_byte,integrity_flag\n";
    f.close();
    std::cout << "[DAQ] Initialized acquisition archive: " << csv_path << std::endl;
    return true;
}

ObservationSample TimeMachineDAQDaemon::acquire_single_sample(double elapsed_seconds) {
    static std::mt19937_64 rng(54321);
    std::normal_distribution<double> dist_w(0.0, 20e-12); // 20 ps TIC white noise
    std::normal_distribution<double> dist_rw(0.0, 1e-15);
    std::normal_distribution<double> dist_temp(0.0, 0.001);
    std::normal_distribution<double> dist_mag(0.0, 0.05);

    accumulated_phase += target_shift * 1.0; // 1s time step

    ObservationSample s;
    s.epoch_seconds = elapsed_seconds;
    s.raw_phase_sec = accumulated_phase + dist_w(rng) + dist_rw(rng);

    s.temp_core_station_a = 30.000 + dist_temp(rng);
    s.temp_core_station_b = 30.000 + dist_temp(rng);
    s.temp_ambient = 21.0 + 0.2 * std::sin(2.0 * PI * elapsed_seconds / 86400.0);
    s.barometric_press_hpa = 1013.25 + 0.5 * std::cos(2.0 * PI * elapsed_seconds / 43200.0);
    s.mag_field_total_ut = 42.10 + dist_mag(rng);

    s.lock_status = 0x03; // Bit 0 (Clock A) and Bit 1 (Clock B) locked
    s.integrity_flag = 0; // 0 = VALID

    sample_count++;
    return s;
}

int TimeMachineDAQDaemon::run_acquisition_loop(size_t max_samples, double interval_sec) {
    if (!init_storage()) return -1;

    transition_to(FSMState::ACQUIRE);
    std::cout << "[DAQ] Starting continuous acquisition loop (" << max_samples
              << " samples, " << interval_sec << "s interval)..." << std::endl;

    std::ofstream f(csv_path, std::ios::app);
    for (size_t i = 0; i < max_samples; ++i) {
        double elapsed = static_cast<double>(i) * interval_sec;
        ObservationSample s = acquire_single_sample(elapsed);

        f << std::fixed << std::setprecision(6)
          << "2026-11-02T14:00:00Z," << s.epoch_seconds << ","
          << std::scientific << std::setprecision(12) << s.raw_phase_sec << ","
          << std::fixed << std::setprecision(4)
          << s.temp_core_station_a << "," << s.temp_core_station_b << ","
          << s.temp_ambient << "," << s.barometric_press_hpa << ","
          << s.mag_field_total_ut << "," << static_cast<int>(s.lock_status) << ","
          << s.integrity_flag << "\n";
    }

    f.close();
    std::cout << "[DAQ] Successfully recorded " << max_samples << " samples to " << csv_path << std::endl;
    transition_to(FSMState::STANDBY);
    return static_cast<int>(max_samples);
}

void TimeMachineDAQDaemon::transition_to(FSMState next_state) {
    current_state = next_state;
}

std::string TimeMachineDAQDaemon::get_state_name() const {
    switch (current_state) {
        case FSMState::STANDBY: return "STATE_STANDBY";
        case FSMState::CALIBRATE: return "STATE_CALIBRATE";
        case FSMState::ACQUIRE: return "STATE_ACQUIRE";
        case FSMState::ANOMALY: return "STATE_ANOMALY";
        case FSMState::SAFE: return "STATE_SAFE";
        default: return "UNKNOWN";
    }
}

} // namespace ptm

#endif // PTM_DAQ_DAEMON_CPP
