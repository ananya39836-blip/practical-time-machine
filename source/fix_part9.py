import sys

path = r"C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\source\build_part9.py"
with open(path, "r", encoding="utf-8") as f:
    text = f.read()

p1 = text.find('("MASTER EVIDENCE MATRIX')
p2 = text.find('pages_data = []\n    for header', p1)
p_end = text.rfind('\n', 0, p2)

replacement = """("MASTER EVIDENCE MATRIX & FINAL FEASIBILITY DECISION",
         "Consolidating the master evidence matrix and recording the final feasibility decision of Part 9. Forward proper time dilation is verified; backward time travel is permanently ruled out.",
         "39.1 Master Epistemic Summary Ledger", "Classifying all evaluated physical and technological concepts.",
         "table", [
             [Paragraph('<b>Evaluated Concept</b>', styles['th']), Paragraph('<b>Demonstrated Physical Status</b>', styles['th']), Paragraph('<b>Mathematical Model</b>', styles['th']), Paragraph('<b>Final Consortium Verdict</b>', styles['th'])],
             [Paragraph('Gravitational Redshift (g*dh/c2)', styles['td']), Paragraph('VERIFIED PHYSICAL REALITY', styles['td']), Paragraph('Exact weak-field metric', styles['td']), Paragraph('MEASURED & OPERATIONAL (7.13-sigma)', styles['td'])],
             [Paragraph('Relativistic Proper Time (Delta tau)', styles['td']), Paragraph('VERIFIED PHYSICAL REALITY', styles['td']), Paragraph('Invariant line element ds', styles['td']), Paragraph('ACCUMULATES 2.828 ns IN 72h', styles['td'])],
             [Paragraph('Acoustic / Optical Analogue Horizons', styles['td']), Paragraph('LABORATORY ANALOGUE ONLY', styles['td']), Paragraph('Effective acoustic metric', styles['td']), Paragraph('KINEMATIC SIMULATION (Flat spacetime)', styles['td'])],
             [Paragraph('Traversable Wormhole Throat', styles['td']), Paragraph('THEORETICAL MODEL ONLY', styles['td']), Paragraph('Morris-Thorne geometry', styles['td']), Paragraph('UNFEASIBLE (Violates NEC / QEI)', styles['td'])],
             [Paragraph('Closed Timelike Curves (CTCs)', styles['td']), Paragraph('UNPHYSICAL / FORBIDDEN', styles['td']), Paragraph('Tipler / Godel metrics', styles['td']), Paragraph('RULED OUT BY HAWKING CPC & QEI', styles['td'])],
             [Paragraph('Retrocausal Time Machine', styles['td']), Paragraph('UNPHYSICAL / FICTIONAL', styles['td']), Paragraph('Causal paradox loops', styles['td']), Paragraph('PERMANENTLY REJECTED', styles['td'])]
         ],
         "39.2 Definitive Feasibility Verdict & Consortium Sign-Off", "Consortium Review Board certifies that forward gravitational proper time dilation is verified at 7.13-sigma, while backward time travel and wormholes are permanently rejected under NEC, QEI, and Hawking CPC theorems. Certified October 24, 2026.")
    ]
"""

# Find the end of the p_defs list
idx_bracket = text.rfind(']', p1, p2)
idx_start = p1

new_text = text[:idx_start] + replacement + text[idx_bracket+1:]
with open(path, "w", encoding="utf-8") as f:
    f.write(new_text)

print("build_part9.py updated successfully.")
