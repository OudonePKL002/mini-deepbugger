import csv
import random
from pathlib import Path


SEED = 42

PROJECT_ROOT = Path(__file__).resolve().parents[1]

INPUT_MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_manifest.csv"
)

OUTPUT_MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_w1_split.csv"
)


random.seed(SEED)


# ============================================================
# 1. Load original W1 manifest
# ============================================================

rows = []

with INPUT_MANIFEST.open(
    "r",
    newline="",
    encoding="utf-8",
) as f:

    reader = csv.DictReader(f)

    for row in reader:
        row["label"] = int(row["label"])
        rows.append(row)


# ============================================================
# 2. Collect unique families
# ============================================================

families = sorted(
    {
        row["family"]
        for row in rows
    }
)


print(
    "=== Mini-DeepWeak W1 Family-Level Split ==="
)

print(
    "Total families:",
    len(families),
)

print(
    "Total samples:",
    len(rows),
)


# ============================================================
# 3. Deterministic family shuffle
# ============================================================

shuffled_families = list(families)

random.shuffle(
    shuffled_families
)


TRAIN_FAMILY_COUNT = 6


train_families = set(
    shuffled_families[
        :TRAIN_FAMILY_COUNT
    ]
)


dev_families = set(
    shuffled_families[
        TRAIN_FAMILY_COUNT:
    ]
)


# ============================================================
# 4. Assign split
# ============================================================

output_rows = []


for row in rows:

    if row["family"] in train_families:
        split = "train"

    elif row["family"] in dev_families:
        split = "dev"

    else:
        raise RuntimeError(
            f"Unknown family: "
            f"{row['family']}"
        )


    output_rows.append(
        {
            "sample_id":
                row["sample_id"],

            "family":
                row["family"],

            "function_name":
                row["function_name"],

            "optimization":
                row["optimization"],

            "label":
                row["label"],

            "split":
                split,
        }
    )


# ============================================================
# 5. Validate family leakage
# ============================================================

overlap = (
    train_families
    & dev_families
)


if overlap:

    raise RuntimeError(
        "Family leakage detected: "
        f"{sorted(overlap)}"
    )


# ============================================================
# 6. Statistics
# ============================================================

train_rows = [
    row
    for row in output_rows
    if row["split"] == "train"
]


dev_rows = [
    row
    for row in output_rows
    if row["split"] == "dev"
]


def count_labels(
    split_rows,
):

    vulnerable = sum(
        row["label"] == 1
        for row in split_rows
    )

    non_vulnerable = sum(
        row["label"] == 0
        for row in split_rows
    )

    return (
        vulnerable,
        non_vulnerable,
    )


train_vuln, train_safe = (
    count_labels(
        train_rows
    )
)


dev_vuln, dev_safe = (
    count_labels(
        dev_rows
    )
)


# ============================================================
# 7. Save manifest
# ============================================================

with OUTPUT_MANIFEST.open(
    "w",
    newline="",
    encoding="utf-8",
) as f:

    writer = csv.DictWriter(
        f,
        fieldnames=[
            "sample_id",
            "family",
            "function_name",
            "optimization",
            "label",
            "split",
        ],
    )

    writer.writeheader()

    writer.writerows(
        output_rows
    )


# ============================================================
# 8. Report
# ============================================================

print()

print(
    "Train families:",
    sorted(train_families),
)

print(
    "Dev families:",
    sorted(dev_families),
)


print()

print(
    "Train samples:",
    len(train_rows),
)

print(
    "  Vulnerable:",
    train_vuln,
)

print(
    "  Non-vulnerable:",
    train_safe,
)


print()

print(
    "Dev samples:",
    len(dev_rows),
)

print(
    "  Vulnerable:",
    dev_vuln,
)

print(
    "  Non-vulnerable:",
    dev_safe,
)


print()

print(
    "Family overlap:",
    len(overlap),
)

print(
    "Saved to:",
    OUTPUT_MANIFEST.relative_to(
        PROJECT_ROOT
    ),
)