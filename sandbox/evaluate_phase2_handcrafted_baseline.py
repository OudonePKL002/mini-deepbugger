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


PHASE2_MANIFEST = (
    "results/phase2_independent_manifest.csv"
)


VARIANT_BINARIES = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}


THRESHOLD = 0.5


# --------------------------------------------------
# 1. Load Phase-2 manifest
# --------------------------------------------------

rows = []

with open(
    PHASE2_MANIFEST,
    newline="",
) as f:

    reader = csv.DictReader(f)

    for row in reader:

        row["label"] = int(
            row["label"]
        )

        rows.append(row)


print(
    "=== Phase-2 Handcrafted A-CFG Baseline ==="
)

print(
    "Evaluation pairs:",
    len(rows),
)


# --------------------------------------------------
# 2. Parse sample ID
# --------------------------------------------------

def parse_sample_id(sample_id):

    family, optimization = (
        sample_id.rsplit("_", 1)
    )

    return (
        family,
        optimization,
    )


# --------------------------------------------------
# 3. Build graph cache
# --------------------------------------------------

sample_ids = sorted(
    {
        row[key]
        for row in rows
        for key in [
            "sample_a",
            "sample_b",
        ]
    }
)


graphs = {}


print(
    "\n=== Building Graph Cache ==="
)


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


print(
    "Graph samples:",
    len(graphs),
)


# --------------------------------------------------
# 4. Graph-level handcrafted representation
#
# Sum all basic-block semantic feature vectors.
# --------------------------------------------------

def graph_feature_vector(graph):

    return graph.x.sum(
        dim=0,
        keepdim=True,
    )


# --------------------------------------------------
# 5. Evaluate
# --------------------------------------------------

labels = []
scores = []


for row in rows:

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


    score = F.cosine_similarity(
        vector_a,
        vector_b,
    ).item()


    labels.append(
        row["label"]
    )

    scores.append(
        score
    )


predictions = [
    1 if score >= THRESHOLD
    else 0
    for score in scores
]


# --------------------------------------------------
# 6. Classification metrics
# --------------------------------------------------

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


# --------------------------------------------------
# 7. Similarity separation
# --------------------------------------------------

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


positive_mean = (
    sum(positive_scores)
    / len(positive_scores)
)

negative_mean = (
    sum(negative_scores)
    / len(negative_scores)
)

separation = (
    positive_mean
    - negative_mean
)


# --------------------------------------------------
# 8. Confusion counts
# --------------------------------------------------

tp = sum(
    pred == 1 and label == 1
    for pred, label
    in zip(predictions, labels)
)

fp = sum(
    pred == 1 and label == 0
    for pred, label
    in zip(predictions, labels)
)

fn = sum(
    pred == 0 and label == 1
    for pred, label
    in zip(predictions, labels)
)

tn = sum(
    pred == 0 and label == 0
    for pred, label
    in zip(predictions, labels)
)


# --------------------------------------------------
# 9. Results
# --------------------------------------------------

print(
    "\n=== Phase-2 Test Results ==="
)

print(
    "Positive mean:",
    round(
        positive_mean,
        4,
    ),
)

print(
    "Negative mean:",
    round(
        negative_mean,
        4,
    ),
)

print(
    "Separation   :",
    round(
        separation,
        4,
    ),
)

print()

print(
    "Accuracy     :",
    round(
        accuracy,
        4,
    ),
)

print(
    "Precision    :",
    round(
        precision,
        4,
    ),
)

print(
    "Recall       :",
    round(
        recall,
        4,
    ),
)

print(
    "F1           :",
    round(
        f1,
        4,
    ),
)

print()

print(
    "TP:",
    tp,
)

print(
    "FP:",
    fp,
)

print(
    "FN:",
    fn,
)

print(
    "TN:",
    tn,
)