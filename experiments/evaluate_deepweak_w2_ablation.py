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
from sklearn.linear_model import LogisticRegression


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
# Ablation feature pooling helpers
# ============================================================

SHARED_DIM = 29
SECURITY_DIM = 10


def pool_full_39d(graph):
    """
    Non-graph baseline:
    mean-pool the original 39-D node features.

    Output shape: [39]
    """
    if graph.x.ndim != 2:
        raise RuntimeError(
            f"Expected graph.x to be 2-D, got {graph.x.shape}"
        )

    if graph.x.shape[1] != INPUT_DIM:
        raise RuntimeError(
            f"Expected {INPUT_DIM} features, "
            f"got {graph.x.shape[1]}"
        )

    return graph.x.mean(
        dim=0
    )


def pool_security_10d(graph):
    """
    Feature-only security baseline.

    The last 10 dimensions are the DeepWeak
    security-aware indicators.

    Max pooling represents whether each
    security indicator occurs anywhere
    in the function.

    Output shape: [10]
    """
    security_x = graph.x[
        :,
        SHARED_DIM:
        SHARED_DIM + SECURITY_DIM
    ]

    if security_x.shape[1] != SECURITY_DIM:
        raise RuntimeError(
            f"Expected {SECURITY_DIM} security features, "
            f"got {security_x.shape[1]}"
        )

    return security_x.max(
        dim=0
    ).values


def compute_binary_metrics(
    labels,
    probabilities,
):
    predictions = [
        1 if p >= THRESHOLD else 0
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

    roc_auc = roc_auc_score(
        labels,
        probabilities,
    )

    tn, fp, fn, tp = confusion_matrix(
        labels,
        predictions,
        labels=[0, 1],
    ).ravel()

    positive_probs = [
        p
        for p, y in zip(
            probabilities,
            labels,
        )
        if y == 1
    ]

    negative_probs = [
        p
        for p, y in zip(
            probabilities,
            labels,
        )
        if y == 0
    ]

    positive_mean = float(
        np.mean(positive_probs)
    )

    negative_mean = float(
        np.mean(negative_probs)
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
# Ablation feature sanity check
# ============================================================

example_train_graph = next(
    iter(
        train_graphs.values()
    )
)

security_vector = pool_security_10d(
    example_train_graph
)

full_vector = pool_full_39d(
    example_train_graph
)

print()
print(
    "=== Ablation Feature Sanity Check ==="
)

print(
    "Security vector shape:",
    tuple(
        security_vector.shape
    ),
)

print(
    "Full pooled vector shape:",
    tuple(
        full_vector.shape
    ),
)

print(
    "Security vector:",
    security_vector.tolist(),
)

# ============================================================
# W1 security feature coverage
# ============================================================

SECURITY_FEATURE_NAMES = [
    "unsafe_copy_call",
    "bounded_copy_call",
    "null_check",
    "bounds_check",
    "division",
    "zero_guard",
    "arithmetic_overflow_guard",
    "range_guard",
    "dynamic_format_call",
    "literal_format_call",
]


print()
print("=== W1 Security Feature Coverage ===")

coverage_all = torch.zeros(
    SECURITY_DIM,
    dtype=torch.int64,
)

coverage_vulnerable = torch.zeros(
    SECURITY_DIM,
    dtype=torch.int64,
)

coverage_safe = torch.zeros(
    SECURITY_DIM,
    dtype=torch.int64,
)

all_zero_count = 0

for row in train_rows:

    graph = train_graphs[
        row["sample_id"]
    ]

    vector = pool_security_10d(
        graph
    )

    present = (
        vector > 0
    ).to(torch.int64)

    coverage_all += present

    if row["label"] == 1:
        coverage_vulnerable += present
    else:
        coverage_safe += present

    if present.sum().item() == 0:
        all_zero_count += 1


for i, name in enumerate(
    SECURITY_FEATURE_NAMES
):
    print(
        f"{name:28s} "
        f"all={coverage_all[i].item():2d} "
        f"vuln={coverage_vulnerable[i].item():2d} "
        f"safe={coverage_safe[i].item():2d}"
    )


print()
print(
    "All-zero security vectors:",
    all_zero_count,
    "/",
    len(train_rows),
)

# ============================================================
# Model A data:
# 10-D security features only
# ============================================================

X_train_security = np.stack([
    pool_security_10d(
        train_graphs[
            row["sample_id"]
        ]
    ).numpy()
    for row in train_rows
])

y_train_security = np.array([
    row["label"]
    for row in train_rows
])

X_w2_security = np.stack([
    pool_security_10d(
        w2_graphs[
            row["sample_id"]
        ]
    ).numpy()
    for row in w2_rows
])

y_w2_security = np.array([
    row["label"]
    for row in w2_rows
])

print()
print(
    "Security-only train shape:",
    X_train_security.shape,
)

print(
    "Security-only W2 shape:",
    X_w2_security.shape,
)


# ============================================================
# Model B/C data:
# 39-D mean-pooled full features
# ============================================================

X_train_full = torch.stack([
    pool_full_39d(
        train_graphs[
            row["sample_id"]
        ]
    )
    for row in train_rows
])

y_train_full = torch.tensor(
    [
        float(row["label"])
        for row in train_rows
    ],
    dtype=torch.float32,
)

X_w2_full = torch.stack([
    pool_full_39d(
        w2_graphs[
            row["sample_id"]
        ]
    )
    for row in w2_rows
])

y_w2_full = torch.tensor(
    [
        float(row["label"])
        for row in w2_rows
    ],
    dtype=torch.float32,
)

print()
print(
    "Full pooled train shape:",
    tuple(X_train_full.shape),
)

print(
    "Full pooled W2 shape:",
    tuple(X_w2_full.shape),
)

class PooledLinearClassifier(
    torch.nn.Module
):
    def __init__(
        self,
        input_dim=INPUT_DIM,
    ):
        super().__init__()

        self.linear = torch.nn.Linear(
            input_dim,
            1,
        )

    def forward(
        self,
        x,
    ):
        return self.linear(
            x
        ).squeeze(-1)

def train_pooled_linear_one_seed(
    seed,
):
    set_seed(seed)

    model = PooledLinearClassifier(
        input_dim=INPUT_DIM
    )

    optimizer = torch.optim.Adam(
        model.parameters(),
        lr=LEARNING_RATE,
    )

    criterion = (
        torch.nn.BCEWithLogitsLoss()
    )

    indices = list(
        range(
            len(X_train_full)
        )
    )

    for epoch in range(
        1,
        EPOCHS + 1,
    ):

        model.train()

        random.shuffle(
            indices
        )

        total_loss = 0.0

        for index in indices:

            optimizer.zero_grad()

            x = X_train_full[
                index
            ]

            target = y_train_full[
                index
            ]

            logit = model(
                x
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

    return model

def evaluate_pooled_linear(
    model,
):
    model.eval()

    probabilities = []

    with torch.no_grad():

        for x in X_w2_full:

            logit = model(
                x
            )

            probability = (
                torch.sigmoid(
                    logit
                ).item()
            )

            probabilities.append(
                probability
            )

    return compute_binary_metrics(
        y_w2_full
        .to(torch.int64)
        .tolist(),
        probabilities,
    )

class PooledMLPClassifier(
    torch.nn.Module
):
    def __init__(
        self,
        input_dim=INPUT_DIM,
        hidden_dim=HIDDEN_DIM,
        embedding_dim=EMBEDDING_DIM,
    ):
        super().__init__()

        self.network = torch.nn.Sequential(
            torch.nn.Linear(
                input_dim,
                hidden_dim,
            ),
            torch.nn.ReLU(),

            torch.nn.Linear(
                hidden_dim,
                embedding_dim,
            ),
            torch.nn.ReLU(),

            torch.nn.Linear(
                embedding_dim,
                1,
            ),
        )

    def forward(
        self,
        x,
    ):
        return self.network(
            x
        ).squeeze(-1)

def train_pooled_mlp_one_seed(
    seed,
):
    set_seed(seed)

    model = PooledMLPClassifier(
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

    indices = list(
        range(
            len(X_train_full)
        )
    )

    for epoch in range(
        1,
        EPOCHS + 1,
    ):

        model.train()

        random.shuffle(
            indices
        )

        total_loss = 0.0

        for index in indices:

            optimizer.zero_grad()

            x = X_train_full[
                index
            ]

            target = y_train_full[
                index
            ]

            logit = model(
                x
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

    return model

def evaluate_pooled_mlp(
    model,
):
    model.eval()

    probabilities = []

    with torch.no_grad():

        for x in X_w2_full:

            logit = model(
                x
            )

            probability = (
                torch.sigmoid(
                    logit
                ).item()
            )

            probabilities.append(
                probability
            )

    return compute_binary_metrics(
        y_w2_full
        .to(torch.int64)
        .tolist(),
        probabilities,
    )

pooled_mlp_results = []

print()
print(
    "=== Ablation C: "
    "39-D Pooled Features + MLP ==="
)

for seed in SEEDS:

    model = train_pooled_mlp_one_seed(
        seed
    )

    metrics = evaluate_pooled_mlp(
        model
    )

    result = {
        "model":
            "Pooled39_MLP",
        "seed":
            seed,
        **metrics,
    }

    pooled_mlp_results.append(
        result
    )

    print()
    print(
        f"--- Seed {seed} ---"
    )

    print(
        f"Accuracy  : "
        f"{metrics['accuracy']:.4f}"
    )

    print(
        f"Precision : "
        f"{metrics['precision']:.4f}"
    )

    print(
        f"Recall    : "
        f"{metrics['recall']:.4f}"
    )

    print(
        f"F1        : "
        f"{metrics['f1']:.4f}"
    )

    print(
        f"ROC-AUC   : "
        f"{metrics['roc_auc']:.4f}"
    )

    print(
        "Confusion : "
        f"TP={metrics['tp']} "
        f"FP={metrics['fp']} "
        f"FN={metrics['fn']} "
        f"TN={metrics['tn']}"
    )

print()
print(
    "=== Pooled39 MLP Summary ==="
)

for metric_name in [
    "accuracy",
    "precision",
    "recall",
    "f1",
    "roc_auc",
]:

    values = np.array([
        result[metric_name]
        for result
        in pooled_mlp_results
    ])

    print(
        f"{metric_name:12s} "
        f"{values.mean():.4f} "
        f"+/- {values.std(ddof=1):.4f}"
    )


# ============================================================
# Model A:
# 10-D Security Features + Logistic Regression
# ============================================================

def evaluate_security_logreg(
    seed,
):
    model = LogisticRegression(
        solver="liblinear",
        C=1.0,
        max_iter=1000,
        random_state=seed,
    )

    model.fit(
        X_train_security,
        y_train_security,
    )

    probabilities = (
        model.predict_proba(
            X_w2_security
        )[:, 1]
    )

    metrics = compute_binary_metrics(
        y_w2_security.tolist(),
        probabilities.tolist(),
    )

    return metrics


security_lr_results = []

print()
print(
    "=== Ablation A: "
    "10-D Security + Logistic Regression ==="
)

for seed in SEEDS:

    metrics = evaluate_security_logreg(
        seed
    )

    result = {
        "model":
            "Security10_LogReg",
        "seed":
            seed,
        **metrics,
    }

    security_lr_results.append(
        result
    )

    print()
    print(
        f"--- Seed {seed} ---"
    )

    print(
        f"Accuracy  : "
        f"{metrics['accuracy']:.4f}"
    )

    print(
        f"Precision : "
        f"{metrics['precision']:.4f}"
    )

    print(
        f"Recall    : "
        f"{metrics['recall']:.4f}"
    )

    print(
        f"F1        : "
        f"{metrics['f1']:.4f}"
    )

    print(
        f"ROC-AUC   : "
        f"{metrics['roc_auc']:.4f}"
    )

    print(
        "Confusion : "
        f"TP={metrics['tp']} "
        f"FP={metrics['fp']} "
        f"FN={metrics['fn']} "
        f"TN={metrics['tn']}"
    )

print()
print(
    "=== Security10 Logistic Regression "
    "Summary ==="
)

for metric_name in [
    "accuracy",
    "precision",
    "recall",
    "f1",
    "roc_auc",
]:

    values = np.array([
        result[metric_name]
        for result
        in security_lr_results
    ])

    print(
        f"{metric_name:12s} "
        f"{values.mean():.4f} "
        f"+/- {values.std(ddof=1):.4f}"
    )


pooled_linear_results = []

print()
print(
    "=== Ablation B: "
    "39-D Pooled Features + Linear Classifier ==="
)

for seed in SEEDS:

    model = train_pooled_linear_one_seed(
        seed
    )

    metrics = evaluate_pooled_linear(
        model
    )

    result = {
        "model":
            "Pooled39_Linear",
        "seed":
            seed,
        **metrics,
    }

    pooled_linear_results.append(
        result
    )

    print()
    print(
        f"--- Seed {seed} ---"
    )

    print(
        f"Accuracy  : "
        f"{metrics['accuracy']:.4f}"
    )

    print(
        f"Precision : "
        f"{metrics['precision']:.4f}"
    )

    print(
        f"Recall    : "
        f"{metrics['recall']:.4f}"
    )

    print(
        f"F1        : "
        f"{metrics['f1']:.4f}"
    )

    print(
        f"ROC-AUC   : "
        f"{metrics['roc_auc']:.4f}"
    )

    print(
        "Confusion : "
        f"TP={metrics['tp']} "
        f"FP={metrics['fp']} "
        f"FN={metrics['fn']} "
        f"TN={metrics['tn']}"
    )


print()
print(
    "=== Pooled39 Linear Summary ==="
)

for metric_name in [
    "accuracy",
    "precision",
    "recall",
    "f1",
    "roc_auc",
]:

    values = np.array([
        result[metric_name]
        for result
        in pooled_linear_results
    ])

    print(
        f"{metric_name:12s} "
        f"{values.mean():.4f} "
        f"+/- {values.std(ddof=1):.4f}"
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
    / "deepweak_w2_ablation_results.csv"
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

# ============================================================
# Final DeepWeak Ablation Export
# ============================================================

ABLATION_PER_SEED_CSV = (
    PROJECT_ROOT
    / "results"
    / "deepweak_w2_ablation_per_seed.csv"
)

ABLATION_SUMMARY_CSV = (
    PROJECT_ROOT
    / "results"
    / "deepweak_w2_ablation_summary.csv"
)


# ------------------------------------------------------------
# Add model name to original GCN results
# ------------------------------------------------------------

gcn_results = [
    {
        "model": "GCN39",
        **result,
    }
    for result in results
]


# ------------------------------------------------------------
# Combine all four experiments
# ------------------------------------------------------------

all_ablation_results = (
    security_lr_results
    + pooled_linear_results
    + pooled_mlp_results
    + gcn_results
)


# ------------------------------------------------------------
# Save per-seed results
# ------------------------------------------------------------

per_seed_fields = [
    "model",
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
]


with ABLATION_PER_SEED_CSV.open(
    "w",
    newline="",
    encoding="utf-8",
) as f:

    writer = csv.DictWriter(
        f,
        fieldnames=per_seed_fields,
    )

    writer.writeheader()

    for result in all_ablation_results:

        writer.writerow({
            field:
                result.get(field, "")
            for field in per_seed_fields
        })


# ------------------------------------------------------------
# Build summary
# ------------------------------------------------------------

MODEL_ORDER = [
    "Security10_LogReg",
    "Pooled39_Linear",
    "Pooled39_MLP",
    "GCN39",
]

SUMMARY_METRICS = [
    "accuracy",
    "precision",
    "recall",
    "f1",
    "roc_auc",
]

summary_rows = []


for model_name in MODEL_ORDER:

    model_results = [
        result
        for result in all_ablation_results
        if result["model"] == model_name
    ]

    summary = {
        "model": model_name,
        "n_runs": len(model_results),
    }

    for metric_name in SUMMARY_METRICS:

        values = np.array(
            [
                result[metric_name]
                for result in model_results
            ],
            dtype=float,
        )

        summary[
            f"{metric_name}_mean"
        ] = values.mean()

        summary[
            f"{metric_name}_std"
        ] = (
            values.std(ddof=1)
            if len(values) > 1
            else 0.0
        )

    summary_rows.append(
        summary
    )


# ------------------------------------------------------------
# Save summary CSV
# ------------------------------------------------------------

summary_fields = [
    "model",
    "n_runs",
]

for metric_name in SUMMARY_METRICS:

    summary_fields.extend([
        f"{metric_name}_mean",
        f"{metric_name}_std",
    ])


with ABLATION_SUMMARY_CSV.open(
    "w",
    newline="",
    encoding="utf-8",
) as f:

    writer = csv.DictWriter(
        f,
        fieldnames=summary_fields,
    )

    writer.writeheader()
    writer.writerows(
        summary_rows
    )


# ------------------------------------------------------------
# Console summary
# ------------------------------------------------------------

print()
print(
    "=== Final DeepWeak Ablation Summary ==="
)

for row in summary_rows:

    print()
    print(
        row["model"]
    )

    print(
        f"  Accuracy : "
        f"{row['accuracy_mean']:.4f} "
        f"+/- {row['accuracy_std']:.4f}"
    )

    print(
        f"  Precision: "
        f"{row['precision_mean']:.4f} "
        f"+/- {row['precision_std']:.4f}"
    )

    print(
        f"  Recall   : "
        f"{row['recall_mean']:.4f} "
        f"+/- {row['recall_std']:.4f}"
    )

    print(
        f"  F1       : "
        f"{row['f1_mean']:.4f} "
        f"+/- {row['f1_std']:.4f}"
    )

    print(
        f"  ROC-AUC  : "
        f"{row['roc_auc_mean']:.4f} "
        f"+/- {row['roc_auc_std']:.4f}"
    )


print()
print(
    "Saved per-seed results to:",
    ABLATION_PER_SEED_CSV,
)

print(
    "Saved summary to:",
    ABLATION_SUMMARY_CSV,
)