# Data Analysis & Statistical Validation Plan

**Document ID:** `PTM-OPS-ANAPLAN-REV2.0`  
**Classification:** UNCLASSIFIED / FORMAL STATISTICAL ANALYSIS PLAN  
**Date of Release:** October 2026  

---

## 1. Analysis Objectives
The data analysis pipeline processes raw time-interval counter phase measurements, subtracts environmental covariates, computes overlapping Allan deviations, fits the relativistic proper time slope via Weighted Least Squares (WLS), and tests all 10 Quantitative Acceptance Criteria (AC-01 through AC-10).

---

## 2. Statistical Pipeline Architecture

```
Raw CSV Phase Telemetry (1 Hz)
            |
            v
[Stage 1: Data Integrity & Parity Check] -> Exclude malformed frames
            |
            v
[Stage 2: MAD Outlier Filter (M > 4.5)] -> Flag phase slips > 100 ps
            |
            v
[Stage 3: Environmental De-Trending] ---> Regress Temp & Pressure cycles
            |
            v
[Stage 4: Kalman State Estimation] -----> Track Phase x(t), Freq y(t), Drift D(t)
            |
            v
[Stage 5: Overlapping Allan Variance] --> Compute sigma_y(tau) curve
            |
            v
[Stage 6: Acceptance Gates Audit] ------> Evaluate AC-01 through AC-10
            |
            v
Publication-Ready Summary Report (PDF / Markdown / JSON)
```

---

## 3. Mathematical Equations and Statistical Criteria

### 3.1 Median Absolute Deviation (MAD) Outlier Filter
For successive phase increments $\Delta x_i = x_{i+1} - x_i$:
$$\text{MAD} = \text{median}\left( |\Delta x_i - \text{median}(\Delta x)| \right)$$
Modified Z-score:
$$M_i = \frac{0.6745 \cdot |\Delta x_i - \text{median}(\Delta x)|}{\text{MAD}}$$
Samples with $M_i > 4.5$ are flagged with `integrity_flag = 1` and excluded from physical regression.

### 3.2 Overlapping Allan Deviation
$$\sigma_y^2(m \tau_0) = \frac{1}{2 m^2 \tau_0^2 (N - 2m)} \sum_{i=1}^{N-2m} \left[ x_{i+2m} - 2 x_{i+m} + x_i \right]^2$$

### 3.3 Weighted Least Squares (WLS) Parameter Estimation
$$\hat{\boldsymbol{\theta}} = (\mathbf{X}^T \mathbf{W} \mathbf{X})^{-1} \mathbf{X}^T \mathbf{W} \mathbf{x}$$
where $\hat{\boldsymbol{\theta}} = [x_0, y_{\text{rel}}, D]^T$ and $\mathbf{W} = \text{Cov}(\mathbf{x})^{-1}$.

### 3.4 Whiteness & Normality Testing
- **Durbin-Watson Autocorrelation Statistic:**
  $$d = \frac{\sum_{i=2}^N (e_i - e_{i-1})^2}{\sum_{i=1}^N e_i^2}$$
  Acceptance threshold: $d \in [1.80, 2.20]$.
- **Kolmogorov-Smirnov Normality Test:**
  $$D = \sup_x |F_n(x) - F_0(x)|$$
  Acceptance threshold: $p \ge 0.05$.
