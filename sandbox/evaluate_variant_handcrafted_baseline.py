import csv

import torch
import torch.nn.functional as F

from sklearn.metrics import (
    accuracy_score,
    precision_score,
    recall_score,
    f1_score,
)

from acfg_builder import build_pyg_graph


MANIFEST_PATH = "results/variant_pair_manifest.csv"


VARIANT_BINARIES = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}


THRESHOLD = 0.5


# --------------------------------------------------
# 1. Load manifest
# --------------------------------------------------

rows = []

with open(MANIFEST_PATH, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        row["label"] = int(
            row["label"]
        )

        rows.append(row)


test_rows = [
    row
    for row in rows
    if row["split"] == "test"
]


print("=== Handcrafted Baseline on Variant Test Set ===")

print(
    "Test pairs:",
    len(test_rows),
)


# --------------------------------------------------
# 2. Parse sample ID
# --------------------------------------------------

def parse_sample_id(sample_id):

    family, optimization = (
        sample_id.rsplit("_", 1)
    )

    return family, optimization


# --------------------------------------------------
# 3. Build graph cache
# --------------------------------------------------

sample_ids = sorted(
    {
        row[key]
        for row in test_rows
        for key in [
            "sample_a",
            "sample_b",
        ]
    }
)


graphs = {}


for sample_id in sample_ids:

    family, optimization = (
        parse_sample_id(
            sample_id
        )
    )

    binary_path = (
        VARIANT_BINARIES[
            optimization
        ]
    )

    graph = build_pyg_graph(
        binary_path,
        family,
    )

    graphs[sample_id] = graph


# --------------------------------------------------
# 4. Convert A-CFG into handcrafted
#    graph-level feature vector
# --------------------------------------------------

def graph_feature_vector(graph):

    # Sum keeps overall instruction/category counts
    # across all basic blocks.
    vector = graph.x.sum(
        dim=0,
        keepdim=True,
    )

    return vector


# --------------------------------------------------
# 5. Evaluate
# --------------------------------------------------

labels = []
scores = []


for row in test_rows:

    vector_a = graph_feature_vector(
        graphs[
            row["sample_a"]
        ]
    )

    vector_b = graph_feature_vector(
        graphs[
            row["sample_b"]
        ]
    )


    similarity = F.cosine_similarity(
        vector_a,
        vector_b,
    ).item()


    labels.append(
        row["label"]
    )

    scores.append(
        similarity
    )


predictions = [
    1 if score >= THRESHOLD
    else 0
    for score in scores
]


accuracy = accuracy_score(
    labels,
    predictions,
)

precision = precision_score(
    labels,
    predictions,
    zero_division=0,
)

recall = recall_score(
    labels,
    predictions,
    zero_division=0,
)

f1 = f1_score(
    labels,
    predictions,
    zero_division=0,
)


positive_scores = [
    score
    for score, label
    in zip(scores, labels)
    if label == 1
]

negative_scores = [
    score
    for score, label
    in zip(scores, labels)
    if label == 0
]


mean_positive = (
    sum(positive_scores)
    / len(positive_scores)
)

mean_negative = (
    sum(negative_scores)
    / len(negative_scores)
)

separation = (
    mean_positive
    - mean_negative
)


print("\n=== Test Results ===")

print(
    "Positive mean:",
    round(mean_positive, 4),
)

print(
    "Negative mean:",
    round(mean_negative, 4),
)

print(
    "Separation   :",
    round(separation, 4),
)

print(
    "Accuracy     :",
    round(accuracy, 4),
)

print(
    "Precision    :",
    round(precision, 4),
)

print(
    "Recall       :",
    round(recall, 4),
)

print(
    "F1           :",
    round(f1, 4),
)