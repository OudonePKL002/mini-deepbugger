from collections import Counter


instructions = [
    "sub sp, sp, #0x10",
    "str w0, [sp, #0x8]",
    "ldr w8, [sp, #0x8]",
    "subs w8, w8, #0xa",
    "b.le 0x1000004a4",
]


def get_opcode(instruction):
    return instruction.strip().split()[0].lower()


def categorize_opcode(opcode):
    if opcode in {"ldr", "ldur"}:
        return "load"

    if opcode in {"str", "stur"}:
        return "store"

    if opcode in {"add", "sub", "adds", "subs", "mul"}:
        return "arithmetic"

    if opcode.startswith("b."):
        return "conditional_branch"

    if opcode in {"b", "br"}:
        return "unconditional_branch"

    if opcode in {"bl", "blr"}:
        return "call"

    if opcode == "ret":
        return "return"

    if opcode in {"mov", "movz", "movk"}:
        return "move"

    return "other"


opcodes = []
categories = []

for instruction in instructions:
    opcode = get_opcode(instruction)
    category = categorize_opcode(opcode)

    opcodes.append(opcode)
    categories.append(category)


print("=== Instructions ===")
for instruction in instructions:
    print(instruction)

print("\n=== Opcodes ===")
print(opcodes)

print("\n=== Instruction Categories ===")
for opcode, category in zip(opcodes, categories):
    print(f"{opcode:8} -> {category}")

print("\n=== Feature Counts ===")
counts = Counter(categories)

for category, count in counts.items():
    print(f"{category:25}: {count}")