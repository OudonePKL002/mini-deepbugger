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
    "arithmetic_sub",
    "arithmetic_mul",

    "stack_management",
    "compare",

    "branch_eq",
    "branch_ne",
    "branch_gt",
    "branch_ge",
    "branch_lt",
    "branch_le",

    "unconditional_branch",

    "cset_eq",
    "cset_ne",
    "cset_gt",
    "cset_ge",
    "cset_lt",
    "cset_le",

    "test_bit_zero",
    "test_bit_nonzero",

    "predicate_gt_zero",
    "predicate_lt_zero",

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
        2.0,  # arithmetic_sub
        2.0,  # arithmetic_mul

        0.5,  # stack_management
        1.5,  # compare

        2.0,  # branch_eq
        2.0,  # branch_ne
        2.0,  # branch_gt
        2.0,  # branch_ge
        2.0,  # branch_lt
        2.0,  # branch_le

        0.5,  # unconditional_branch

        2.0,  # cset_eq
        2.0,  # cset_ne
        2.0,  # cset_gt
        2.0,  # cset_ge
        2.0,  # cset_lt
        2.0,  # cset_le

        2.0,  # test_bit_zero
        2.0,  # test_bit_nonzero

        3.0,  # predicate_gt_zero
        3.0,  # predicate_lt_zero

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

    if not parts:
        return "other"

    opcode = parts[0].lower()

    # --------------------------------------
    # Stack setup / cleanup
    # --------------------------------------

    if opcode in {"sub", "add"} and len(parts) > 1:
        if parts[1].startswith("sp"):
            return "stack_management"

    # --------------------------------------
    # Memory
    # --------------------------------------

    if opcode in {"ldr", "ldur"}:
        return "load"

    if opcode in {"str", "stur"}:
        return "store"

    # --------------------------------------
    # Arithmetic
    # --------------------------------------

    if opcode in {"add", "adds"}:
        return "arithmetic_add"

    if opcode in {"sub"}:
        return "arithmetic_sub"

    if opcode in {"mul", "madd", "msub"}:
        return "arithmetic_mul"

    # --------------------------------------
    # Compare-like instructions
    # --------------------------------------

    if opcode in {"cmp", "cmn", "subs"}:
        return "compare"

    # --------------------------------------
    # Conditional branches
    # --------------------------------------

    branch_map = {
        "b.eq": "branch_eq",
        "b.ne": "branch_ne",
        "b.gt": "branch_gt",
        "b.ge": "branch_ge",
        "b.lt": "branch_lt",
        "b.le": "branch_le",
    }

    if opcode in branch_map:
        return branch_map[opcode]

    # --------------------------------------
    # Bit-test branches
    # --------------------------------------

    if opcode == "tbz":
        return "test_bit_zero"

    if opcode == "tbnz":
        return "test_bit_nonzero"

    # --------------------------------------
    # Conditional set
    # --------------------------------------

    if opcode in {"cset", "csetm"}:

        condition = parts[-1].lower()

        cset_map = {
            "eq": "cset_eq",
            "ne": "cset_ne",
            "gt": "cset_gt",
            "ge": "cset_ge",
            "lt": "cset_lt",
            "le": "cset_le",
        }

        return cset_map.get(
            condition,
            "other",
        )

    # --------------------------------------
    # Other control flow
    # --------------------------------------

    if opcode in {"b", "br"}:
        return "unconditional_branch"

    if opcode in {"bl", "blr"}:
        return "call"

    if opcode == "ret":
        return "return"

    # --------------------------------------
    # Move
    # --------------------------------------

    if opcode in {"mov", "movz", "movk"}:
        return "move"

    return "other"


def detect_semantic_predicates(instructions):

    predicates = []

    normalized = [
        inst.strip().lower()
        for inst in instructions
    ]

    for i, inst in enumerate(normalized):

        # --------------------------------------------------
        # Case 1:
        # subs ..., #0
        # cset ..., gt
        # --------------------------------------------------

        if inst.startswith("subs") and "#0x0" in inst:

            if i + 1 < len(normalized):

                next_inst = normalized[i + 1]

                if next_inst.startswith("cset"):

                    parts = next_inst.split()

                    if parts[-1] == "gt":
                        predicates.append(
                            "predicate_gt_zero"
                        )

                    elif parts[-1] == "lt":
                        predicates.append(
                            "predicate_lt_zero"
                        )

                # ------------------------------------------
                # Branch-based implementation
                #
                # b.le false  => true condition is > 0
                # b.ge false  => true condition is < 0
                # ------------------------------------------

                elif next_inst.startswith("b.le"):

                    predicates.append(
                        "predicate_gt_zero"
                    )

                elif next_inst.startswith("b.ge"):

                    predicates.append(
                        "predicate_lt_zero"
                    )

        # --------------------------------------------------
        # ARM sign-bit test:
        #
        # tbz w8, #0x1f, target
        #
        # If sign bit is zero, value is non-negative.
        # In our is_negative() pattern, fall-through means
        # negative.
        # --------------------------------------------------

        if inst.startswith("tbz") and "#0x1f" in inst:

            predicates.append(
                "predicate_lt_zero"
            )

    return predicates

# --------------------------------------------------
# 4. Extract feature vector
# --------------------------------------------------

def extract_feature_vector(instructions):

    categories = [
        categorize_instruction(inst)
        for inst in instructions
    ]

    predicates = detect_semantic_predicates(
        instructions
    )

    all_features = (
        categories + predicates
    )

    counts = Counter(all_features)

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

output_file = output_dir / "deepclone_baseline_v4_results.csv"

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