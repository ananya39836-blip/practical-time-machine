"""
run_all_project_pipelines.py - Master Pipeline Execution Script
Executes all scientific computation, unit testing, simulation,
data validation, figure generation, and document auditing scripts.
"""

import os
import sys
import subprocess
import time

BASE_DIR = r"C:\Users\HP\.gemini\antigravity\scratch\time-machine-project"
SOURCE_DIR = os.path.join(BASE_DIR, "source")

scripts_to_execute = [
    ("Physics Unit Tests", os.path.join(SOURCE_DIR, "test_simulation.py")),
    ("Relativistic Simulation Engine", os.path.join(SOURCE_DIR, "simulation_engine.py")),
    ("Empirical Data Validator", os.path.join(SOURCE_DIR, "data_validator.py")),
    ("Standard Plot Generation", os.path.join(SOURCE_DIR, "generate_plots.py")),
    ("Advanced Figure Generation (Figs 6-9)", os.path.join(SOURCE_DIR, "generate_new_figures.py")),
    ("Master 36-Volume Document Audit", os.path.join(SOURCE_DIR, "audit_all_volumes.py")),
]

def main():
    print("=" * 80)
    print("  PRACTICAL TIME MACHINE: EXECUTING ALL SCIENTIFIC & BUILD PIPELINES")
    print("=" * 80)
    print(f"Base Directory: {BASE_DIR}")
    print(f"Timestamp: {time.strftime('%Y-%m-%d %H:%M:%S UTC', time.gmtime())}\n")

    overall_success = True
    results = []

    for name, script_path in scripts_to_execute:
        print(f">>> Running: {name} ({os.path.basename(script_path)})...")
        t0 = time.time()
        try:
            res = subprocess.run(
                [sys.executable, script_path],
                cwd=SOURCE_DIR,
                capture_output=True,
                text=True,
                timeout=120
            )
            elapsed = time.time() - t0
            if res.returncode == 0:
                print(f"    [SUCCESS] Finished in {elapsed:.2f}s")
                results.append((name, "SUCCESS", f"{elapsed:.2f}s", ""))
            else:
                print(f"    [FAILED] Exit code {res.returncode}")
                print(f"    Error output:\n{res.stderr[:300]}")
                results.append((name, "FAILED", f"{elapsed:.2f}s", res.stderr[:200]))
                overall_success = False
        except Exception as e:
            elapsed = time.time() - t0
            print(f"    [ERROR] Exception: {e}")
            results.append((name, "ERROR", f"{elapsed:.2f}s", str(e)))
            overall_success = False

    print("\n" + "=" * 80)
    print("  PIPELINE EXECUTION SUMMARY MATRIX")
    print("=" * 80)
    print(f"{'Pipeline Task':<40} | {'Status':<10} | {'Time':<8} | {'Details'}")
    print("-" * 80)
    for name, status, elapsed, err in results:
        print(f"{name:<40} | {status:<10} | {elapsed:<8} | {err[:20] if err else 'Clean execution'}")
    print("-" * 80)
    print(f"OVERALL EXECUTION STATUS: {'ALL PIPELINES PASSED (100%)' if overall_success else 'ISSUES ENCOUNTERED'}")
    print("=" * 80)

if __name__ == "__main__":
    main()
