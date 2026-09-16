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


from src.deepweak_graph_builder import build_deepweak_graph
from src.deepweak_classifier import DeepWeakClassifier


# ============================================================
# Frozen Protocol
# ============================================================

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


W1_BINARIES = {
    "O0": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O0",
    "O1": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O1",
    "O2": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O2",
    "O3": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O3",
}


W2_BINARIES = {
    "O0": PROJECT_ROOT / "sandbox/deepweak/w2_weaknesses_O0",
    "O1": PROJECT_ROOT / "sandbox/deepweak/w2_weaknesses_O1",
    "O2": PROJECT_ROOT / "sandbox/deepweak/w2_weaknesses_O2",
    "O3": PROJECT_ROOT / "sandbox/deepweak/w2_weaknesses_O3",
}


SEEDS = [
    42,
    123,
    2026,
    7,
    99,
]


INPUT_DIM = 39
HIDDEN_DIM = 32
EMBEDDING_DIM = 16

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
# CSV loader
# ============================================================

def load_manifest(path):

    rows = []

    with path.open(
        "r",
        newline="",
        encoding="utf-8",
    ) as f:

        reader = csv.DictReader(f)

        for row in reader:

            row["label"] = int(
                row["label"]
            )

            rows.append(row)

    return rows


# ============================================================
# Load W1 / W2
# ============================================================

w1_rows = load_manifest(
    W1_MANIFEST
)

w2_rows = load_manifest(
    W2_MANIFEST
)


# Training uses ONLY W1 train rows.

train_rows = [
    row
    for row in w1_rows
    if row["split"] == "train"
]


print(
    "=== Mini-DeepWeak W2 Frozen Independent Evaluation ==="
)

print()

print(
    "W1 training samples:",
    len(train_rows),
)

print(
    "W2 evaluation samples:",
    len(w2_rows),
)


# ============================================================
# Leakage validation
# ============================================================

train_families = {
    row["family"]
    for row in train_rows
}

w2_families = {
    row["family"]
    for row in w2_rows
}


overlap = (
    train_families
    & w2_families
)


print()

print(
    "W1 training families:",
    len(train_families),
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
        "W1/W2 family leakage detected: "
        f"{sorted(overlap)}"
    )


print(
    "Leakage check: OK"
)


# ============================================================
# Build graph caches
# ============================================================

print()
print(
    "=== Building Frozen Graph Cache ==="
)


train_graphs = {}

for row in train_rows:

    sample_id = row["sample_id"]

    if sample_id in train_graphs:
        continue

    graph = build_deepweak_graph(
        str(
            W1_BINARIES[
                row["optimization"]
            ]
        ),
        row["function_name"],
    )

    if (
        graph.num_nodes <= 0
        or graph.num_node_features != INPUT_DIM
    ):
        raise RuntimeError(
            f"Invalid W1 graph: {sample_id}"
        )

    train_graphs[
        sample_id
    ] = graph


w2_graphs = {}

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

    if (
        graph.num_nodes <= 0
        or graph.num_node_features != INPUT_DIM
    ):
        raise RuntimeError(
            f"Invalid W2 graph: {sample_id}"
        )

    w2_graphs[
        sample_id
    ] = graph


print(
    "W1 cached graphs:",
    len(train_graphs),
)

print(
    "W2 cached graphs:",
    len(w2_graphs),
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
# Train frozen W1 model
# ============================================================

def train_one_seed(seed):

    set_seed(seed)


    model = DeepWeakClassifier(
        input_dim=INPUT_DIM,
        hidden_dim=HIDDEN_DIM,
        embedding_dim=EMBEDDING_DIM,
    )


    optimizer = torch.optim.Adam(
        model.parameters(),
        lr=LEARNING_RATE,
    )


    criterion = (
        torch.nn.BCEWithLogitsLoss()
    )


    local_rows = list(
        train_rows
    )


    for epoch in range(
        1,
        EPOCHS + 1,
    ):

        model.train()

        random.shuffle(
            local_rows
        )


        total_loss = 0.0


        for row in local_rows:

            optimizer.zero_grad()


            graph = train_graphs[
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
                / len(local_rows)
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
# W2 evaluation
# ============================================================

def evaluate_w2(model):

    model.eval()


    labels = []
    probabilities = []


    with torch.no_grad():

        for row in w2_rows:

            graph = w2_graphs[
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
        if probability >= THRESHOLD
        else 0
        for probability
        in probabilities
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

    roc_auc = roc_auc_score(
        labels,
        probabilities,
    )


    tn, fp, fn, tp = (
        confusion_matrix(
            labels,
            predictions,
            labels=[0, 1],
        ).ravel()
    )


    positive_probabilities = [
        probability
        for probability, label
        in zip(
            probabilities,
            labels,
        )
        if label == 1
    ]


    negative_probabilities = [
        probability
        for probability, label
        in zip(
            probabilities,
            labels,
        )
        if label == 0
    ]


    positive_mean = float(
        np.mean(
            positive_probabilities
        )
    )


    negative_mean = float(
        np.mean(
            negative_probabilities
        )
    )


    separation = (
        positive_mean
        - negative_mean
    )


    return {
        "accuracy":
            accuracy,

        "precision":
            precision,

        "recall":
            recall,

        "f1":
            f1,

        "roc_auc":
            roc_auc,

        "positive_mean_probability":
            positive_mean,

        "negative_mean_probability":
            negative_mean,

        "probability_separation":
            separation,

        "tp":
            int(tp),

        "fp":
            int(fp),

        "fn":
            int(fn),

        "tn":
            int(tn),
    }


# ============================================================
# Multi-seed independent evaluation
# ============================================================

results = []


print()
print(
    "=== W2 5-Seed Independent Evaluation ==="
)


for seed in SEEDS:

    print()
    print(
        f"--- Seed {seed} ---"
    )


    model = train_one_seed(
        seed
    )


    metrics = evaluate_w2(
        model
    )


    result = {
        "seed": seed,
        **metrics,
    }


    results.append(
        result
    )


    print()

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
# Mean +/- std
# ============================================================

print()
print(
    "=== W2 Summary: Mean +/- Std ==="
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


summary = {}


for metric in SUMMARY_METRICS:

    values = np.array(
        [
            result[metric]
            for result in results
        ],
        dtype=float,
    )


    mean = float(
        values.mean()
    )

    std = float(
        values.std(
            ddof=1
        )
    )


    summary[
        metric
    ] = {
        "mean": mean,
        "std": std,
    }


    print(
        f"{metric:<28} "
        f"{mean:.4f} "
        f"+/- "
        f"{std:.4f}"
    )


# ============================================================
# Save per-seed results
# ============================================================

OUTPUT = (
    PROJECT_ROOT
    / "results"
    / "deepweak_w2_frozen_results.csv"
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
    writer.writerows(
        results
    )


print()
print(
    "Saved to:",
    OUTPUT.relative_to(
        PROJECT_ROOT
    ),
)


print()
print(
    "IMPORTANT:"
)

print(
    "W2 is independent evaluation only."
)

print(
    "Do not tune Mini-DeepWeak v1 "
    "using W2 results."
)