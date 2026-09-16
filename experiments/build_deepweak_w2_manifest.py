import csv
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[1]

OUTPUT = (
    PROJECT_ROOT
    / "data"
    / "deepweak_w2_manifest.csv"
)


FAMILIES = [
    "safe_divide_alt",
    "pointer_write",
    "index_write",
    "length_copy",
    "multiplication_guard",
    "format_wrapper",
]


OPT_LEVELS = [
    "O0",
    "O1",
    "O2",
    "O3",
]


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

            rows.append({
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
            })


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


vulnerable = sum(
    row["label"] == 1
    for row in rows
)

safe = sum(
    row["label"] == 0
    for row in rows
)


print(
    "=== Mini-DeepWeak W2 Independent Manifest ==="
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
    vulnerable,
)

print(
    "Non-vulnerable:",
    safe,
)

print(
    "\nSaved to:",
    OUTPUT.relative_to(
        PROJECT_ROOT
    ),
)