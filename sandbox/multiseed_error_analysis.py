import csv
import random
from collections import defaultdict

import numpy as np
import torch
import torch.nn.functional as F

from acfg_builder import build_pyg_graph
from graph_encoder_demo import GraphEncoder


MANIFEST_PATH = "results/variant_pair_manifest.csv"

VARIANT_BINARIES = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}

SEEDS = [42, 123, 2026, 7, 99]

EPOCHS = 100
THRESHOLD = 0.5


# --------------------------------------------------
# 1. Load pair manifest
# --------------------------------------------------

rows = []

with open(MANIFEST_PATH, newline="") as f:
    reader = csv.DictReader(f)

    for row in reader:
        row["label"] = int(row["label"])
        rows.append(row)


train_rows = [
    row for row in rows
    if row["split"] == "train"
]

test_rows = [
    row for row in rows
    if row["split"] == "test"
]


# --------------------------------------------------
# 2. Sample parser
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

    graph = build_pyg_graph(
        VARIANT_BINARIES[optimization],
        family,
    )

    graphs[sample_id] = graph


print("Graph samples:", len(graphs))


# --------------------------------------------------
# 4. Store errors across seeds
# --------------------------------------------------

pair_statistics = defaultdict(
    lambda: {
        "label": None,
        "scores": [],
        "predictions": [],
        "error_count": 0,
    }
)


# --------------------------------------------------
# 5. Train and evaluate one seed
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


    # ----------------------------------------------
    # Training
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

    with torch.no_grad():

        for row in test_rows:

            emb_a = encode_graph(
                graphs[row["sample_a"]]
            )

            emb_b = encode_graph(
                graphs[row["sample_b"]]
            )

            score = F.cosine_similarity(
                emb_a,
                emb_b,
            ).item()

            prediction = (
                1 if score >= THRESHOLD
                else 0
            )

            key = (
                row["sample_a"],
                row["sample_b"],
            )

            stats = pair_statistics[key]

            stats["label"] = row["label"]

            stats["scores"].append(
                score
            )

            stats["predictions"].append(
                prediction
            )

            if prediction != row["label"]:
                stats["error_count"] += 1


# --------------------------------------------------
# 6. Run all seeds
# --------------------------------------------------

print("\n=== Running Seeds ===")

for seed in SEEDS:

    print("Seed:", seed)

    run_seed(seed)


# --------------------------------------------------
# 7. Build summary
# --------------------------------------------------

summary_rows = []


for (
    sample_a,
    sample_b,
), stats in pair_statistics.items():

    mean_score = np.mean(
        stats["scores"]
    )

    std_score = np.std(
        stats["scores"],
        ddof=1,
    )

    error_rate = (
        stats["error_count"]
        / len(SEEDS)
    )

    family_a, opt_a = parse_sample_id(
        sample_a
    )

    family_b, opt_b = parse_sample_id(
        sample_b
    )


    summary_rows.append({
        "sample_a": sample_a,
        "sample_b": sample_b,
        "family_a": family_a,
        "family_b": family_b,
        "opt_a": opt_a,
        "opt_b": opt_b,
        "label": stats["label"],
        "mean_similarity": mean_score,
        "std_similarity": std_score,
        "error_count": stats["error_count"],
        "error_rate": error_rate,
    })


# Sort hardest pairs first
summary_rows.sort(
    key=lambda x: (
        x["error_count"],
        abs(
            x["mean_similarity"]
            - THRESHOLD
        ),
    ),
    reverse=True,
)


# --------------------------------------------------
# 8. Print hardest pairs
# --------------------------------------------------

print("\n=== Most Frequently Misclassified Pairs ===")


for row in summary_rows:

    if row["error_count"] == 0:
        continue

    error_type = (
        "FN"
        if row["label"] == 1
        else "FP"
    )

    print(
        f"{error_type:2} "
        f"{row['sample_a']:22} "
        f"vs "
        f"{row['sample_b']:22} "
        f"mean={row['mean_similarity']:.4f} "
        f"std={row['std_similarity']:.4f} "
        f"errors={row['error_count']}/{len(SEEDS)}"
    )


# --------------------------------------------------
# 9. Optimization-pair error statistics
# --------------------------------------------------

optimization_errors = defaultdict(
    lambda: {
        "total": 0,
        "errors": 0,
    }
)


for row in summary_rows:

    opt_pair = tuple(
        sorted(
            [
                row["opt_a"],
                row["opt_b"],
            ]
        )
    )

    optimization_errors[
        opt_pair
    ]["total"] += len(SEEDS)

    optimization_errors[
        opt_pair
    ]["errors"] += row["error_count"]


print("\n=== Error Rate by Optimization Pair ===")


for opt_pair, stats in sorted(
    optimization_errors.items()
):

    rate = (
        stats["errors"]
        / stats["total"]
    )

    print(
        f"{opt_pair[0]} vs {opt_pair[1]} "
        f"errors={stats['errors']}/"
        f"{stats['total']} "
        f"rate={rate:.4f}"
    )


# --------------------------------------------------
# 10. Save CSV
# --------------------------------------------------

output_file = (
    "results/"
    "multiseed_graph_error_analysis.csv"
)


with open(
    output_file,
    "w",
    newline="",
) as f:

    writer = csv.DictWriter(
        f,
        fieldnames=[
            "sample_a",
            "sample_b",
            "family_a",
            "family_b",
            "opt_a",
            "opt_b",
            "label",
            "mean_similarity",
            "std_similarity",
            "error_count",
            "error_rate",
        ],
    )

    writer.writeheader()

    writer.writerows(
        summary_rows
    )


print(
    "\nSaved to:",
    output_file,
)