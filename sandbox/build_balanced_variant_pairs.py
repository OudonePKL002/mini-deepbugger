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


TRAIN_FAMILIES = [
    "add",
    "subtract",
    "multiply",
    "maximum",
    "minimum",

    "is_zero",
    "is_nonzero",
    "greater_than_ten",
    "sign_bit",
    "is_sum_positive",
]

TEST_FAMILIES = [
    "is_positive",
    "is_negative",
    "identity",
]


# --------------------------------------------------
# Create sample IDs
# --------------------------------------------------

def samples_for_family(family):
    return [
        f"{family}_{opt}"
        for opt in OPT_LEVELS
    ]


# --------------------------------------------------
# Positive pairs
# Same family, different optimization levels
# --------------------------------------------------

def build_positive_pairs(families):

    pairs = []

    for family in families:

        samples = samples_for_family(
            family
        )

        for a, b in combinations(
            samples,
            2,
        ):

            pairs.append({
                "sample_a": a,
                "sample_b": b,
                "family_a": family,
                "family_b": family,
                "label": 1,
            })

    return pairs


# --------------------------------------------------
# Candidate negative pairs
# Different function families
# --------------------------------------------------

def build_negative_candidates(families):

    candidates = []

    for family_a, family_b in combinations(
        families,
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

                candidates.append({
                    "sample_a": sample_a,
                    "sample_b": sample_b,
                    "family_a": family_a,
                    "family_b": family_b,
                    "label": 0,
                })

    return candidates


# --------------------------------------------------
# Build balanced split
# --------------------------------------------------

def build_split(
    split_name,
    families,
):

    positives = build_positive_pairs(
        families
    )

    negative_candidates = (
        build_negative_candidates(
            families
        )
    )

    random.shuffle(
        negative_candidates
    )

    # Same number of negatives as positives
    negatives = negative_candidates[
        :len(positives)
    ]

    rows = []

    for item in positives + negatives:

        row = {
            "split": split_name,
            **item,
        }

        rows.append(row)

    random.shuffle(rows)

    return rows


train_rows = build_split(
    "train",
    TRAIN_FAMILIES,
)

test_rows = build_split(
    "test",
    TEST_FAMILIES,
)


all_rows = (
    train_rows
    + test_rows
)


# --------------------------------------------------
# Save CSV
# --------------------------------------------------

output_dir = Path("results")
output_dir.mkdir(
    exist_ok=True
)

output_file = (
    output_dir
    / "variant_pair_manifest.csv"
)


with open(
    output_file,
    "w",
    newline="",
) as f:

    writer = csv.DictWriter(
        f,
        fieldnames=[
            "split",
            "sample_a",
            "sample_b",
            "family_a",
            "family_b",
            "label",
        ],
    )

    writer.writeheader()
    writer.writerows(
        all_rows
    )


# --------------------------------------------------
# Statistics
# --------------------------------------------------

def print_stats(
    name,
    rows,
):

    positive_count = sum(
        row["label"] == 1
        for row in rows
    )

    negative_count = sum(
        row["label"] == 0
        for row in rows
    )

    print(f"\n{name}")

    print(
        "Total:",
        len(rows),
    )

    print(
        "Positive:",
        positive_count,
    )

    print(
        "Negative:",
        negative_count,
    )


print(
    "=== Mini-DeepClone Balanced Variant Pair Dataset ==="
)

print_stats(
    "TRAIN",
    train_rows,
)

print_stats(
    "TEST",
    test_rows,
)

print(
    "\nTotal pairs:",
    len(all_rows),
)

print(
    "\nSaved to:",
    output_file,
)