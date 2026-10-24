with open(r"C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\source\build_part12_full.py", "r", encoding="utf-8") as f:
    text = f.read()

lines = [l for l in text.split("\n") if l.startswith('        ("')]
print("Entries in p_defs for Vol 1:", len(lines))
