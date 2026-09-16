import csv
import random
from pathlib import Path

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


SEED = 42
random.seed(SEED)
torch.manual_seed(SEED)


MANIFEST_PATH = "results/variant_pair_manifest.csv"


VARIANT_BINARIES = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}


THRESHOLD = 0.5
EPOCHS = 100


# --------------------------------------------------
# 1. Load pair manifest
# --------------------------------------------------

rows = []

with open(MANIFEST_PATH, newline="") as f:

    reader = csv.DictReader(f)

    for row in reader:

        row["label"] = int(
            row["label"]
        )

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


print("=== Mini-DeepClone Variant Graph Training ===")

print("\nTrain pairs:", len(train_rows))
print("Test pairs :", len(test_rows))


# --------------------------------------------------
# 2. Parse sample ID
#
# add_O0
# maximum_O3
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
# 3. Collect unique samples
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


print(
    "Unique graph samples:",
    len(sample_ids),
)


# --------------------------------------------------
# 4. Build A-CFG graph cache
# --------------------------------------------------

graphs = {}


print("\n=== Building Graph Cache ===")


for sample_id in sample_ids:

    family, optimization = (
        parse_sample_id(sample_id)
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
        f"{sample_id:22} "
        f"nodes={graph.num_nodes:<2} "
        f"edges={graph.num_edges:<2}"
    )


# --------------------------------------------------
# 5. Model
# --------------------------------------------------

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


# --------------------------------------------------
# 6. Encode one graph
# --------------------------------------------------

def encode_graph(graph):

    batch = torch.zeros(
        graph.num_nodes,
        dtype=torch.long,
    )

    embedding = model(
        graph.x,
        graph.edge_index,
        batch,
    )

    return embedding


# --------------------------------------------------
# 7. Pair score
# --------------------------------------------------

def pair_similarity(
    sample_a,
    sample_b,
):

    emb_a = encode_graph(
        graphs[sample_a]
    )

    emb_b = encode_graph(
        graphs[sample_b]
    )

    score = F.cosine_similarity(
        emb_a,
        emb_b,
    )

    return (
        score,
        emb_a,
        emb_b,
    )


# --------------------------------------------------
# 8. Evaluation
# --------------------------------------------------

def evaluate(rows):

    model.eval()

    labels = []
    scores = []

    with torch.no_grad():

        for row in rows:

            score, _, _ = (
                pair_similarity(
                    row["sample_a"],
                    row["sample_b"],
                )
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


    return {
        "accuracy": accuracy,
        "precision": precision,
        "recall": recall,
        "f1": f1,
        "mean_positive": mean_positive,
        "mean_negative": mean_negative,
        "separation": separation,
        "scores": scores,
        "labels": labels,
        "predictions": predictions,
    }


# --------------------------------------------------
# 9. Before training
# --------------------------------------------------

print("\n=== Before Training ===")


train_before = evaluate(
    train_rows
)

test_before = evaluate(
    test_rows
)


print("\nTRAIN")
print(
    "Positive mean:",
    round(
        train_before[
            "mean_positive"
        ],
        4,
    )
)
print(
    "Negative mean:",
    round(
        train_before[
            "mean_negative"
        ],
        4,
    )
)
print(
    "Separation   :",
    round(
        train_before[
            "separation"
        ],
        4,
    )
)


print("\nTEST")
print(
    "Positive mean:",
    round(
        test_before[
            "mean_positive"
        ],
        4,
    )
)
print(
    "Negative mean:",
    round(
        test_before[
            "mean_negative"
        ],
        4,
    )
)
print(
    "Separation   :",
    round(
        test_before[
            "separation"
        ],
        4,
    )
)


# --------------------------------------------------
# 10. Training
# --------------------------------------------------

print("\n=== Training ===")


for epoch in range(
    1,
    EPOCHS + 1,
):

    model.train()

    random.shuffle(
        train_rows
    )

    total_loss = 0.0


    for row in train_rows:

        optimizer.zero_grad()

        score, emb_a, emb_b = (
            pair_similarity(
                row["sample_a"],
                row["sample_b"],
            )
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

        total_loss += (
            loss.item()
        )


    if (
        epoch == 1
        or epoch % 10 == 0
    ):

        average_loss = (
            total_loss
            / len(train_rows)
        )

        print(
            f"Epoch {epoch:3d} "
            f"Loss={average_loss:.4f}"
        )


# --------------------------------------------------
# 11. Final evaluation
# --------------------------------------------------

print("\n=== After Training ===")


train_after = evaluate(
    train_rows
)

test_after = evaluate(
    test_rows
)


def print_metrics(
    title,
    result,
):

    print(f"\n{title}")

    print(
        "Positive mean:",
        round(
            result[
                "mean_positive"
            ],
            4,
        )
    )

    print(
        "Negative mean:",
        round(
            result[
                "mean_negative"
            ],
            4,
        )
    )

    print(
        "Separation   :",
        round(
            result[
                "separation"
            ],
            4,
        )
    )

    print(
        "Accuracy     :",
        round(
            result[
                "accuracy"
            ],
            4,
        )
    )

    print(
        "Precision    :",
        round(
            result[
                "precision"
            ],
            4,
        )
    )

    print(
        "Recall       :",
        round(
            result[
                "recall"
            ],
            4,
        )
    )

    print(
        "F1           :",
        round(
            result[
                "f1"
            ],
            4,
        )
    )


print_metrics(
    "TRAIN",
    train_after,
)

print_metrics(
    "TEST",
    test_after,
)


# --------------------------------------------------
# 12. Save test predictions
# --------------------------------------------------

output_file = Path(
    "results/"
    "variant_graph_test_predictions.csv"
)


with open(
    output_file,
    "w",
    newline="",
) as f:

    writer = csv.writer(f)

    writer.writerow(
        [
            "sample_a",
            "sample_b",
            "label",
            "similarity",
            "prediction",
        ]
    )

    for (
        row,
        score,
        prediction,
    ) in zip(
        test_rows,
        test_after[
            "scores"
        ],
        test_after[
            "predictions"
        ],
    ):

        writer.writerow(
            [
                row["sample_a"],
                row["sample_b"],
                row["label"],
                round(
                    score,
                    4,
                ),
                prediction,
            ]
        )


print(
    "\nTest predictions saved to:"
)

print(
    output_file
)