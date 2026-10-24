import os
source_dir = r"C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\source"
for fname in ['simulation_engine.py', 'daq_daemon.py', 'data_validator.py', 'test_simulation.py', 'verify_project.py', 'generate_plots.py']:
    fpath = os.path.join(source_dir, fname)
    if os.path.exists(fpath):
        size = os.path.getsize(fpath)
        with open(fpath, 'r', encoding='utf-8') as f:
            lines = f.readlines()
        print(f"{fname}: {len(lines)} lines, {size} bytes")
        if lines:
            print(f"   Header: {lines[0].strip()}")
