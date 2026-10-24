with open(r"C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\source\build_part12.py", "r", encoding="utf-8") as f:
    text = f.read()

lines = [l for l in text.split("\n") if l.startswith('        ("')]
print("Entries in p_defs:", len(lines))
