import csv
import random
from itertools import combinations
from pathlib import Path


SEED = 42
random.seed(SEED)


OPT_LEVELS = [
    "O0",
    "O1",
    "O2",
    "O3",
]


PHASE2_FAMILIES = [
    "absolute_value",
    "square",
    "is_even",
    "is_odd",
    "average_two",
    "max_three",
    "min_three",
    "in_range",
    "clamp_zero_hundred",
    "sum_three",
]


# --------------------------------------------------
# Samples for one family
# --------------------------------------------------

def samples_for_family(family):

    return [
        f"{family}_{opt}"
        for opt in OPT_LEVELS
    ]


# --------------------------------------------------
# Positive pairs:
# same function family,
# different optimization levels
# --------------------------------------------------

positive_pairs = []


for family in PHASE2_FAMILIES:

    samples = samples_for_family(
        family
    )

    for a, b in combinations(
        samples,
        2,
    ):

        positive_pairs.append({
            "sample_a": a,
            "sample_b": b,
            "family_a": family,
            "family_b": family,
            "label": 1,
        })


# --------------------------------------------------
# Negative candidates:
# different function families
# --------------------------------------------------

negative_candidates = []


for family_a, family_b in combinations(
    PHASE2_FAMILIES,
    2,
):

    samples_a = samples_for_family(
        family_a
    )

    samples_b = samples_for_family(
        family_b
    )

    for sample_a in samples_a:

        for sample_b in samples_b:

            negative_candidates.append({
                "sample_a": sample_a,
                "sample_b": sample_b,
                "family_a": family_a,
                "family_b": family_b,
                "label": 0,
            })


random.shuffle(
    negative_candidates
)


# Balanced evaluation:
# 60 positive + 60 negative
negative_pairs = negative_candidates[
    :len(positive_pairs)
]


rows = (
    positive_pairs
    + negative_pairs
)


random.shuffle(rows)


# --------------------------------------------------
# Save
# --------------------------------------------------

output_dir = Path("results")
output_dir.mkdir(exist_ok=True)

output_file = (
    output_dir
    / "phase2_independent_manifest.csv"
)


with open(
    output_file,
    "w",
    newline="",
) as f:

    writer = csv.DictWriter(
        f,
        fieldnames=[
            "sample_a",
            "sample_b",
            "family_a",
            "family_b",
            "label",
        ],
    )

    writer.writeheader()
    writer.writerows(rows)


print(
    "=== Phase-2 Independent Evaluation Dataset ==="
)

print(
    "Families:",
    len(PHASE2_FAMILIES),
)

print(
    "Graph samples:",
    len(PHASE2_FAMILIES)
    * len(OPT_LEVELS),
)

print(
    "Positive pairs:",
    len(positive_pairs),
)

print(
    "Negative pairs:",
    len(negative_pairs),
)

print(
    "Total pairs:",
    len(rows),
)

print(
    "\nSaved to:",
    output_file,
)