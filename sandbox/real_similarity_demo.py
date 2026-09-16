from collections import Counter
import torch
import torch.nn.functional as F


FEATURE_ORDER = [
    "load",
    "store",
    "arithmetic_add",
    "arithmetic_mul",
    "stack_management",
    "conditional_branch",
    "unconditional_branch",
    "call",
    "return",
    "move",
    "other",
]


def get_opcode(instruction):
    return instruction.strip().split()[0].lower()


def categorize_instruction(instruction):
    parts = instruction.strip().split()
    opcode = parts[0].lower()

    # Stack management
    if opcode in {"sub", "add"} and len(parts) > 1:
        if parts[1].startswith("sp"):
            return "stack_management"

    # Loads
    if opcode in {"ldr", "ldur"}:
        return "load"

    # Stores
    if opcode in {"str", "stur"}:
        return "store"

    # Arithmetic semantics
    if opcode in {"add", "adds"}:
        return "arithmetic_add"

    if opcode in {"mul", "madd", "msub"}:
        return "arithmetic_mul"

    # Branches
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


def extract_feature_vector(instructions):
    categories = [
        categorize_instruction(inst)
        for inst in instructions
    ]

    counts = Counter(categories)

    return torch.tensor(
        [
            counts.get(feature, 0)
            for feature in FEATURE_ORDER
        ],
        dtype=torch.float32,
    )


add_instructions = [
    "sub sp, sp, #0x10",
    "str w0, [sp, #0xc]",
    "str w1, [sp, #0x8]",
    "ldr w8, [sp, #0xc]",
    "ldr w9, [sp, #0x8]",
    "add w0, w8, w9",
    "add sp, sp, #0x10",
    "ret",
]


add_similar_instructions = [
    "sub sp, sp, #0x10",
    "str w0, [sp, #0xc]",
    "str w1, [sp, #0x8]",
    "ldr w8, [sp, #0xc]",
    "ldr w9, [sp, #0x8]",
    "add w0, w8, w9",
    "add sp, sp, #0x10",
    "ret",
]


multiply_instructions = [
    "sub sp, sp, #0x10",
    "str w0, [sp, #0xc]",
    "str w1, [sp, #0x8]",
    "ldr w8, [sp, #0xc]",
    "ldr w9, [sp, #0x8]",
    "mul w0, w8, w9",
    "add sp, sp, #0x10",
    "ret",
]


add_vector = extract_feature_vector(add_instructions)
add_similar_vector = extract_feature_vector(add_similar_instructions)
multiply_vector = extract_feature_vector(multiply_instructions)


sim_add_add = F.cosine_similarity(
    add_vector.unsqueeze(0),
    add_similar_vector.unsqueeze(0),
).item()

sim_add_mul = F.cosine_similarity(
    add_vector.unsqueeze(0),
    multiply_vector.unsqueeze(0),
).item()


print("=== Mini-DeepClone Real Function Similarity v0.1 ===")

print("\nFeature Order:")
print(FEATURE_ORDER)

print("\nadd:")
print(add_vector)

print("\nadd_similar:")
print(add_similar_vector)

print("\nmultiply:")
print(multiply_vector)

print("\nSimilarity(add, add_similar):")
print(sim_add_add)

print("\nSimilarity(add, multiply):")
print(sim_add_mul)