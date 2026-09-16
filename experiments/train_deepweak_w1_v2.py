import csv
import random
import sys
from pathlib import Path

import numpy as np
import torch

from sklearn.metrics import (
    accuracy_score,
    precision_score,
    recall_score,
    f1_score,
    roc_auc_score,
    confusion_matrix,
)


PROJECT_ROOT = Path(__file__).resolve().parents[1]

if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))


from src.deepweak_graph_builder import (
    build_deepweak_graph,
)

from src.deepweak_classifier import (
    DeepWeakClassifier,
)


MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_w1_split_v2.csv"
)


BINARIES = {
    "O0": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O0",
    "O1": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O1",
    "O2": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O2",
    "O3": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O3",
}


SEEDS = [
    42,
    123,
    2026,
    7,
    99,
]


EPOCHS = 100
LEARNING_RATE = 0.01
THRESHOLD = 0.5


# ============================================================
# Reproducibility
# ============================================================

def set_seed(seed):

    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)


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


print(
    "=== Mini-DeepWeak W1 Development Evaluation ==="
)

print(
    "Train samples:",
    len(train_rows),
)

print(
    "Dev samples:",
    len(dev_rows),
)


# ============================================================
# Leakage check
# ============================================================

train_families = {
    row["family"]
    for row in train_rows
}

dev_families = {
    row["family"]
    for row in dev_rows
}


overlap = (
    train_families
    & dev_families
)


print(
    "\nTrain families:",
    sorted(train_families),
)

print(
    "Dev families:",
    sorted(dev_families),
)

print(
    "Family overlap:",
    len(overlap),
)


if overlap:
    raise RuntimeError(
        f"Family leakage detected: {overlap}"
    )


# ============================================================
# Build graph cache
# ============================================================

print(
    "\n=== Building 39-D Graph Cache ==="
)


graphs = {}


for row in rows:

    sample_id = row["sample_id"]

    if sample_id in graphs:
        continue


    graph = build_deepweak_graph(
        str(
            BINARIES[
                row["optimization"]
            ]
        ),
        row["function_name"],
    )


    if graph.num_nodes <= 0:
        raise RuntimeError(
            f"Invalid graph: {sample_id}"
        )


    if graph.num_node_features != 39:
        raise RuntimeError(
            f"Expected 39 features for "
            f"{sample_id}, got "
            f"{graph.num_node_features}"
        )


    graphs[sample_id] = graph


print(
    "Cached graphs:",
    len(graphs),
)


# ============================================================
# Forward helper
# ============================================================

def forward_graph(
    model,
    graph,
):

    batch = torch.zeros(
        graph.num_nodes,
        dtype=torch.long,
    )


    return model(
        graph.x,
        graph.edge_index,
        batch,
    )


# ============================================================
# Train one seed
# ============================================================

def train_one_seed(seed):

    set_seed(seed)


    model = DeepWeakClassifier(
        input_dim=39,
        hidden_dim=32,
        embedding_dim=16,
    )


    optimizer = torch.optim.Adam(
        model.parameters(),
        lr=LEARNING_RATE,
    )


    criterion = (
        torch.nn.BCEWithLogitsLoss()
    )


    local_train_rows = list(
        train_rows
    )


    for epoch in range(
        1,
        EPOCHS + 1,
    ):

        model.train()

        random.shuffle(
            local_train_rows
        )


        total_loss = 0.0


        for row in local_train_rows:

            optimizer.zero_grad()


            graph = graphs[
                row["sample_id"]
            ]


            logit = forward_graph(
                model,
                graph,
            )


            target = torch.tensor(
                [
                    float(
                        row["label"]
                    )
                ],
                dtype=torch.float32,
            )


            loss = criterion(
                logit,
                target,
            )


            loss.backward()

            optimizer.step()


            total_loss += (
                loss.item()
            )


        if epoch in {
            1,
            25,
            50,
            75,
            100,
        }:

            mean_loss = (
                total_loss
                / len(
                    local_train_rows
                )
            )


            print(
                f"  Epoch "
                f"{epoch:3d}/"
                f"{EPOCHS} "
                f"loss="
                f"{mean_loss:.6f}"
            )


    return model


# ============================================================
# Evaluate
# ============================================================

def evaluate(
    model,
    eval_rows,
):

    model.eval()


    labels = []
    probabilities = []


    with torch.no_grad():

        for row in eval_rows:

            graph = graphs[
                row["sample_id"]
            ]


            logit = forward_graph(
                model,
                graph,
            )


            probability = (
                torch.sigmoid(
                    logit
                ).item()
            )


            labels.append(
                row["label"]
            )

            probabilities.append(
                probability
            )


    predictions = [
        1
        if p >= THRESHOLD
        else 0
        for p in probabilities
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


    try:

        roc_auc = roc_auc_score(
            labels,
            probabilities,
        )

    except ValueError:

        roc_auc = float("nan")


    tn, fp, fn, tp = (
        confusion_matrix(
            labels,
            predictions,
            labels=[0, 1],
        ).ravel()
    )


    positive_mean = float(
        np.mean(
            [
                p
                for p, label
                in zip(
                    probabilities,
                    labels,
                )
                if label == 1
            ]
        )
    )


    negative_mean = float(
        np.mean(
            [
                p
                for p, label
                in zip(
                    probabilities,
                    labels,
                )
                if label == 0
            ]
        )
    )


    return {
        "accuracy": accuracy,
        "precision": precision,
        "recall": recall,
        "f1": f1,
        "roc_auc": roc_auc,
        "positive_mean_probability":
            positive_mean,
        "negative_mean_probability":
            negative_mean,
        "probability_separation":
            positive_mean - negative_mean,
        "tp": int(tp),
        "fp": int(fp),
        "fn": int(fn),
        "tn": int(tn),
    }


# ============================================================
# Multi-seed run
# ============================================================

results = []


print(
    "\n=== 5-Seed W1 Development Run ==="
)


for seed in SEEDS:

    print(
        f"\n--- Seed {seed} ---"
    )


    model = train_one_seed(
        seed
    )


    metrics = evaluate(
        model,
        dev_rows,
    )


    result = {
        "seed": seed,
        **metrics,
    }


    results.append(
        result
    )


    print(
        f"Accuracy   : "
        f"{metrics['accuracy']:.4f}"
    )

    print(
        f"Precision  : "
        f"{metrics['precision']:.4f}"
    )

    print(
        f"Recall     : "
        f"{metrics['recall']:.4f}"
    )

    print(
        f"F1         : "
        f"{metrics['f1']:.4f}"
    )

    print(
        f"ROC-AUC    : "
        f"{metrics['roc_auc']:.4f}"
    )

    print(
        f"Positive P : "
        f"{metrics['positive_mean_probability']:.4f}"
    )

    print(
        f"Negative P : "
        f"{metrics['negative_mean_probability']:.4f}"
    )

    print(
        f"Separation : "
        f"{metrics['probability_separation']:.4f}"
    )

    print(
        "Confusion  : "
        f"TP={metrics['tp']} "
        f"FP={metrics['fp']} "
        f"FN={metrics['fn']} "
        f"TN={metrics['tn']}"
    )


# ============================================================
# Summary
# ============================================================

print(
    "\n=== W1 Summary: Mean +/- Std ==="
)


SUMMARY_METRICS = [
    "accuracy",
    "precision",
    "recall",
    "f1",
    "roc_auc",
    "positive_mean_probability",
    "negative_mean_probability",
    "probability_separation",
]


for metric in SUMMARY_METRICS:

    values = np.array(
        [
            result[metric]
            for result in results
        ],
        dtype=float,
    )


    print(
        f"{metric:<28} "
        f"{values.mean():.4f} "
        f"+/- "
        f"{values.std(ddof=1):.4f}"
    )


# ============================================================
# Save
# ============================================================

OUTPUT = (
    PROJECT_ROOT
    / "results"
    / "deepweak_w1_v2_multiseed_results.csv"
)


with OUTPUT.open(
    "w",
    newline="",
    encoding="utf-8",
) as f:

    writer = csv.DictWriter(
        f,
        fieldnames=[
            "seed",
            "accuracy",
            "precision",
            "recall",
            "f1",
            "roc_auc",
            "positive_mean_probability",
            "negative_mean_probability",
            "probability_separation",
            "tp",
            "fp",
            "fn",
            "tn",
        ],
    )


    writer.writeheader()
    writer.writerows(results)


print(
    "\nSaved to:",
    OUTPUT.relative_to(
        PROJECT_ROOT
    ),
)