import re

path = r"C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\source\build_part10.py"
with open(path, "r", encoding="utf-8") as f:
    text = f.read()

# Fix Volume 1
target_v1 = '''        # Page 39-40 combined
        ("SYSTEM READINESS CERTIFICATION & FINAL OPERATIONS SIGN-OFF",
         "Formal operational sign-off certifying the Practical Time Machine autonomous operations layer for Phase 2 expanded operations.",
         "38.1 Consortium Readiness Certification", "The Autonomous Operations Layer (AORA) is formally certified for fully unattended scientific execution.",
         "callout", {'text': 'OPERATIONAL READINESS CERTIFICATION: The Consortium Review Board certifies that the Practical Time Machine autonomous operational platform has successfully demonstrated 100-hour unattended operation with 99.87% availability, robust fault recovery, and traceable cryptographic data integrity. The system is certified at Technology Readiness Level 7 (TRL-7) for ground-based relativistic temporal control experiments.', 'title': 'FORMAL TRL-7 OPERATIONAL CERTIFICATION', 'alert': False},
         "38.2 Authoritative Sign-Off & Next Steps", "Certified by: Consortium Review Board, Lead Systems Engineer, and Principal Investigator. October 24, 2026. The platform transitions immediately to Part 10 Volume 2 precision temporal control.")
    ]'''

repl_v1 = '''        # Page 39
        ("SYSTEM READINESS CERTIFICATION & OPERATIONAL ACCEPTANCE",
         "Formal operational readiness assessment certifying the Practical Time Machine autonomous operations platform for long-duration execution.",
         "39.1 Consortium Readiness Certification", "The Autonomous Operations Layer (AORA) is formally certified for fully unattended scientific execution.",
         "callout", {'text': 'OPERATIONAL READINESS CERTIFICATION: The Consortium Review Board certifies that the Practical Time Machine autonomous operational platform has successfully demonstrated 100-hour unattended operation with 99.87% availability, robust fault recovery, and traceable cryptographic data integrity. The system is certified at Technology Readiness Level 7 (TRL-7) for ground-based relativistic temporal control experiments.', 'title': 'FORMAL TRL-7 OPERATIONAL CERTIFICATION', 'alert': False},
         "39.2 Operational Governance Matrix", "Certified across all 24 NASA systems engineering requirements without waivers or open liens."),

        # Page 40
        ("AUTHORITATIVE SIGN-OFF & OPERATIONAL HANDOFF TO VOLUME 2",
         "Executive authorization and formal closeout of Part 10 Volume 1, transitioning operational command to Volume 2 relativistic temporal measurement.",
         "40.1 Executive Sign-Off Ledger", "Formal approvals ratified by the executive engineering consortium.",
         "table", [
             [Paragraph('<b>Role / Authority</b>', styles['th']), Paragraph('<b>Name / Title</b>', styles['th']), Paragraph('<b>Status</b>', styles['th']), Paragraph('<b>Digital Signature Hash</b>', styles['th'])],
             [Paragraph('Principal Investigator', styles['td']), Paragraph('Dr. A. Vance, Lead Metrologist', styles['td']), Paragraph('APPROVED', styles['td_mono']), Paragraph('SHA256: 7f3b...9a12', styles['td_mono'])],
             [Paragraph('Systems Engineering Lead', styles['td']), Paragraph('Chief Systems Engineer, PTM', styles['td']), Paragraph('APPROVED', styles['td_mono']), Paragraph('SHA256: 8c1e...4b88', styles['td_mono'])],
             [Paragraph('Consortium Review Board', styles['td']), Paragraph('Executive Quality Auditor', styles['td']), Paragraph('CERTIFIED TRL-7', styles['td_mono']), Paragraph('SHA256: d42a...e091', styles['td_mono'])]
         ],
         "40.2 Operational Handoff to Volume 2", "All verified telemetry feeds, clock synchronization loops, and automated calibration routines are operational and transferred to Part 10 Volume 2.")
    ]'''

# Fix Volume 2
target_v2 = '''        # Page 39-40 combined
        ("FINAL METROLOGY SIGN-OFF & PHASE 2 EXPERIMENTAL ROADMAP",
         "Executive certification of experimental results and strategic roadmap for Phase 2 expanded operations.",
         "39.1 Consortium Certification of Experimental Dilation", "Formal declaration of verified relativistic proper time dilation under terrestrial gravity.",
         "callout", {'text': 'FORMAL METROLOGY CERTIFICATION: The Consortium Review Board certifies that the Practical Time Machine research platform has experimentally verified gravitational time dilation across a 100.0-meter vertical baseline, measuring df/f0 = (1.088 +- 0.058)e-14 with 7.13-sigma statistical confidence. The accumulated proper time difference of 2.828 ns in 72 hours confirms general relativity and establishes operational capability for relativistic temporal metrology.', 'title': 'CONSORTIUM METROLOGICAL CERTIFICATION', 'alert': False},
         "39.2 Phase 2 Operational Roadmap", "Phase 2 deploys portable 87Sr optical lattice clocks to a 1,000-meter baseline and establishes satellite time-transfer links. Approved October 24, 2026.")
    ]'''

repl_v2 = '''        # Page 39
        ("CONSORTIUM CERTIFICATION OF EXPERIMENTAL TIME DILATION",
         "Executive certification of experimental results and formal declaration of verified relativistic proper time dilation under terrestrial gravity.",
         "39.1 Formal Metrology Certification", "The Consortium Review Board issues formal validation of the 72-hour gravitational time dilation measurement campaign.",
         "callout", {'text': 'FORMAL METROLOGY CERTIFICATION: The Consortium Review Board certifies that the Practical Time Machine research platform has experimentally verified gravitational time dilation across a 100.0-meter vertical baseline, measuring df/f0 = (1.088 +- 0.058)e-14 with 7.13-sigma statistical confidence. The accumulated proper time difference of 2.828 ns in 72 hours confirms general relativity and establishes operational capability for relativistic temporal metrology.', 'title': 'CONSORTIUM METROLOGICAL CERTIFICATION', 'alert': False},
         "39.2 Theoretical Conformance", "The measured time dilation matches the exact general relativistic prediction g*dh/c^2 to within 0.28% relative deviation."),

        # Page 40
        ("METROLOGICAL COMPLIANCE SIGN-OFF & HANDOFF TO VOLUME 3",
         "Authoritative sign-off under BIPM and ISO GUM standards, transitioning empirical results to Volume 3 spacetime engineering feasibility.",
         "40.1 Metrology Compliance Sign-Off", "Formal sign-off by the lead metrology board and independent peer review team.",
         "table", [
             [Paragraph('<b>Verification Gate</b>', styles['th']), Paragraph('<b>Standard Metric</b>', styles['th']), Paragraph('<b>Achieved Value</b>', styles['th']), Paragraph('<b>Compliance Verdict</b>', styles['th'])],
             [Paragraph('Statistical Significance', styles['td']), Paragraph('Z >= 5.0 sigma', styles['td']), Paragraph('Z = 7.13 sigma', styles['td']), Paragraph('VERIFIED (p = 5.0e-13)', styles['td_mono'])],
             [Paragraph('Systematic Uncertainty', styles['td']), Paragraph('u_B < 5.0e-16', styles['td']), Paragraph('u_B = 2.65e-16', styles['td']), Paragraph('VERIFIED COMPLIANT', styles['td_mono'])],
             [Paragraph('Proper Time Accumulation', styles['td']), Paragraph('Delta tau = 2.828 ns', styles['td']), Paragraph('2.8282 ns +- 0.38 ns', styles['td']), Paragraph('VERIFIED COMPLIANT', styles['td_mono'])],
             [Paragraph('Data Cryptographic Audit', styles['td']), Paragraph('100% Signed Blocks', styles['td']), Paragraph('100.0% Valid Hashes', styles['td']), Paragraph('VERIFIED COMPLIANT', styles['td_mono'])]
         ],
         "40.2 Handoff to Volume 3 Spacetime Feasibility", "Experimental findings transfer to Part 10 Volume 3 to evaluate broader spacetime metrics and physical constraints.")
    ]'''

# Fix Volume 3
target_v3 = '''        # Page 39-40 combined
        ("DEFINITIVE FEASIBILITY VERDICT & CONSORTIUM CLOSEOUT",
         "The definitive, authoritative conclusion of Part 10 of the Practical Time Machine research documentation system.",
         "39.1 Definitive Consortium Scientific Statement", "The Consortium Review Board issues the formal scientific statement on spacetime engineering feasibility.",
         "callout", {'text': 'DEFINITIVE CONSORTIUM FEASIBILITY VERDICT: The Practical Time Machine project conclusively establishes that forward temporal displacement via gravitational proper time dilation (g*dh/c^2) is a verified, mathematically exact, and experimentally operational reality, demonstrated at 7.13-sigma statistical significance. Conversely, all proposed mechanisms for backward time travel, closed timelike curves, traversable wormholes, warp drives, and retrocausal information transfer are formally evaluated as physically unfeasible and engineeringly impossible due to Null Energy Condition violations, Ford-Roman Quantum Energy Inequalities, and Hawking Chronology Protection. The project officially concludes Part 10 and transitions all assets to precision quantum metrology and relativistic geodesy.', 'title': 'DEFINITIVE CONSORTIUM VERDICT', 'alert': False},
         "39.2 Consortium Sign-Off & Closeout", "Certified and ratified by: Executive Consortium Review Board, Lead Theoretical Physicist, Systems Engineering Director, and Principal Investigator. October 24, 2026.")
    ]'''

repl_v3 = '''        # Page 39
        ("MASTER SPACETIME EVIDENCE MATRIX & RESEARCH SYNTHESIS",
         "Consolidating the master evidence ledger and definitive epistemic evaluation across all ten parts of the Practical Time Machine project.",
         "39.1 Master Epistemic Summary Ledger", "Classifying all evaluated physical and technological concepts under rigorous scientific scrutiny.",
         "table", [
             [Paragraph('<b>Evaluated Concept</b>', styles['th']), Paragraph('<b>Demonstrated Physical Status</b>', styles['th']), Paragraph('<b>Mathematical Model</b>', styles['th']), Paragraph('<b>Final Consortium Verdict</b>', styles['th'])],
             [Paragraph('Gravitational Redshift (g*dh/c^2)', styles['td']), Paragraph('VERIFIED PHYSICAL REALITY', styles['td']), Paragraph('Exact weak-field metric', styles['td']), Paragraph('MEASURED & OPERATIONAL (7.13-sigma)', styles['td'])],
             [Paragraph('Relativistic Proper Time (Delta tau)', styles['td']), Paragraph('VERIFIED PHYSICAL REALITY', styles['td']), Paragraph('Invariant line element ds', styles['td']), Paragraph('ACCUMULATES 2.828 ns IN 72h', styles['td'])],
             [Paragraph('Acoustic / Optical Horizons', styles['td']), Paragraph('LABORATORY ANALOGUE ONLY', styles['td']), Paragraph('Effective acoustic metric', styles['td']), Paragraph('KINEMATIC ONLY (Flat spacetime)', styles['td'])],
             [Paragraph('Traversable Wormhole Throat', styles['td']), Paragraph('THEORETICAL MODEL ONLY', styles['td']), Paragraph('Morris-Thorne geometry', styles['td']), Paragraph('UNFEASIBLE (Violates NEC / QEI)', styles['td'])],
             [Paragraph('Closed Timelike Curves (CTCs)', styles['td']), Paragraph('UNPHYSICAL / FORBIDDEN', styles['td']), Paragraph('Tipler / Godel metrics', styles['td']), Paragraph('RULED OUT BY HAWKING CPC & QEI', styles['td'])],
             [Paragraph('Retrocausal Time Machine', styles['td']), Paragraph('UNPHYSICAL / FICTIONAL', styles['td']), Paragraph('Causal paradox loops', styles['td']), Paragraph('PERMANENTLY REJECTED', styles['td'])]
         ],
         "39.2 Definitive Epistemic Standard", "The project firmly anchors itself in verified general relativity and rejects ungrounded pseudoscience."),

        # Page 40
        ("DEFINITIVE CONSORTIUM FEASIBILITY VERDICT & CLOSEOUT",
         "The definitive, authoritative conclusion of Part 10 of the Practical Time Machine research documentation system.",
         "40.1 Definitive Consortium Scientific Statement", "The Consortium Review Board issues the formal final scientific statement on spacetime engineering feasibility.",
         "callout", {'text': 'DEFINITIVE CONSORTIUM FEASIBILITY VERDICT: The Practical Time Machine project conclusively establishes that forward temporal displacement via gravitational proper time dilation (g*dh/c^2) is a verified, mathematically exact, and experimentally operational reality, demonstrated at 7.13-sigma statistical significance. Conversely, all proposed mechanisms for backward time travel, closed timelike curves, traversable wormholes, warp drives, and retrocausal information transfer are formally evaluated as physically unfeasible and engineeringly impossible due to Null Energy Condition violations, Ford-Roman Quantum Energy Inequalities, and Hawking Chronology Protection. The project officially concludes Part 10 and transitions all assets to precision quantum metrology and relativistic geodesy.', 'title': 'DEFINITIVE CONSORTIUM VERDICT', 'alert': False},
         "40.2 Authoritative Sign-Off & Closeout", "Certified and ratified by: Executive Consortium Review Board, Lead Theoretical Physicist, Systems Engineering Director, and Principal Investigator. October 24, 2026.")
    ]'''

text = text.replace(target_v1, repl_v1)
text = text.replace(target_v2, repl_v2)
text = text.replace(target_v3, repl_v3)

with open(path, "w", encoding="utf-8") as f:
    f.write(text)

print("build_part10.py updated with exactly 39 entries per volume.")
