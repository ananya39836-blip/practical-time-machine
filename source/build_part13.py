# build_part13.py - Master build runner for all 5 Part 13 volumes (Exact 40 physical pages each)
import os, sys
from pypdf import PdfReader

sys.path.insert(0, os.path.dirname(__file__))
from build_part13_vol1 import generate_part13_volume1
from build_part13_vol2 import generate_part13_volume2
from build_part13_vol3 import generate_part13_volume3
from build_part13_vol4 import generate_part13_volume4
from build_part13_vol5 import generate_part13_volume5

DOC_DIR = r'C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\documents'

def build_all_part13():
    print("=" * 80)
    print("STARTING PART 13 COMPILATION: 5 VOLUMES (EXACT 40 PHYSICAL PAGES EACH)")
    print("=" * 80)
    
    generate_part13_volume1()
    generate_part13_volume2()
    generate_part13_volume3()
    generate_part13_volume4()
    generate_part13_volume5()
    
    files = [
        'Time_Machine_Part13_Volume_1_LiDAR_Geodesy_and_Metrology.pdf',
        'Time_Machine_Part13_Volume_2_Computer_Vision_Optical_Metrology.pdf',
        'Time_Machine_Part13_Volume_3_3D_Spacetime_Modeling_Digital_Twin.pdf',
        'Time_Machine_Part13_Volume_4_Laser_Ranging_and_Optical_Time_Transfer.pdf',
        'Time_Machine_Part13_Volume_5_Level10_MultiModal_Sensor_Fusion_Synthesis.pdf'
    ]
    
    for f in files:
        full_p = os.path.join(DOC_DIR, f)
        reader = PdfReader(full_p)
        p_count = len(reader.pages)
        print(f"Verified: {f} -> {p_count} physical pages ({os.path.getsize(full_p):,} bytes).")
        assert p_count == 40, f"Page count error in {f}: expected 40, got {p_count}"
        
    print("=" * 80)
    print("ALL 5 PART 13 VOLUMES SUCCESSFULLY VERIFIED AT EXACTLY 40 PAGES EACH (200 PAGES TOTAL)")
    print("=" * 80)

if __name__ == '__main__':
    build_all_part13()
