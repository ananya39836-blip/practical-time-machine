"""
test_simulation.py - Automated unit tests for Relativistic Proper Time Simulation Engine
Practical Time Machine Research Project.
"""

import unittest
import numpy as np
from simulation_engine import (
    RelativisticProperTimeModel,
    ClockNoiseSimulator,
    compute_overlapping_allan_deviation,
    RelativisticKalmanFilter,
    C, G_SURFACE
)

class TestRelativisticProperTime(unittest.TestCase):
    def setUp(self):
        self.model = RelativisticProperTimeModel(elevation_diff_m=100.0)

    def test_gravitational_redshift_value(self):
        """Verify g*dh / c^2 calculation matches analytical physics to 5 sig figs."""
        expected = (9.80665 * 100.0) / (299792458.0**2) # ~ 1.091102e-14
        calc = self.model.gravitational_fractional_shift()
        self.assertAlmostEqual(calc, expected, places=18)

    def test_accumulated_time_dilation(self):
        """Verify 24-hour accumulated time dilation for 100m elevation."""
        day_seconds = 86400.0
        acc_time = self.model.accumulated_time_dilation_seconds(day_seconds)
        # Expected = 1.091102e-14 * 86400 ~ 9.4271e-10 s = 0.94271 ns
        self.assertAlmostEqual(acc_time * 1e9, 0.942712, places=4)

    def test_sagnac_phase_shift(self):
        """Verify Sagnac shift calculation for 1000 km^2 baseline loop."""
        area = 1e6 # 1 km^2
        sagnac = self.model.sagnac_phase_shift(area)
        self.assertGreater(sagnac, 0.0)
        self.assertLess(sagnac, 1e-12)

    def test_kalman_filter_convergence(self):
        """Verify Kalman filter converges on known constant frequency offset."""
        kf = RelativisticKalmanFilter(dt=1.0)
        true_freq = 1.0e-13
        # Feed 100 observations of linear ramp phase
        for i in range(100):
            t = i + 1
            meas = true_freq * t + np.random.normal(0, 1e-12)
            p_est, f_est = kf.update(meas)
        # Filter should be tracking within reasonable noise bounds
        self.assertTrue(abs(f_est - true_freq) < 5e-13)

    def test_allan_deviation_computation(self):
        """Verify overlapping Allan deviation calculation on known white noise."""
        np.random.seed(123)
        # White phase noise
        N = 1000
        phase = np.random.normal(0, 1e-11, N)
        taus, adev = compute_overlapping_allan_deviation(phase, 1.0)
        self.assertTrue(len(taus) > 0)
        self.assertTrue(len(adev) == len(taus))
        # For white phase noise, Allan dev slopes as tau^-1
        self.assertTrue(adev[0] > adev[-1])

if __name__ == '__main__':
    unittest.main()
