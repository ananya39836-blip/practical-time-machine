# System Architecture Specification: DRM-GTDVP Platform

**Document ID:** `PTM-ENG-ARCH-REV2.0`  
**Classification:** UNCLASSIFIED / FORMAL ARCHITECTURE SPECIFICATION  
**Date of Release:** October 2026  

---

## 1. Architectural Philosophy & Layered Decomposition
The Differential Relativistic Metric and Gravitational Time Dilation Validation Platform (DRM-GTDVP) is designed as a five-tier hierarchically decoupled system. Strict galvanic, RF, and thermal barriers isolate each layer to prevent spurious cross-coupling.

```
+-----------------------------------------------------------------------+
| LAYER 5: SUPERVISORY CONTROL & RELATIVISTIC SOLVER                    |
| - Ubuntu PREEMPT_RT Real-Time Daemon | Python SCPI/VISA Manager       |
| - Discrete Kalman State Estimator    | Overlapping Allan Variance     |
| - Multi-variable Environmental WLS   | SQLite WAL / HDF5 Archival     |
+-----------------------------------------------------------------------+
                                   | (LXI / Gigabit Ethernet / RS-232)
+-----------------------------------------------------------------------+
| LAYER 4: SENSORY & ENVIRONMENTAL TELEMETRY                           |
| - 4-Wire 1/10 DIN PT100 RTD Sensors (Omega P-M-1/10-1/8-6)            |
| - Quartz Resonant Barometer (Paroscientific Digiquartz 745-16B)      |
| - 3-Axis Precision Fluxgate Magnetometer (Bartington Mag-03MS100)     |
+-----------------------------------------------------------------------+
                                   | (Isolated Differential ADC)
+-----------------------------------------------------------------------+
| LAYER 3: TIME-INTERVAL METROLOGY & PHASE COMPARISON                   |
| - Keysight 53230A Universal Time Interval Counter (20 ps Single-Shot) |
| - Dual-Mixer Time Difference (DMTD) Sub-Femtosecond Phase Engine      |
| - Multi-GNSS Carrier-Phase PPP Receiver (Septentrio PolaRx5)          |
+-----------------------------------------------------------------------+
                                   | (Sucoflex 104 Matched Coax, 10 MHz Sine)
+-----------------------------------------------------------------------+
| LAYER 2: ENVIRONMENTAL ISOLATION & MECHANICAL ENCLOSURE               |
| - Double-Walled High-Permeability Mu-Metal Shields (S > 1,400)        |
| - Active Dual-Stage Peltier TEC Temperature Chamber (±0.002°C RMS)    |
| - Damped Optical Breadboard with Pneumatic Air Mounts (VC-E Class)    |
+-----------------------------------------------------------------------+
                                   | (Direct Physics Package Thermal Cradle)
+-----------------------------------------------------------------------+
| LAYER 1: PRIMARY FREQUENCY GENERATION & ATOMIC STANDARDS              |
| - Station A (Ground Datum): SRS PRS10 Rubidium Standard (10 MHz)      |
| - Station B (Elevated):    SRS PRS10 Rubidium Standard (10 MHz)      |
| - Low-Noise Distribution Amplifier (SRS FS735, > 120 dB Isolation)    |
+-----------------------------------------------------------------------+
```

---

## 2. Hardware Subsystem Allocations

### 2.1 Subsystem 1: Frequency Generation
- **Primary Oscillators:** Dual Stanford Research Systems PRS10 Rubidium Atomic Frequency Standards disciplined by an internal 10 MHz SC-cut OCXO.
- **Phase Noise:** $-130\text{ dBc/Hz}$ at 10 Hz offset; $-155\text{ dBc/Hz}$ at 10 kHz.
- **Short-Term Stability:** $\sigma_y(1\text{s}) = 2.0 \times 10^{-11}$; $\sigma_y(100\text{s}) = 1.0 \times 10^{-12}$; Flicker floor reaches $1.5 \times 10^{-13}$ at $\tau = 10^4\text{ s}$.

### 2.2 Subsystem 2: Environmental Shielding
- **Magnetic Shield:** Concentric double-layer cylinders fabricated from ASTM A753 Alloy 4 Mu-Metal (1.5 mm thickness, 1,150°C hydrogen annealed). Static shielding factor $S = 1,420$; internal residual magnetic field $< 40\text{ nT}$.
- **Thermal Stabilization:** Nested aluminum chambers insulated with 30 mm aerogel batting ($k = 0.015\text{ W/m}\cdot\text{K}$). Four Marlow Peltier TEC modules driven by Wavelength Electronics PTC10K-CH analog controllers stabilize the physics package to $30.000 \pm 0.002^\circ\text{C}$.
- **Vibration Mounts:** Newport M-VIS3030 optical breadboard suspended by pneumatic isolators with self-leveling valves. Fundamental resonance engineered to 1.5 Hz.

### 2.3 Subsystem 3: Measurement & Metrology
- **Time Interval Counter:** Keysight 53230A dual-channel counter utilizing dual Vernier analog delay interpolation (20 ps single-shot resolution, 12 digits/s frequency resolution).
- **RF Harnessing:** Huber+Suhner Sucoflex 104 low-loss microwave cabling phase-matched to within $\pm 1.0\text{ picosecond}$ at 10 MHz.

---

## 3. Interface Definitions & Signal Routing (ICD Summary)
- **ICD-01 (10 MHz RF Output):** $+7.0 \pm 0.5\text{ dBm}$ sine into $50.0\ \Omega$, SMA connector, isolation $> 80\text{ dB}$.
- **ICD-02 (1 PPS Trigger):** TTL pulse ($0\text{ to }+3.3\text{ V}$) into $50\ \Omega$, rise time $< 2.0\text{ ns}$, jitter $< 25\text{ ps RMS}$, BNC bulkhead.
- **ICD-03 (LXI Network):** IEEE 488.2 SCPI over raw TCP port 5025, shielded RJ45 Cat6A, galvanic chassis isolation.
- **ICD-04 (Sensor Serial):** RS-232 / RS-485 at 115,200 baud, 8N1, optical isolation 2.5 kV RMS.
