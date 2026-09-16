import csv
import sys
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[1]

if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))


from src.deepweak_graph_builder import build_deepweak_graph


W1_MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_w1_split_v2.csv"
)

W2_MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_w2_manifest.csv"
)


W2_BINARIES = {
    "O0": PROJECT_ROOT / "sandbox/deepweak/w2_weaknesses_O0",
    "O1": PROJECT_ROOT / "sandbox/deepweak/w2_weaknesses_O1",
    "O2": PROJECT_ROOT / "sandbox/deepweak/w2_weaknesses_O2",
    "O3": PROJECT_ROOT / "sandbox/deepweak/w2_weaknesses_O3",
}


def load_csv(path):

    rows = []

    with path.open(
        "r",
        newline="",
        encoding="utf-8",
    ) as f:

        reader = csv.DictReader(f)

        for row in reader:
            row["label"] = int(row["label"])
            rows.append(row)

    return rows


w1_rows = load_csv(W1_MANIFEST)
w2_rows = load_csv(W2_MANIFEST)


print(
    "=== Mini-DeepWeak W2 Independent Sanity Check ==="
)


# ============================================================
# 1. Family overlap
# ============================================================

w1_families = {
    row["family"]
    for row in w1_rows
}

w2_families = {
    row["family"]
    for row in w2_rows
}


overlap = (
    w1_families
    & w2_families
)


print()
print(
    "W1 families:",
    len(w1_families),
)

print(
    "W2 families:",
    len(w2_families),
)

print(
    "Family overlap:",
    len(overlap),
)


if overlap:

    raise RuntimeError(
        f"W1/W2 family leakage detected: "
        f"{sorted(overlap)}"
    )


print(
    "W1/W2 leakage check: OK"
)


# ============================================================
# 2. Class balance
# ============================================================

positive = sum(
    row["label"] == 1
    for row in w2_rows
)

negative = sum(
    row["label"] == 0
    for row in w2_rows
)


print()
print(
    "W2 samples:",
    len(w2_rows),
)

print(
    "Vulnerable:",
    positive,
)

print(
    "Non-vulnerable:",
    negative,
)


if len(w2_rows) != 48:
    raise RuntimeError(
        "Expected 48 W2 samples."
    )

if positive != 24:
    raise RuntimeError(
        "Expected 24 vulnerable samples."
    )

if negative != 24:
    raise RuntimeError(
        "Expected 24 non-vulnerable samples."
    )


# ============================================================
# 3. Build 39-D graph cache
# ============================================================

print()
print(
    "=== Building W2 39-D Graphs ==="
)


invalid = []
graphs = {}


for row in w2_rows:

    sample_id = row["sample_id"]

    graph = build_deepweak_graph(
        str(
            W2_BINARIES[
                row["optimization"]
            ]
        ),
        row["function_name"],
    )


    graphs[sample_id] = graph


    valid = (
        graph.num_nodes > 0
        and graph.num_node_features == 39
    )


    if not valid:
        invalid.append(sample_id)


    print(
        f"{sample_id:36} "
        f"nodes={graph.num_nodes:<3} "
        f"edges={graph.num_edges:<3} "
        f"features={graph.num_node_features:<3} "
        f"{'OK' if valid else 'INVALID'}"
    )


print()
print(
    "=== Summary ==="
)

print(
    "Total W2 graphs:",
    len(graphs),
)

print(
    "Invalid graphs:",
    len(invalid),
)


if invalid:

    print(
        "\nInvalid samples:"
    )

    for sample_id in invalid:
        print(sample_id)

else:

    print(
        "\nAll W2 graphs are valid."
    )

    print(
        "W2 is ready for independent evaluation."
    )