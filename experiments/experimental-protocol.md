# Standard Operating Procedure SOP-PTM-001: 14-Day Differential Observation Run

**Document ID:** `PTM-OPS-SOP-001-REV2.0`  
**Classification:** UNCLASSIFIED / FORMAL LABORATORY STANDARD OPERATING PROCEDURE  
**Date of Release:** October 2026  

---

## 1. Scope & Objective
This procedure specifies the mandatory operational steps for executing an official 14-day continuous differential relativistic elevation observation campaign on the DRM-GTDVP platform.

---

## 2. Pre-Requisites & Required Equipment
- 2× Stanford Research Systems PRS10 Rubidium Standards (Soaked $\ge 48\text{ hours}$, Lock LED Green).
- 1× Keysight 53230A Universal Time Interval Counter (NIST 17025 Cal Certificate valid).
- 1× Newport M-VIS3030 Optical Bench floating at $15.0 \pm 1.0\text{ mm}$.
- Phase-matched coaxial harnesses W101 and W102 torqued to $0.90\text{ N}\cdot\text{m}$.
- Surveyed vertical elevation baseline $\Delta h = 100.000 \pm 0.005\text{ meters}$.
- Supervisory Industrial PC running Ubuntu 24.04 RT with `ptm-acquisition.service` installed.

---

## 3. Step-by-Step Execution Sequence

### Phase 1: Pre-Run Hold Point (T - 2 Hours)
1. Verify cleanroom ambient air temperature: $21.0 \pm 0.5^\circ\text{C}$; RH: $45 \pm 5\%$.
2. Verify core temperatures on Station A and Station B: $30.000 \pm 0.005^\circ\text{C}$.
3. Verify zero-baseline calibration report signed off; noise floor $< 15\text{ ps/day}$.
4. Verify disk partition `/data` free space $> 200\text{ GB}$.

### Phase 2: Interconnect & Sealing (T - 30 Minutes)
5. Station A is situated at Ground Datum; Station B is elevated to Platform ($+100.0\text{ m}$).
6. Connect W101 from Station A 10 MHz to Keysight Ch1; connect W102 from Station B to Ch2.
7. Torque all SMA coupling nuts to $0.90\text{ N}\cdot\text{m}$ using calibrated torque wrench.
8. Seal optical canopy doors.

### Phase 3: Acquisition Start (T = 0)
9. Engage acquisition daemon: `systemctl start ptm-acquisition.service`.
10. Check service status: `systemctl status ptm-acquisition` must report `Active (running)`.
11. Tail output stream: `tail -f /data/EXP-DRM-001_raw.csv`. Verify 1 Hz rows appending.
12. Inspect initial 60 samples: verify time-interval readings are advancing at $\sim +1.09 \times 10^{-14}\text{ s/s}$.

### Phase 4: Daily Shift Audit (Day 1 to 14)
13. Perform twice-daily shift inspection at 08:00 and 20:00 UTC (complete 10-point checklist).
14. Log environmental covariates in electronic laboratory ledger.
15. Verify Kalman filter normalized innovation squared $\text{NIS} < 6.63$.

### Phase 5: Run Conclusion & Archival (Day 14 + 1 Hour)
16. Stop acquisition service: `systemctl stop ptm-acquisition.service`.
17. Force SQLite WAL checkpoint: `sqlite3 /data/ptm.db 'PRAGMA wal_checkpoint(TRUNCATE);'`.
18. Generate SHA-256 cryptographic digest: `sha256sum EXP-DRM-001_raw.csv > MANIFEST.sha256`.
19. Set file permissions to read-only: `chmod 444 EXP-DRM-001_raw.csv`.
20. Execute automated validation pipeline: `python source/data_validator.py`.
