import csv
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[1]

INPUT_MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_manifest.csv"
)

OUTPUT_MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_w1_split_v2.csv"
)


TRAIN_FAMILIES = {
    "buffer_index",
    "division",
    "format",
    "integer_overflow",
    "null_pointer",
    "signed_range",
    "text_copy",
}

DEV_FAMILIES = {
    "array_bounds",
    "buffer_copy",
}


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


output_rows = []


for row in rows:

    family = row["family"]

    if family in TRAIN_FAMILIES:
        split = "train"

    elif family in DEV_FAMILIES:
        split = "dev"

    else:
        raise RuntimeError(
            f"Family is not assigned: {family}"
        )

    output_rows.append({
        "sample_id": row["sample_id"],
        "family": family,
        "function_name": row["function_name"],
        "optimization": row["optimization"],
        "label": row["label"],
        "split": split,
    })


overlap = TRAIN_FAMILIES & DEV_FAMILIES

if overlap:
    raise RuntimeError(
        f"Family leakage detected: {sorted(overlap)}"
    )


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


def count_labels(split_rows):

    vulnerable = sum(
        row["label"] == 1
        for row in split_rows
    )

    safe = sum(
        row["label"] == 0
        for row in split_rows
    )

    return vulnerable, safe


train_vuln, train_safe = count_labels(train_rows)
dev_vuln, dev_safe = count_labels(dev_rows)


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
    writer.writerows(output_rows)


print("=== Mini-DeepWeak W1 Split v2 ===")

print("\nTrain families:")
for family in sorted(TRAIN_FAMILIES):
    print(" ", family)

print("\nDev families:")
for family in sorted(DEV_FAMILIES):
    print(" ", family)

print("\nTrain samples:", len(train_rows))
print("  Vulnerable:", train_vuln)
print("  Non-vulnerable:", train_safe)

print("\nDev samples:", len(dev_rows))
print("  Vulnerable:", dev_vuln)
print("  Non-vulnerable:", dev_safe)

print("\nFamily overlap:", len(overlap))
print(
    "Saved to:",
    OUTPUT_MANIFEST.relative_to(PROJECT_ROOT),
)