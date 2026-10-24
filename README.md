# Practical Time Machine: Advanced Spacetime Engineering Research Project

**Subtitle:** A Science-First Investigation into Temporal Physics, Relativistic Engineering, Experimental Validation, and Future Technologies.

---
[Video demonstration](https://drive.google.com/file/d/1uPqUT2GegT6num9Lq7J0O7jT6Q6xUJ89/view?usp=drivesdk)
[GitHub url](https://github.com/ananya39836-blip/practical-time-machine/tree/main)

[Streamlit Application URL](https://practical-time-machine.streamlit.app/)

## Executive Overview
The **Practical Time Machine** research initiative investigates whether an engineered laboratory system capable of producing, measuring, and exploiting relativistic temporal effects can be built with known physics and experimentally testable methods.

The project strictly separates:
- **Forward time travel via relativistic proper time dilation**, which is mathematically proven and experimentally measured.
- **Precision differential measurement** of proper time accumulation between clocks operating at differing gravitational potentials ($g\Delta h / c^2$).
- **Speculative spacetime engineering** (Morris-Thorne wormholes, Closed Timelike Curves, Tipler cylinders), which are investigated as theoretical general relativistic research topics rather than demonstrated technology.
- **Physical impossibilities**: Travel into the historical past is constrained by the Null Energy Condition (NEC), Ford-Roman Quantum Energy Inequalities (QEI), and Hawking's Chronology Protection Conjecture.

---

## Three-Volume Technical Documentation Architecture
The primary deliverable of this project is a **120-page, three-volume technical documentation suite**, where each volume is an official engineering PDF containing **exactly 40 physical A4 pages**:

| Volume | Title | Identifier | Physical Pages | File Path |
| :--- | :--- | :--- | :---: | :--- |
| **Volume 1** | *Experimental System Architecture and Hardware Design* | `PTM-ENG-VOL1-REV2.0` | **40 Pages** | [`documents/Time_Machine_Part2_PDF1_Hardware_Architecture.pdf`](documents/Time_Machine_Part2_PDF1_Hardware_Architecture.pdf) |
| **Volume 2** | *Physical Models, Quantitative Engineering and Simulation* | `PTM-SIM-VOL2-REV2.0` | **40 Pages** | [`documents/Time_Machine_Part2_PDF2_Simulation_Software.pdf`](documents/Time_Machine_Part2_PDF2_Simulation_Software.pdf) |
| **Volume 3** | *Experimental Implementation, Test Procedures and Validation* | `PTM-OPS-VOL3-REV2.0` | **40 Pages** | [`documents/Time_Machine_Part2_PDF3_Assembly_Validation.pdf`](documents/Time_Machine_Part2_PDF3_Assembly_Validation.pdf) |

---

## Selected Physical Prototype
- **Name:** Differential Relativistic Metric and Gravitational Time Dilation Validation Platform (DRM-GTDVP)
- **Primary Physical Mechanism:** Weak-field terrestrial gravitational redshift:
  $$\frac{\Delta f}{f_0} = \frac{g \Delta h}{c^2} \approx 1.0909 \times 10^{-16} \text{ m}^{-1} \cdot \Delta h$$
- **Baseline Configuration:** $\Delta h = 100.0\text{ m}$ elevation baseline producing a fractional frequency advance of $1.091 \times 10^{-14}\text{ s/s}$, accumulating **$0.9427\text{ ns/day}$ ($2.828\text{ ns}$ over 72 hours)**.
- **Hardware Foundation:**
  - 2× Stanford Research Systems PRS10 Rubidium Frequency Standards (Allan deviation $\sigma_y(100\text{s}) \le 2 \times 10^{-12}$)
  - 1× Keysight 53230A Universal Time Interval Counter (20 ps single-shot resolution)
  - 1× Paroscientific Digiquartz 745-16B Barometer ($0.08\text{ hPa}$ accuracy)
  - Dual-layer Mu-Metal magnetic shielding ($S > 1,400$, residual field $< 40\text{ nT}$)
  - Active dual-stage Peltier TEC thermal chamber ($\pm 0.002^\circ\text{C}$ core stability)
  - Star-point isolated electrical grounding and Huber+Suhner Sucoflex 104 phase-matched coaxial cabling.

---

## Project Directory Layout
```
time-machine-project/
├── README.md                           # Master project guide
├── project-charter.md                  # Formal NASA-style project charter
├── data/
│   ├── simulated_experiment_72h.csv    # 72-hour synthetic measurement dataset
│   └── experiment_metadata.json        # Canonical JSON configuration schema
├── design/
│   ├── system-requirements.md          # Traceable requirements matrix (REQ-SYS-001..020)
│   ├── system-architecture.md          # 5-layer subsystem architecture & interfaces
│   └── simulation-plan.md              # Computational verification & validation plan
├── documents/
│   ├── Time_Machine_Part2_PDF1_Hardware_Architecture.pdf  # Exact 40 pages
│   ├── Time_Machine_Part2_PDF2_Simulation_Software.pdf    # Exact 40 pages
│   ├── Time_Machine_Part2_PDF3_Assembly_Validation.pdf    # Exact 40 pages
│   └── [Document Aliases for Parts 1, 3, 4, 5]
├── experiments/
│   ├── experimental-protocol.md        # SOP-PTM-001 14-day observation protocol
│   └── data-analysis-plan.md           # Weighted Least Squares & Allan variance pipeline
├── figures/
│   ├── fig1_allan_deviation.png        # Allan deviation comparison
│   ├── fig2_time_dilation_accumulation.png # Proper time vs duration & elevation
│   ├── fig3_system_architecture.png    # Interconnect block diagram
│   ├── fig4_synthetic_residuals.png    # 72-hour tracking & Kalman residuals
│   └── fig5_spacetime_metrics.png      # Wormhole NEC stress-energy deficit
├── research/
│   ├── literature-review.md            # Peer-reviewed relativistic literature review
│   ├── evidence-matrix.csv             # Structured evidence & claims taxonomy
│   ├── assumptions-register.md         # Explicit physical & engineering assumptions
│   └── risk-register.csv               # FMEA risk & failure mitigation ledger
├── source/
│   ├── reportlab_engine.py             # Custom styling, 40-page NumberedCanvas engine
│   ├── build_volume1.py                # Volume 1 generator (40 pages)
│   ├── build_volume2.py                # Volume 2 generator (40 pages)
│   ├── build_volume3.py                # Volume 3 generator (40 pages)
│   ├── simulation_engine.py            # Executable relativistic Python engine
│   ├── test_simulation.py              # Automated unit tests (100% pass)
│   └── generate_plots.py               # Technical figure generator
└── validation/
    ├── page-count-report.txt           # Automated PDF page verification report
    ├── citation-audit.md               # Primary literature citation audit
    └── pdf-quality-report.md           # Visual & typography quality audit
```

---

## Verification & Execution Instructions
To execute unit tests:
```bash
python -m unittest discover -s source -p "test_*.py"
```
To run the relativistic simulation engine:
```bash
python source/simulation_engine.py
```
To regenerate all three 40-page PDF volumes:
```bash
python source/build_volume1.py
python source/build_volume2.py
python source/build_volume3.py
```
