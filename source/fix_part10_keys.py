path = r"C:\Users\HP\.gemini\antigravity\scratch\time-machine-project\source\build_part10.py"
with open(path, "r", encoding="utf-8") as f:
    text = f.read()

old_block = """        pages_data.append({
            'title': header,
            'intro': intro,
            'sub1_title': sub1_t,
            'sub1_text': sub1_tx,
            'feature_type': ftype,
            'feature_data': fdata,
            'sub2_title': sub2_t,
            'sub2_text': sub2_tx
        })"""

new_block = """        pages_data.append({
            'header': header,
            'intro': intro,
            'sub1_title': sub1_t,
            'sub1_text': sub1_tx,
            'flowable_type': ftype,
            'flowable_data': fdata,
            'sub2_title': sub2_t,
            'sub2_text': sub2_tx
        })"""

text = text.replace(old_block, new_block)

with open(path, "w", encoding="utf-8") as f:
    f.write(text)

print("Updated dict keys in build_part10.py successfully.")
