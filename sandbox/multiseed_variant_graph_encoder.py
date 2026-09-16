import csv
import random

import numpy as np
import torch
import torch.nn.functional as F

from sklearn.metrics import (
    accuracy_score,
    precision_score,
    recall_score,
    f1_score,
)

from acfg_builder import build_pyg_graph
from graph_encoder_demo import GraphEncoder


MANIFEST_PATH = "results/variant_pair_manifest.csv"

VARIANT_BINARIES = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}

SEEDS = [
    42,
    123,
    2026,
    7,
    99,
]

EPOCHS = 100
THRESHOLD = 0.5


# --------------------------------------------------
# 1. Load manifest
# --------------------------------------------------

rows = []

with open(MANIFEST_PATH, newline="") as f:
    reader = csv.DictReader(f)

    for row in reader:
        row["label"] = int(row["label"])
        rows.append(row)


train_rows = [
    row
    for row in rows
    if row["split"] == "train"
]

test_rows = [
    row
    for row in rows
    if row["split"] == "test"
]


# --------------------------------------------------
# 2. Parse sample ID
# --------------------------------------------------

def parse_sample_id(sample_id):
    family, optimization = sample_id.rsplit("_", 1)
    return family, optimization


# --------------------------------------------------
# 3. Build graph cache once
# --------------------------------------------------

sample_ids = sorted(
    {
        row[key]
        for row in rows
        for key in ["sample_a", "sample_b"]
    }
)


graphs = {}


print("=== Building Graph Cache ===")


for sample_id in sample_ids:

    family, optimization = parse_sample_id(
        sample_id
    )

    binary_path = VARIANT_BINARIES[
        optimization
    ]

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
# 4. Run one seed
# --------------------------------------------------

def run_seed(seed):

    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)

    model = GraphEncoder(
        input_dim=29,
        hidden_dim=32,
        embedding_dim=16,
    )

    optimizer = torch.optim.Adam(
        model.parameters(),
        lr=0.01,
    )

    criterion = torch.nn.CosineEmbeddingLoss(
        margin=0.5
    )


    def encode_graph(graph):

        batch = torch.zeros(
            graph.num_nodes,
            dtype=torch.long,
        )

        return model(
            graph.x,
            graph.edge_index,
            batch,
        )


    def score_pair(sample_a, sample_b):

        emb_a = encode_graph(
            graphs[sample_a]
        )

        emb_b = encode_graph(
            graphs[sample_b]
        )

        return F.cosine_similarity(
            emb_a,
            emb_b,
        )


    # ----------------------------------------------
    # Train
    # ----------------------------------------------

    local_train_rows = list(train_rows)

    for epoch in range(EPOCHS):

        model.train()

        random.shuffle(
            local_train_rows
        )

        for row in local_train_rows:

            optimizer.zero_grad()

            emb_a = encode_graph(
                graphs[row["sample_a"]]
            )

            emb_b = encode_graph(
                graphs[row["sample_b"]]
            )

            target_value = (
                1.0
                if row["label"] == 1
                else -1.0
            )

            target = torch.tensor(
                [target_value],
                dtype=torch.float32,
            )

            loss = criterion(
                emb_a,
                emb_b,
                target,
            )

            loss.backward()
            optimizer.step()


    # ----------------------------------------------
    # Test
    # ----------------------------------------------

    model.eval()

    labels = []
    scores = []

    with torch.no_grad():

        for row in test_rows:

            score = score_pair(
                row["sample_a"],
                row["sample_b"],
            )

            labels.append(
                row["label"]
            )

            scores.append(
                score.item()
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


    mean_positive = np.mean(
        positive_scores
    )

    mean_negative = np.mean(
        negative_scores
    )

    separation = (
        mean_positive
        - mean_negative
    )


    return {
        "seed": seed,
        "accuracy": accuracy,
        "precision": precision,
        "recall": recall,
        "f1": f1,
        "positive_mean": mean_positive,
        "negative_mean": mean_negative,
        "separation": separation,
    }


# --------------------------------------------------
# 5. Run all seeds
# --------------------------------------------------

results = []


print("\n=== Multi-seed Evaluation ===")


for seed in SEEDS:

    result = run_seed(seed)

    results.append(result)

    print(
        f"\nSeed {seed}"
    )

    print(
        "Accuracy :",
        round(result["accuracy"], 4),
    )

    print(
        "Precision:",
        round(result["precision"], 4),
    )

    print(
        "Recall   :",
        round(result["recall"], 4),
    )

    print(
        "F1       :",
        round(result["f1"], 4),
    )

    print(
        "Separation:",
        round(result["separation"], 4),
    )


# --------------------------------------------------
# 6. Mean ± std
# --------------------------------------------------

metrics = [
    "accuracy",
    "precision",
    "recall",
    "f1",
    "separation",
]


print("\n=== Summary: Mean ± Std ===")


for metric in metrics:

    values = np.array(
        [
            result[metric]
            for result in results
        ],
        dtype=float,
    )

    mean = values.mean()
    std = values.std(ddof=1)

    print(
        f"{metric:10}: "
        f"{mean:.4f} ± {std:.4f}"
    )


# --------------------------------------------------
# 7. Save CSV
# --------------------------------------------------

output_file = (
    "results/"
    "multiseed_variant_graph_results.csv"
)


with open(
    output_file,
    "w",
    newline="",
) as f:

    writer = csv.DictWriter(
        f,
        fieldnames=[
            "seed",
            "accuracy",
            "precision",
            "recall",
            "f1",
            "positive_mean",
            "negative_mean",
            "separation",
        ],
    )

    writer.writeheader()
    writer.writerows(results)


print(
    "\nSaved to:",
    output_file,
)