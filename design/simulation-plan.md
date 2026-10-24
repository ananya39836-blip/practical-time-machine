# Numerical Simulation & Verification Plan

**Document ID:** `PTM-SIM-PLAN-REV2.0`  
**Classification:** UNCLASSIFIED / FORMAL SIMULATION PLAN  
**Date of Release:** October 2026  

---

## 1. Objectives & Scope
The simulation plan defines the mathematical models, numerical algorithms, parameter sweep configurations, and acceptance test cases required to validate the DRM-GTDVP platform before and during physical experimental campaigns.

---

## 2. Core Simulation Architecture

### 2.1 Relativistic Proper Time Integrator
- **Metric Formulation:** Weak-field terrestrial Schwarzschild line element in Earth-Centered Earth-Fixed (ECEF) coordinates:
  $$d\tau = dt \cdot \sqrt{1 - \frac{2GM}{rc^2} - \frac{(\mathbf{\Omega}_E \times \mathbf{r})^2}{c^2} - \frac{v^2}{c^2}}$$
- **Numerical Integration:** 4th-Order Runge-Kutta (RK4) integrator with local truncation error $\mathcal{O}(\Delta t^5)$ evaluated at 1-second step intervals.
- **Verification Criterion:** Discrepancy between RK4 numerical integration and exact analytical Taylor expansion must remain $< 1.0 \times 10^{-16}$ over 72 hours.

### 2.2 Stochastic Clock Noise Synthesis
- **Algorithm:** Barnes-Greenstein / Kasdin frequency-domain colored noise filter via Inverse Real Fast Fourier Transform (IRFFT).
- **Power-Law Composition:**
  $$S_y(f) = h_0 + \frac{h_{-1}}{f} + \frac{h_{-2}}{f^2}$$
  with $h_0 = 2.0 \times 10^{-21}\text{ Hz}^{-1}$, $h_{-1} = 1.0 \times 10^{-23}$, and $h_{-2} = 5.0 \times 10^{-28}\text{ Hz}$.
- **Verification Criterion:** Simulated Allan variance curve $\sigma_y(\tau)$ must match manufacturer specification curves within $\pm 10\%$ from $\tau = 1\text{ s}$ to $\tau = 100,000\text{ s}$.

### 2.3 Discrete-Continuous Kalman Filter
- **State Vector:** $\mathbf{X}(t) = [x(t), y(t), D(t)]^T$ (phase error, fractional frequency offset, linear aging drift).
- **Measurement Model:** $\mathbf{z}_k = \mathbf{H} \mathbf{x}_k + v_k$, where $\mathbf{H} = [1, 0, 0]$ and $R = (20\text{ ps})^2$.
- **Verification Criterion:** Kalman state estimate converges to true simulated frequency offset within $3\sigma$ confidence interval in $< 100$ measurement cycles.

---

## 3. Automated Test Suite Specification
The Python test module `source/test_simulation.py` executes 5 automated regression tests:
1. `test_gravitational_redshift_value`: Confirms $g\Delta h / c^2$ analytical calculation to 18 decimal places.
2. `test_accumulated_time_dilation`: Confirms 24-hour proper time accumulation for $\Delta h = 100\text{ m}$ ($0.94271\text{ ns}$).
3. `test_sagnac_phase_shift`: Confirms Sagnac time shift formula for rotating frame baselines.
4. `test_kalman_filter_convergence`: Confirms Kalman filter converges on known constant frequency offset under noise.
5. `test_allan_deviation_computation`: Confirms overlapping Allan deviation calculation on known white noise sequences.
