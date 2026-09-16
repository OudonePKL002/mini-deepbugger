import csv
import random
import sys
from pathlib import Path

import numpy as np
import torch

PROJECT_ROOT = Path(__file__).resolve().parents[1]

if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))


from src.deepweak_graph_builder import build_deepweak_graph
from src.deepweak_classifier import DeepWeakClassifier


MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_w1_split.csv"
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
# Load split
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
    "=== Mini-DeepWeak W1 Error Analysis ==="
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
# Graph cache
# ============================================================

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

    graphs[sample_id] = graph


# ============================================================
# Forward
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

def train_model(seed):

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

    for _ in range(EPOCHS):

        model.train()

        random.shuffle(
            local_train_rows
        )

        for row in local_train_rows:

            optimizer.zero_grad()

            logit = forward_graph(
                model,
                graphs[
                    row["sample_id"]
                ],
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

    return model


# ============================================================
# Evaluate every seed / sample
# ============================================================

records = []


for seed in SEEDS:

    print(
        f"\n--- Seed {seed} ---"
    )

    model = train_model(seed)

    model.eval()

    with torch.no_grad():

        for row in dev_rows:

            logit = forward_graph(
                model,
                graphs[
                    row["sample_id"]
                ],
            )

            probability = (
                torch.sigmoid(
                    logit
                ).item()
            )

            prediction = (
                1
                if probability >= THRESHOLD
                else 0
            )

            true_label = (
                row["label"]
            )

            if (
                prediction == 1
                and true_label == 1
            ):
                error_type = "TP"

            elif (
                prediction == 1
                and true_label == 0
            ):
                error_type = "FP"

            elif (
                prediction == 0
                and true_label == 1
            ):
                error_type = "FN"

            else:
                error_type = "TN"


            record = {
                "seed": seed,
                "sample_id":
                    row["sample_id"],
                "family":
                    row["family"],
                "optimization":
                    row["optimization"],
                "true_label":
                    true_label,
                "probability":
                    probability,
                "prediction":
                    prediction,
                "error_type":
                    error_type,
            }

            records.append(record)


            print(
                f"{row['sample_id']:<28} "
                f"label={true_label} "
                f"p={probability:.4f} "
                f"pred={prediction} "
                f"{error_type}"
            )


# ============================================================
# Aggregate by sample
# ============================================================

print()
print(
    "=== Repeated Error Summary ==="
)


sample_ids = sorted(
    {
        record["sample_id"]
        for record in records
    }
)


for sample_id in sample_ids:

    sample_records = [
        record
        for record in records
        if record["sample_id"]
        == sample_id
    ]

    probabilities = [
        record["probability"]
        for record in sample_records
    ]

    error_count = sum(
        record["error_type"]
        in {"FP", "FN"}
        for record in sample_records
    )

    first = sample_records[0]

    print(
        f"{sample_id:<28} "
        f"family={first['family']:<14} "
        f"label={first['true_label']} "
        f"mean_p={np.mean(probabilities):.4f} "
        f"std_p={np.std(probabilities, ddof=1):.4f} "
        f"errors={error_count}/{len(SEEDS)}"
    )


# ============================================================
# Aggregate by family
# ============================================================

print()
print(
    "=== Family-Level Error Summary ==="
)


families = sorted(
    {
        row["family"]
        for row in dev_rows
    }
)


for family in families:

    family_records = [
        record
        for record in records
        if record["family"] == family
    ]

    fn = sum(
        record["error_type"] == "FN"
        for record in family_records
    )

    fp = sum(
        record["error_type"] == "FP"
        for record in family_records
    )

    total = len(
        family_records
    )

    print(
        f"{family:<16} "
        f"FN={fn:<3} "
        f"FP={fp:<3} "
        f"errors={fn + fp}/{total}"
    )


# ============================================================
# Save CSV
# ============================================================

OUTPUT = (
    PROJECT_ROOT
    / "results"
    / "deepweak_w1_error_analysis.csv"
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
            "sample_id",
            "family",
            "optimization",
            "true_label",
            "probability",
            "prediction",
            "error_type",
        ],
    )

    writer.writeheader()
    writer.writerows(records)


print()
print(
    "Saved to:",
    OUTPUT.relative_to(
        PROJECT_ROOT
    ),
)