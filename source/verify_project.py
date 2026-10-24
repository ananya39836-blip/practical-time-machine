"""
verify_project.py - Final Comprehensive Verification and Audit Script
Checks all tests, datasets, models, code, and PDFs.
"""

import os
import unittest
from pypdf import PdfReader

BASE_DIR = r'C:\Users\HP\.gemini\antigravity\scratch\time-machine-project'

def run_all_checks():
    print("=" * 70)
    print("PRACTICAL TIME MACHINE — COMPREHENSIVE PROJECT VERIFICATION")
    print("=" * 70)
    
    # 1. Run Unit Tests
    print("\n[CHECK 1/4] Running Automated Unit Tests...")
    suite = unittest.defaultTestLoader.discover(os.path.join(BASE_DIR, 'source'), pattern='test_*.py')
    runner = unittest.TextTestRunner(verbosity=2)
    test_result = runner.run(suite)
    if not test_result.wasSuccessful():
        print("ERROR: Unit tests failed!")
        return False
        
    # 2. Run Data Validator
    print("\n[CHECK 2/4] Running Data Validator & Acceptance Criteria Audit...")
    from data_validator import run_validation
    val_passed = run_validation()
    if not val_passed:
        print("ERROR: Data validation failed!")
        return False
        
    # 3. Check PDF Pages
    print("\n[CHECK 3/4] Verifying 3 PDF Document Page Counts & Sizes...")
    docs = [
        "Time_Machine_Part2_PDF1_Hardware_Architecture.pdf",
        "Time_Machine_Part2_PDF2_Simulation_Software.pdf",
        "Time_Machine_Part2_PDF3_Assembly_Validation.pdf"
    ]
    total_pages = 0
    all_exact_40 = True
    for d in docs:
        path = os.path.join(BASE_DIR, 'documents', d)
        if not os.path.exists(path):
            print(f"ERROR: Missing document: {path}")
            return False
        r = PdfReader(path)
        cnt = len(r.pages)
        sz = os.path.getsize(path)
        total_pages += cnt
        status = "EXACT 40 PAGES (PASS)" if cnt == 40 else f"FAIL ({cnt} pages)"
        print(f"  * {d}: {cnt} pages, {sz:,} bytes -> {status}")
        if cnt != 40:
            all_exact_40 = False
            
    print(f"\n  Total Pages Across 3 Volumes: {total_pages} / 120 Target")
    if not all_exact_40 or total_pages != 120:
        print("ERROR: PDF Page count audit failed!")
        return False
        
    # 4. Inventory Core Artifacts
    print("\n[CHECK 4/4] Verifying Project Artifacts in Workspace...")
    required_files = [
        "README.md",
        "project-charter.md",
        "research/literature-review.md",
        "research/evidence-matrix.csv",
        "research/assumptions-register.md",
        "research/risk-register.csv",
        "design/system-requirements.md",
        "design/system-architecture.md",
        "design/simulation-plan.md",
        "experiments/experimental-protocol.md",
        "experiments/data-analysis-plan.md",
        "validation/page-count-report.txt",
        "validation/citation-audit.md",
        "validation/pdf-quality-report.md"
    ]
    missing = []
    for req in required_files:
        p = os.path.join(BASE_DIR, req)
        if os.path.exists(p):
            print(f"  [EXISTS] {req} ({os.path.getsize(p):,} bytes)")
        else:
            missing.append(req)
            print(f"  [MISSING] {req}")
            
    if missing:
        print(f"ERROR: Missing required files: {missing}")
        return False
        
    print("\n" + "=" * 70)
    print("ALL VERIFICATION CHECKS PASSED WITH 100% COMPLIANCE!")
    print("=" * 70)
    return True

if __name__ == '__main__':
    run_all_checks()
