#ifndef PTM_COMPUTER_VISION_HPP
#define PTM_COMPUTER_VISION_HPP

#include "types.hpp"
#include <vector>

namespace ptm {

class ComputerVisionEngine {
public:
    double loop_rate_hz; // Real-time vision loop (200 Hz default)

    explicit ComputerVisionEngine(double rate_hz = 200.0);

    // Executes sub-pixel laser beam tracking simulation over num_frames
    VisionTrackingResult run_tracking_simulation(size_t num_frames = 200, uint64_t seed = 101) const;

    // Sub-pixel 2D moment centroid calculation from intensity grid
    static void compute_subpixel_centroid(const std::vector<double>& intensity_grid,
                                          size_t width, size_t height,
                                          double& xc, double& yc, double threshold = 0.0);

    // Simulates Ramsey-Bordé cold-atom absorption interferogram (100x100 grid)
    static std::vector<double> generate_atom_fringe_image(size_t grid_dim, double phase_shift_rad, double contrast = 0.485);

    // Extracts interferometric fringe contrast and phase from image grid
    static void extract_fringe_metrics(const std::vector<double>& image, size_t grid_dim,
                                       double& extracted_contrast, double& extracted_phase);

    // Calculates composite Optical Alignment Index (OAI)
    static double calculate_oai(double rms_jitter_nm, double m2_factor = 1.034, double atom_ratio = 1.0);
};

} // namespace ptm

#endif // PTM_COMPUTER_VISION_HPP
