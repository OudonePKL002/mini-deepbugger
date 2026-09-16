import csv
import sys
from pathlib import Path

import torch


PROJECT_ROOT = Path(__file__).resolve().parents[1]

if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))


from src.deepweak_graph_builder import (
    build_deepweak_graph,
)

from src.deepweak_features import (
    SECURITY_FEATURE_ORDER,
)


MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_w1_split.csv"
)


BINARIES = {
    "O0": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O0",
    "O1": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O1",
    "O2": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O2",
    "O3": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O3",
}


# ============================================================
# Load manifest
# ============================================================

rows = []

with MANIFEST.open(
    "r",
    newline="",
    encoding="utf-8",
) as f:

    reader = csv.DictReader(f)

    for row in reader:
        row["label"] = int(row["label"])
        rows.append(row)


# ============================================================
# Build graph cache
# ============================================================

graphs = {}


for row in rows:

    sample_id = row["sample_id"]

    graphs[sample_id] = build_deepweak_graph(
        str(
            BINARIES[
                row["optimization"]
            ]
        ),
        row["function_name"],
    )


# ============================================================
# Security feature coverage
# ============================================================

def get_security_presence(
    split_rows,
):

    counts = torch.zeros(
        len(SECURITY_FEATURE_ORDER),
        dtype=torch.int64,
    )


    for row in split_rows:

        graph = graphs[
            row["sample_id"]
        ]

        security = graph.x[:, 29:]


        # Feature is considered present if it appears
        # in at least one node of this graph.
        present = (
            security.sum(dim=0) > 0
        ).long()


        counts += present


    return counts


train_rows = [
    row
    for row in rows
    if row["split"] == "train"
]


dev_rows = [
    row
    for row in rows
    if row["split"] == "dev"
]


train_counts = get_security_presence(
    train_rows
)

dev_counts = get_security_presence(
    dev_rows
)


print(
    "=== Mini-DeepWeak Security Feature Coverage ==="
)

print()

print(
    f"{'Feature':30} "
    f"{'Train':>8} "
    f"{'Dev':>8}"
)

print("-" * 50)


for i, name in enumerate(
    SECURITY_FEATURE_ORDER
):

    print(
        f"{name:30} "
        f"{train_counts[i].item():8d} "
        f"{dev_counts[i].item():8d}"
    )


# ============================================================
# Dev-only features
# ============================================================

print()
print(
    "=== DEV-ONLY SECURITY FEATURES ==="
)


dev_only = []


for i, name in enumerate(
    SECURITY_FEATURE_ORDER
):

    if (
        train_counts[i] == 0
        and dev_counts[i] > 0
    ):

        dev_only.append(name)

        print(
            f"{name}: "
            f"train=0, "
            f"dev={dev_counts[i].item()}"
        )


if not dev_only:

    print(
        "No security feature is exclusive "
        "to the dev set."
    )


# ============================================================
# Per-family coverage
# ============================================================

print()
print(
    "=== Per-Family Security Features ==="
)


families = sorted({
    row["family"]
    for row in rows
})


for family in families:

    family_rows = [
        row
        for row in rows
        if row["family"] == family
    ]


    active = set()


    for row in family_rows:

        graph = graphs[
            row["sample_id"]
        ]

        security = graph.x[:, 29:]

        present = (
            security.sum(dim=0) > 0
        )


        for i, value in enumerate(
            present.tolist()
        ):

            if value:
                active.add(
                    SECURITY_FEATURE_ORDER[i]
                )


    split = family_rows[0]["split"]

    print(
        f"\n{family} [{split}]"
    )


    if not active:

        print("  (none)")

    else:

        for feature in sorted(active):
            print(
                " ",
                feature
            )