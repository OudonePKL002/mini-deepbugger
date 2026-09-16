import csv
import sys
from pathlib import Path

import torch.nn.functional as F


PROJECT_ROOT = Path(__file__).resolve().parents[1]

if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))


from src.acfg_builder import build_pyg_graph


MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_manifest.csv"
)


BINARIES = {
    "O0": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O0",
    "O1": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O1",
    "O2": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O2",
    "O3": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O3",
}


# --------------------------------------------------
# Load manifest
# --------------------------------------------------

rows = []

with MANIFEST.open(
    "r",
    newline="",
    encoding="utf-8",
) as f:

    reader = csv.DictReader(f)
    rows.extend(reader)


families = sorted(
    {
        row["family"]
        for row in rows
    }
)


# --------------------------------------------------
# Build graph cache
# --------------------------------------------------

graphs = {}


for row in rows:

    sample_id = row["sample_id"]

    graph = build_pyg_graph(
        str(
            BINARIES[
                row["optimization"]
            ]
        ),
        row["function_name"],
    )

    graphs[sample_id] = graph


# --------------------------------------------------
# Convert graph into direct handcrafted vector
#
# Same idea as Mini-DeepClone baseline:
# sum basic-block semantic features
# --------------------------------------------------

def graph_vector(graph):

    return graph.x.sum(
        dim=0,
        keepdim=True,
    )


# --------------------------------------------------
# Compare bad vs good within same family / opt level
# --------------------------------------------------

print(
    "=== Mini-DeepWeak Bad-vs-Good Inspection ==="
)

print()


all_scores = []


for family in families:

    print(
        f"\n--- {family} ---"
    )


    for opt in [
        "O0",
        "O1",
        "O2",
        "O3",
    ]:

        bad_id = (
            f"{family}_bad_{opt}"
        )

        good_id = (
            f"{family}_good_{opt}"
        )


        bad_graph = graphs[
            bad_id
        ]

        good_graph = graphs[
            good_id
        ]


        bad_vector = graph_vector(
            bad_graph
        )

        good_vector = graph_vector(
            good_graph
        )


        similarity = (
            F.cosine_similarity(
                bad_vector,
                good_vector,
            ).item()
        )


        all_scores.append(
            {
                "family": family,
                "optimization": opt,
                "similarity": similarity,
            }
        )


        print(
            f"{opt}  "
            f"bad(nodes={bad_graph.num_nodes}, "
            f"edges={bad_graph.num_edges})  "
            f"good(nodes={good_graph.num_nodes}, "
            f"edges={good_graph.num_edges})  "
            f"similarity={similarity:.4f}"
        )


# --------------------------------------------------
# Summary
# --------------------------------------------------

print()
print(
    "=== Similarity Summary ==="
)


scores = [
    row["similarity"]
    for row in all_scores
]


mean_similarity = (
    sum(scores)
    / len(scores)
)


print(
    "Pairs:",
    len(scores),
)

print(
    "Mean bad-good similarity:",
    round(
        mean_similarity,
        4,
    ),
)

print(
    "Minimum similarity:",
    round(
        min(scores),
        4,
    ),
)

print(
    "Maximum similarity:",
    round(
        max(scores),
        4,
    ),
)


# --------------------------------------------------
# Potentially difficult cases
# --------------------------------------------------

print()
print(
    "=== High-Similarity Bad/Good Cases ==="
)


hard_cases = [
    row
    for row in all_scores
    if row["similarity"] >= 0.90
]


if not hard_cases:

    print(
        "No bad/good pair has "
        "similarity >= 0.90."
    )

else:

    for row in sorted(
        hard_cases,
        key=lambda x: x["similarity"],
        reverse=True,
    ):

        print(
            f"{row['family']:<20} "
            f"{row['optimization']} "
            f"similarity="
            f"{row['similarity']:.4f}"
        )