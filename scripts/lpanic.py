import os
import re

Import("env")
print("Extra script activated")


def replace_panic_with_lpanic(source, target, env):
    file_path = os.path.join(os.getcwd(), "lib", "minilua", "minilua.h")
    if not os.path.exists(file_path):
        print("File not found:", file_path)
        return

    try:
        with open(file_path, "r", encoding="utf-8") as f:
            content = f.read()
    except Exception as e:
        print("Error reading file:", e)
        return

    pattern = r"(?P<prefix>[ &])panic(?P<rest>\s*[\(\)])"

    def repl(match):
        prefix = match.group("prefix")
        rest = match.group("rest")
        return f"{prefix}lpanic{rest}"

    new_content = re.sub(pattern, repl, content)

    if new_content == content:
        print("No changes required or no calls to panic found")
        return

    try:
        with open(file_path, "w", encoding="utf-8") as f:
            f.write(new_content)
        print("Replacement successfully performed in:", file_path)
    except Exception as e:
        print("Error writing file:", e)


env.Execute(replace_panic_with_lpanic)
