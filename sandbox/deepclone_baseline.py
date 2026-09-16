import subprocess
import re
from collections import Counter

import torch
import torch.nn.functional as F
import csv
from pathlib import Path
from itertools import combinations

BINARY_PATH = "sandbox/hello"


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


WEIGHTS = torch.tensor(
    [
        0.5,  # load
        0.5,  # store
        2.0,  # arithmetic_add
        2.0,  # arithmetic_mul
        0.5,  # stack_management
        1.0,  # conditional_branch
        1.0,  # unconditional_branch
        1.0,  # call
        0.5,  # return
        1.0,  # move
        1.0,  # other
    ],
    dtype=torch.float32,
)


# --------------------------------------------------
# 1. Disassemble binary
# --------------------------------------------------

def disassemble_binary(binary_path):

    result = subprocess.run(
        ["otool", "-tvV", binary_path],
        capture_output=True,
        text=True,
        check=True,
    )

    return result.stdout


# --------------------------------------------------
# 2. Extract one function
# --------------------------------------------------

def extract_function(disassembly, function_name):

    lines = disassembly.splitlines()

    target = f"_{function_name}:"

    instructions = []

    inside_function = False

    for line in lines:

        stripped = line.strip()

        if stripped == target:
            inside_function = True
            continue

        if inside_function:

            # Stop when next function begins
            if stripped.endswith(":"):
                break

            match = re.match(
                r"^[0-9a-fA-F]+\s+(.+)$",
                stripped,
            )

            if match:
                instructions.append(
                    match.group(1)
                )

    return instructions


# --------------------------------------------------
# 3. Categorize instructions
# --------------------------------------------------

def categorize_instruction(instruction):

    parts = instruction.strip().split()

    opcode = parts[0].lower()

    # Stack setup / cleanup
    if opcode in {"sub", "add"} and len(parts) > 1:
        if parts[1].startswith("sp"):
            return "stack_management"

    if opcode in {"ldr", "ldur"}:
        return "load"

    if opcode in {"str", "stur"}:
        return "store"

    if opcode in {"add", "adds"}:
        return "arithmetic_add"

    if opcode in {"mul", "madd", "msub"}:
        return "arithmetic_mul"

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


# --------------------------------------------------
# 4. Extract feature vector
# --------------------------------------------------

def extract_feature_vector(instructions):

    categories = [
        categorize_instruction(inst)
        for inst in instructions
    ]

    counts = Counter(categories)

    vector = torch.tensor(
        [
            counts.get(feature, 0)
            for feature in FEATURE_ORDER
        ],
        dtype=torch.float32,
    )

    return vector


# --------------------------------------------------
# 5. Similarity
# --------------------------------------------------

def cosine_similarity(a, b):

    return F.cosine_similarity(
        a.unsqueeze(0),
        b.unsqueeze(0),
    ).item()


# --------------------------------------------------
# 6. Main pipeline
# --------------------------------------------------

disassembly = disassemble_binary(
    BINARY_PATH
)


function_names = [
    "add",
    "add_similar",

    "subtract",
    "subtract_similar",

    "multiply",
    "multiply_similar",

    "maximum",
    "maximum_similar",

    "minimum",
    "minimum_similar",

    "is_positive",
    "is_positive_similar",

    "is_negative",
    "is_negative_similar",

    "identity",
    "identity_similar",
]


vectors = {}


print("=== Mini-DeepClone Baseline Pipeline v0.1 ===")


for name in function_names:

    instructions = extract_function(
        disassembly,
        name,
    )

    raw_vector = extract_feature_vector(
        instructions
    )

    weighted_vector = (
        raw_vector * WEIGHTS
    )

    vectors[name] = {
        "instructions": instructions,
        "raw": raw_vector,
        "weighted": weighted_vector,
    }

    print(f"\nFunction: {name}")
    print("Instructions:", len(instructions))
    print("Raw features:")
    print(raw_vector)

    print("Weighted features:")
    print(weighted_vector)


# --------------------------------------------------
# 7. Compare functions
# --------------------------------------------------

pairs = list(combinations(function_names, 2))

results = []


print("\n=== Similarity Results ===")

def get_pair_label(a, b):

    positive_pairs = {
        frozenset(["add", "add_similar"]),
        frozenset(["subtract", "subtract_similar"]),
        frozenset(["multiply", "multiply_similar"]),
        frozenset(["maximum", "maximum_similar"]),
        frozenset(["minimum", "minimum_similar"]),
        frozenset(["is_positive", "is_positive_similar"]),
        frozenset(["is_negative", "is_negative_similar"]),
        frozenset(["identity", "identity_similar"]),
    }

    pair = frozenset([a, b])

    if pair in positive_pairs:
        return 1

    return 0


for a, b in pairs:

    raw_similarity = cosine_similarity(
        vectors[a]["raw"],
        vectors[b]["raw"],
    )

    weighted_similarity = cosine_similarity(
        vectors[a]["weighted"],
        vectors[b]["weighted"],
    )

    print(f"\n{a} vs {b}")

    print(
        "Raw similarity     :",
        round(raw_similarity, 4),
    )

    print(
        "Weighted similarity:",
        round(weighted_similarity, 4),
    )

    results.append({
        "function_a": a,
        "function_b": b,
        "label": get_pair_label(a, b),
        "raw_similarity": round(raw_similarity, 4),
        "weighted_similarity": round(weighted_similarity, 4),
    })


# --------------------------------------------------
# 8. Save results
# --------------------------------------------------

output_dir = Path("results")
output_dir.mkdir(exist_ok=True)

output_file = output_dir / "deepclone_baseline_results.csv"

with open(output_file, "w", newline="") as f:

    writer = csv.DictWriter(
        f,
        fieldnames=[
            "function_a",
            "function_b",
            "label",
            "raw_similarity",
            "weighted_similarity",
        ],
    )

    writer.writeheader()
    writer.writerows(results)


print("\nResults saved to:")
print(output_file)