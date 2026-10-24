with open(r"C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\source\build_part12_full.py", "r", encoding="utf-8") as f:
    text = f.read()

for fn in ["generate_part12_volume1", "generate_part12_volume2", "generate_part12_volume3"]:
    p = text.find("def " + fn)
    p_end = text.find("def ", p + 10)
    if p_end == -1:
        p_end = len(text)
    chunk = text[p:p_end]
    lines = chunk.split("\n")
    tuples = [l for l in lines if l.startswith('        ("')]
    print(f"{fn}: {len(tuples)} entries")
