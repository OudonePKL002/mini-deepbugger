import csv
from pathlib import Path


FAMILIES = [
    "buffer_copy",
    "buffer_index",
    "integer_overflow",
    "null_pointer",
    "division",
    "format",
    "signed_range",
    "array_bounds",
    "text_copy",
]


OPT_LEVELS = [
    "O0",
    "O1",
    "O2",
    "O3",
]


OUTPUT = Path(
    "data/deepweak_manifest.csv"
)


rows = []


for family in FAMILIES:

    for variant, label in [
        ("bad", 1),
        ("good", 0),
    ]:

        function_name = (
            f"{family}_{variant}"
        )

        for opt in OPT_LEVELS:

            rows.append(
                {
                    "sample_id":
                        f"{function_name}_{opt}",

                    "family":
                        family,

                    "function_name":
                        function_name,

                    "optimization":
                        opt,

                    "label":
                        label,
                }
            )


with OUTPUT.open(
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
        ],
    )

    writer.writeheader()
    writer.writerows(rows)


positive = sum(
    row["label"] == 1
    for row in rows
)

negative = sum(
    row["label"] == 0
    for row in rows
)


print(
    "=== Mini-DeepWeak Manifest ==="
)

print(
    "Families:",
    len(FAMILIES),
)

print(
    "Samples:",
    len(rows),
)

print(
    "Vulnerable:",
    positive,
)

print(
    "Non-vulnerable:",
    negative,
)

print(
    "\nSaved to:",
    OUTPUT,
)