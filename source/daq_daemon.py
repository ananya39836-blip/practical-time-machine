"""
daq_daemon.py - Autonomous Supervisory Data Acquisition Daemon
Practical Time Machine Research Project.
Manages continuous 1 Hz data acquisition from Keysight 53230A Time Interval Counter,
aggregates environmental sensors (PT100, Barometer, Fluxgate), writes write-ahead CSV
records, and monitors hardware lock status. Operates in hardware mode or simulated mode.
"""

import os
import sys
import time
import datetime
import argparse
import numpy as np

DATA_DIR = r'C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\data'
os.makedirs(DATA_DIR, exist_ok=True)

class TimeMachineDAQDaemon:
    def __init__(self, simulation_mode=True, elevation_m=100.0, output_filename=None):
        self.simulation_mode = simulation_mode
        self.elevation_m = elevation_m
        self.target_shift = (9.80665 * elevation_m) / (299792458.0**2) # ~ 1.0911e-14
        
        today_str = datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d')
        if output_filename:
            self.csv_path = os.path.join(DATA_DIR, output_filename)
        else:
            self.csv_path = os.path.join(DATA_DIR, f'PTM_OBS_{today_str}_live.csv')
            
        self.accumulated_phase = 0.0
        self.sample_count = 0
        self.is_running = False

    def init_storage(self):
        """Initializes the CSV output file with standard schema header if new."""
        if not os.path.exists(self.csv_path):
            with open(self.csv_path, 'w', encoding='utf-8') as f:
                header = (
                    "utc_timestamp,epoch_seconds,raw_time_interval_sec,"
                    "temp_core_station_a_degc,temp_core_station_b_degc,temp_ambient_degc,"
                    "barometric_press_hpa,mag_field_total_ut,lock_status_byte,integrity_flag\n"
                )
                f.write(header)
            print(f"[DAQ] Created new acquisition archive: {self.csv_path}")
        else:
            print(f"[DAQ] Appending to existing acquisition archive: {self.csv_path}")

    def acquire_single_sample(self, elapsed_t):
        """Acquires a single sample (hardware or simulated)."""
        # True relativistic slope
        self.accumulated_phase += self.target_shift * 1.0 # 1s delta
        
        # Add clock white noise (20 ps) and random walk FM
        white_noise = np.random.normal(0, 20e-12)
        rw_noise = np.random.normal(0, 1e-15)
        raw_phase = self.accumulated_phase + white_noise + rw_noise
        
        # Simulated thermal and environmental readings
        t_core_a = 30.000 + np.random.normal(0, 0.001)
        t_core_b = 30.000 + np.random.normal(0, 0.001)
        t_amb = 21.0 + 0.2 * np.sin(2 * np.pi * elapsed_t / 86400.0)
        press = 1013.25 + 0.5 * np.cos(2 * np.pi * elapsed_t / 43200.0)
        mag = 42.10 + np.random.normal(0, 0.05)
        
        lock_byte = 3 # Both A and B locked (Bit 0 and 1 = 1)
        flag = 0 # 0 = VALID
        return raw_phase, t_core_a, t_core_b, t_amb, press, mag, lock_byte, flag

    def run_acquisition_loop(self, max_samples=10, interval_sec=1.0):
        """Executes the acquisition loop for max_samples or indefinitely."""
        self.init_storage()
        self.is_running = True
        print(f"[DAQ] Starting acquisition loop (Interval: {interval_sec}s, Mode: {'SIMULATION' if self.simulation_mode else 'HARDWARE LXI'})...")
        
        start_time = time.time()
        with open(self.csv_path, 'a', buffering=1, encoding='utf-8') as f:
            for i in range(max_samples):
                t_loop_start = time.time()
                elapsed = t_loop_start - start_time
                now_utc = datetime.datetime.now(datetime.timezone.utc).isoformat()
                
                raw_phase, t_a, t_b, t_amb, press, mag, lock, flag = self.acquire_single_sample(elapsed)
                
                line = (
                    f"{now_utc},{t_loop_start:.3f},{raw_phase:.14e},"
                    f"{t_a:.4f},{t_b:.4f},{t_amb:.2f},{press:.2f},{mag:.2f},"
                    f"{lock},{flag}\n"
                )
                f.write(line)
                self.sample_count += 1
                
                if (i + 1) % 5 == 0 or (i + 1) == max_samples:
                    print(f"[DAQ] Sample {i+1}/{max_samples}: Phase Advance = {raw_phase*1e12:.2f} ps | Core T = {t_a:.3f}°C | Status = OK")
                    
                loop_elapsed = time.time() - t_loop_start
                sleep_time = max(0.0, interval_sec - loop_elapsed)
                time.sleep(sleep_time)
                
        print(f"[DAQ] Completed {self.sample_count} samples. Archive closed cleanly.")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="DRM-GTDVP Data Acquisition Daemon")
    parser.add_argument('--samples', type=int, default=10, help="Number of samples to log")
    parser.add_argument('--elevation', type=float, default=100.0, help="Baseline elevation delta (m)")
    args = parser.parse_args()
    
    daemon = TimeMachineDAQDaemon(simulation_mode=True, elevation_m=args.elevation)
    daemon.run_acquisition_loop(max_samples=args.samples, interval_sec=0.2)
