# build_part13.py - Generates Part 13 Volumes 1 to 5 (Exact 40 physical pages each)
# Focus: LiDAR Geodesy, Computer Vision Optical Metrology, 3D Spacetime Digital Twins,
# Laser Time Transfer, and Level-10 Multi-Modal Sensor Fusion Synthesis.

import os, sys
from reportlab.platypus import Paragraph
from builder_base import build_40page_pdf
from reportlab_engine import get_project_styles

FIG_DIR = r'C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\figures'
DOC_DIR = r'C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\documents'
os.makedirs(DOC_DIR, exist_ok=True)
styles = get_project_styles()

def make_page_entries(raw_pages):
    """Converts a list of 39 raw tuples into the exact dict format required by builder_base."""
    if len(raw_pages) != 39:
        raise ValueError(f"Expected exactly 39 page tuples, got {len(raw_pages)}")
    pages_data = []
    for header, intro, sub1_t, sub1_tx, ftype, fdata, sub2_t, sub2_tx in raw_pages:
        pages_data.append({
            'header': header,
            'intro': intro,
            'sub1_title': sub1_t,
            'sub1_text': sub1_tx,
            'flowable_type': ftype,
            'flowable_data': fdata,
            'sub2_title': sub2_t,
            'sub2_text': sub2_tx
        })
    return pages_data
