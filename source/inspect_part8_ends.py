with open(r"C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\source\build_part8.py", "r", encoding="utf-8") as f:
    text = f.read()

for fn in ["generate_part8_volume1", "generate_part8_volume2", "generate_part8_volume3"]:
    p = text.find("def " + fn)
    p_end = text.find("def ", p + 10)
    if p_end == -1:
        p_end = len(text)
    chunk = text[p:p_end]
    lines = [l.strip() for l in chunk.split("\n") if l.startswith('        ("')]
    print(f"\n{fn}:")
    print("  Entry 38:", lines[-2])
    print("  Entry 39:", lines[-1])
