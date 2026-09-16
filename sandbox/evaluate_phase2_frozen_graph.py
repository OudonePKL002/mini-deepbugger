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


# ==================================================
# Configuration
# ==================================================

PHASE1_MANIFEST = (
    "results/variant_pair_manifest.csv"
)

PHASE2_MANIFEST = (
    "results/phase2_independent_manifest.csv"
)


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


# ==================================================
# 1. Load Phase-1 training pairs
# ==================================================

phase1_rows = []

with open(
    PHASE1_MANIFEST,
    newline="",
) as f:

    reader = csv.DictReader(f)

    for row in reader:

        row["label"] = int(
            row["label"]
        )

        phase1_rows.append(row)


train_rows = [
    row
    for row in phase1_rows
    if row["split"] == "train"
]


print(
    "=== Phase-2 Frozen Graph Evaluation ==="
)

print(
    "\nPhase-1 training pairs:",
    len(train_rows),
)


# ==================================================
# 2. Load Phase-2 independent evaluation pairs
# ==================================================

phase2_rows = []

with open(
    PHASE2_MANIFEST,
    newline="",
) as f:

    reader = csv.DictReader(f)

    for row in reader:

        row["label"] = int(
            row["label"]
        )

        phase2_rows.append(row)


print(
    "Phase-2 evaluation pairs:",
    len(phase2_rows),
)


# ==================================================
# 3. Parse sample ID
#
# Example:
#
# is_sum_positive_O2
#
# ->
#
# family = is_sum_positive
# optimization = O2
# ==================================================

def parse_sample_id(sample_id):

    family, optimization = (
        sample_id.rsplit("_", 1)
    )

    return (
        family,
        optimization,
    )


# ==================================================
# 4. Build graph cache
#
# IMPORTANT:
# Graph creation does NOT train anything.
# ==================================================

all_sample_ids = set()


for row in train_rows:

    all_sample_ids.add(
        row["sample_a"]
    )

    all_sample_ids.add(
        row["sample_b"]
    )


for row in phase2_rows:

    all_sample_ids.add(
        row["sample_a"]
    )

    all_sample_ids.add(
        row["sample_b"]
    )


all_sample_ids = sorted(
    all_sample_ids
)


graphs = {}


print(
    "\n=== Building Graph Cache ==="
)


for sample_id in all_sample_ids:

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
    "Total cached graphs:",
    len(graphs),
)


# ==================================================
# 5. Run one seed
# ==================================================

def run_seed(seed):

    # ----------------------------------------------
    # Reproducibility
    # ----------------------------------------------

    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)


    # ----------------------------------------------
    # New Graph Encoder
    # ----------------------------------------------

    model = GraphEncoder(
        input_dim=29,
        hidden_dim=32,
        embedding_dim=16,
    )


    optimizer = torch.optim.Adam(
        model.parameters(),
        lr=0.01,
    )


    criterion = (
        torch.nn.CosineEmbeddingLoss(
            margin=0.5
        )
    )


    # ----------------------------------------------
    # Encode graph
    # ----------------------------------------------

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


    # ----------------------------------------------
    # Phase-1 TRAINING ONLY
    # ----------------------------------------------

    local_train_rows = list(
        train_rows
    )


    for epoch in range(EPOCHS):

        model.train()

        random.shuffle(
            local_train_rows
        )

        for row in local_train_rows:

            optimizer.zero_grad()


            emb_a = encode_graph(
                graphs[
                    row["sample_a"]
                ]
            )


            emb_b = encode_graph(
                graphs[
                    row["sample_b"]
                ]
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


    # ==================================================
    # Phase-2 EVALUATION ONLY
    #
    # No optimizer
    # No backward()
    # No threshold tuning
    # ==================================================

    model.eval()


    labels = []
    scores = []


    with torch.no_grad():

        for row in phase2_rows:

            emb_a = encode_graph(
                graphs[
                    row["sample_a"]
                ]
            )

            emb_b = encode_graph(
                graphs[
                    row["sample_b"]
                ]
            )


            similarity = (
                F.cosine_similarity(
                    emb_a,
                    emb_b,
                ).item()
            )


            labels.append(
                row["label"]
            )

            scores.append(
                similarity
            )


    # ----------------------------------------------
    # Fixed classification threshold
    # ----------------------------------------------

    predictions = [
        1 if score >= THRESHOLD
        else 0
        for score in scores
    ]


    # ----------------------------------------------
    # Classification metrics
    # ----------------------------------------------

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


    # ----------------------------------------------
    # Embedding separation
    # ----------------------------------------------

    positive_scores = [
        score
        for score, label
        in zip(
            scores,
            labels,
        )
        if label == 1
    ]


    negative_scores = [
        score
        for score, label
        in zip(
            scores,
            labels,
        )
        if label == 0
    ]


    positive_mean = float(
        np.mean(
            positive_scores
        )
    )


    negative_mean = float(
        np.mean(
            negative_scores
        )
    )


    separation = (
        positive_mean
        - negative_mean
    )


    return {
        "seed": seed,

        "accuracy": accuracy,
        "precision": precision,
        "recall": recall,
        "f1": f1,

        "positive_mean": positive_mean,
        "negative_mean": negative_mean,
        "separation": separation,
    }


# ==================================================
# 6. Multi-seed evaluation
# ==================================================

results = []


print(
    "\n=== Phase-2 Multi-seed Evaluation ==="
)


for seed in SEEDS:

    result = run_seed(
        seed
    )

    results.append(
        result
    )


    print(
        f"\nSeed {seed}"
    )

    print(
        "Accuracy       :",
        round(
            result["accuracy"],
            4,
        ),
    )

    print(
        "Precision      :",
        round(
            result["precision"],
            4,
        ),
    )

    print(
        "Recall         :",
        round(
            result["recall"],
            4,
        ),
    )

    print(
        "F1             :",
        round(
            result["f1"],
            4,
        ),
    )

    print(
        "Positive mean  :",
        round(
            result[
                "positive_mean"
            ],
            4,
        ),
    )

    print(
        "Negative mean  :",
        round(
            result[
                "negative_mean"
            ],
            4,
        ),
    )

    print(
        "Separation     :",
        round(
            result[
                "separation"
            ],
            4,
        ),
    )


# ==================================================
# 7. Mean ± Standard deviation
# ==================================================

print(
    "\n=== Phase-2 Summary: Mean ± Std ==="
)


metrics = [
    "accuracy",
    "precision",
    "recall",
    "f1",
    "positive_mean",
    "negative_mean",
    "separation",
]


for metric in metrics:

    values = np.array(
        [
            result[metric]
            for result in results
        ],
        dtype=float,
    )


    mean = values.mean()

    std = values.std(
        ddof=1
    )


    print(
        f"{metric:14}: "
        f"{mean:.4f} ± {std:.4f}"
    )


# ==================================================
# 8. Save results
# ==================================================

output_file = (
    "results/"
    "phase2_frozen_graph_results.csv"
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

    writer.writerows(
        results
    )


print(
    "\nSaved to:",
    output_file,
)