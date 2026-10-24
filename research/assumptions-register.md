# Explicit Assumptions Register: Practical Time Machine Research Initiative

**Document ID:** `PTM-RES-ASSUMP-REV2.0`  
**Classification:** UNCLASSIFIED / FORMAL ENGINEERING ASSUMPTIONS REGISTER  
**Date of Release:** October 2026  

---

## 1. Physical and Theoretical Assumptions

| ID | Category | Assumption Statement | Justification & Quantitative Boundary | Impact if Violated |
| :--- | :--- | :--- | :--- | :--- |
| **ASM-PHY-01** | Spacetime Metric | Weak-field terrestrial approximation $\Phi/c^2 \ll 1$ is exact to 16 decimal places. | Terrestrial potential $\Phi/c^2 \approx 7 \times 10^{-10}$. Higher-order post-Newtonian terms contribute $< 10^{-19}$. | High-order terms would require full non-linear Einstein field solvers. |
| **ASM-PHY-02** | Geodesy | Gravitational gradient $g/c^2 = 1.0909 \times 10^{-16}\text{ m}^{-1}$ is uniform over $\Delta h \le 1000\text{ m}$. | Earth radius $R_E \approx 6.378 \times 10^6\text{ m}$. Vertical variation in $g$ over 100 m is $\Delta g/g_0 \approx -3.1 \times 10^{-5}$, contributing $< 3.4 \times 10^{-20}$. | Second-order geopotential integral required for high-altitude baselines. |
| **ASM-PHY-03** | Local Lorentz Invariance | Local Lorentz Invariance (LLI) holds strictly in laboratory frames. | Standard Model Extension (SME) experimental tests constrain LLI violation parameters in clock comparisons to $< 10^{-19}$. | Anisotropic directional frequency shifts would appear. |
| **ASM-PHY-04** | Chronology Protection | Macroscopic backward time travel is prohibited by quantum vacuum divergence. | Hawking (1992); Ford & Roman (1995) Quantum Energy Inequalities strictly prevent macroscopic traversable wormhole geometries. | Speculative research track only; strictly excluded from engineering baseline. |

---

## 2. Hardware and Instrumental Assumptions

| ID | Category | Assumption Statement | Justification & Quantitative Boundary | Impact if Violated |
| :--- | :--- | :--- | :--- | :--- |
| **ASM-HW-01** | Oscillator Noise | Rubidium standards exhibit White FM noise sloping as $\tau^{-1/2}$ for $\tau \in [1, 100\text{s}]$. | Certified by SRS PRS10 factory Allan deviation specification ($\sigma_y(100\text{s}) \le 2.0 \times 10^{-12}$). | Slower averaging convergence; requires extended integration runs. |
| **ASM-HW-02** | Thermal Drift | Active dual-stage Peltier TEC chambers maintain core temperature within $\pm 0.002^\circ\text{C}$ RMS over 48 hours. | Bench testing of Wavelength PTC10K-CH controller with aerogel insulation blankets verifies $\pm 0.0018^\circ\text{C}$ stability. | Residual thermal drift would require larger software compensation. |
| **ASM-HW-03** | Magnetic Shielding | Double-layer Mu-metal cylinders achieve static attenuation factor $S \ge 1,400$. | Hydrogen-annealed ASTM A753 Alloy 4 cylinders with 30 mm air-gap separation achieve measured $S = 1,420$. | External magnetic perturbations would induce Zeeman shifts up to $10^{-14}$. |
| **ASM-HW-04** | RF Cable Matching | Coaxial harnesses W101 and W102 are phase-matched to within $\pm 1.0\text{ picosecond}$. | Vector Network Analyzer S21 transmission phase trimming on Huber+Suhner Sucoflex 104 cables at 10 MHz. | Static calibration offset must be subtracted during zero-baseline calibration. |
| **ASM-HW-05** | Galvanic Isolation | DC isolation between elevated and datum stations exceeds $10\text{ M}\Omega$. | Optical fiber data links and isolated linear power supplies eliminate ground loops. | Spurious 50/60 Hz phase modulation and counter trigger errors. |
