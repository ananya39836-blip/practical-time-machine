"""
build_all_volumes.py - Master Build and Verification Pipeline
Practical Time Machine Technical Documentation System.
Compiles all 3 volumes, validates exact 40-page count for each,
and generates validation reports.
"""

import os
import sys
import time
from pypdf import PdfReader

# Add source directory
SOURCE_DIR = os.path.dirname(__file__)
sys.path.insert(0, SOURCE_DIR)

from build_volume1 import generate_volume1
from build_volume2 import generate_volume2
from build_volume3 import generate_volume3

DOCS_DIR = r'C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\documents'
VAL_DIR = r'C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\validation'
os.makedirs(VAL_DIR, exist_ok=True)

def build_and_verify_all():
    print("=" * 70)
    print("PRACTICAL TIME MACHINE — MASTER DOCUMENT COMPILATION PIPELINE")
    print("=" * 70)
    
    start_time = time.time()
    
    # 1. Build Volume 1
    print("\n>>> COMPILING VOLUME 1: HARDWARE ARCHITECTURE...")
    p1 = generate_volume1()
    
    # 2. Build Volume 2
    print("\n>>> COMPILING VOLUME 2: QUANTITATIVE SIMULATION...")
    p2 = generate_volume2()
    
    # 3. Build Volume 3
    print("\n>>> COMPILING VOLUME 3: ASSEMBLY & VALIDATION...")
    p3 = generate_volume3()
    
    elapsed = time.time() - start_time
    
    # Validation Audit
    volumes = [
        ("Volume 1: Hardware Architecture", os.path.join(DOCS_DIR, "Time_Machine_Part2_PDF1_Hardware_Architecture.pdf"), p1),
        ("Volume 2: Quantitative Simulation", os.path.join(DOCS_DIR, "Time_Machine_Part2_PDF2_Simulation_Software.pdf"), p2),
        ("Volume 3: Assembly & Validation", os.path.join(DOCS_DIR, "Time_Machine_Part2_PDF3_Assembly_Validation.pdf"), p3),
    ]
    
    report_lines = []
    report_lines.append("PRACTICAL TIME MACHINE — DOCUMENT VALIDATION & PAGE-COUNT AUDIT")
    report_lines.append("=" * 70)
    report_lines.append(f"Audit Date: {time.strftime('%Y-%m-%d %H:%M:%S UTC', time.gmtime())}")
    report_lines.append(f"Total Compilation Elapsed Time: {elapsed:.2f} seconds\n")
    
    all_passed = True
    total_pages = 0
    
    for title, path, pages in volumes:
        reader = PdfReader(path)
        actual_count = len(reader.pages)
        size_kb = os.path.getsize(path) / 1024.0
        status = "PASSED (EXACTLY 40 PAGES)" if actual_count == 40 else f"FAILED ({actual_count} pages)"
        if actual_count != 40:
            all_passed = False
        total_pages += actual_count
        
        report_lines.append(f"Document: {title}")
        report_lines.append(f"  File Path:    {path}")
        report_lines.append(f"  File Size:    {size_kb:.1f} KB")
        report_lines.append(f"  Target Pages: 40")
        report_lines.append(f"  Actual Pages: {actual_count}")
        report_lines.append(f"  Audit Status: {status}\n")
        
    report_lines.append("=" * 70)
    report_lines.append(f"TOTAL SYSTEM PAGES: {total_pages} / 120 TARGET")
    report_lines.append(f"OVERALL AUDIT VERDICT: {'ALL 3 VOLUMES PASSED (100% COMPLIANT)' if all_passed else 'NON-COMPLIANT'}")
    report_lines.append("=" * 70)
    
    report_text = "\n".join(report_lines)
    print("\n" + report_text)
    
    # Write report file
    report_file = os.path.join(VAL_DIR, "page-count-report.txt")
    with open(report_file, "w") as f:
        f.write(report_text)
        
    print(f"\nAudit report saved to: {report_file}")
    return all_passed

if __name__ == '__main__':
    success = build_and_verify_all()
    if not success:
        sys.exit(1)
