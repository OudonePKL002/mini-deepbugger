"""
Mini-DeepClone PoC v1
Final Reproducible Experiment Runner

Protocol:
    Phase 1 -> training/development only
    Phase 2 -> independent evaluation only

IMPORTANT:
    Phase-2 samples MUST NOT be used for:
    - training
    - feature tuning
    - model tuning
    - threshold tuning

Frozen configuration is loaded from:
    config/experiment.json
"""

from __future__ import annotations

import csv
import json
import random
import sys
from pathlib import Path

import numpy as np
import torch
import torch.nn.functional as F

from sklearn.metrics import (
    accuracy_score,
    precision_score,
    recall_score,
    f1_score,
)


# ============================================================
# Project root
# ============================================================

PROJECT_ROOT = Path(__file__).resolve().parents[1]

if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))


from src.acfg_builder import build_pyg_graph
from src.graph_encoder import GraphEncoder


# ============================================================
# Utility
# ============================================================

def print_section(title: str) -> None:
    print()
    print("=" * 64)
    print(title)
    print("=" * 64)


def load_json(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as f:
        return json.load(f)


def load_manifest(path: Path) -> list[dict]:
    rows = []

    with path.open(
        "r",
        newline="",
        encoding="utf-8",
    ) as f:

        reader = csv.DictReader(f)

        for row in reader:

            if "label" not in row:
                raise ValueError(
                    f"Manifest has no 'label' column: {path}"
                )

            row["label"] = int(row["label"])

            rows.append(row)

    return rows


def parse_sample_id(sample_id: str):
    """
    Example:

        absolute_value_O2

    becomes:

        family = absolute_value
        optimization = O2
    """

    family, optimization = sample_id.rsplit("_", 1)

    return family, optimization


def set_seed(seed: int) -> None:
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)


# ============================================================
# Load frozen configuration
# ============================================================

CONFIG_PATH = (
    PROJECT_ROOT
    / "config"
    / "experiment.json"
)


if not CONFIG_PATH.exists():
    raise FileNotFoundError(
        f"Frozen configuration not found:\n{CONFIG_PATH}"
    )


CONFIG = load_json(CONFIG_PATH)


# ============================================================
# Frozen parameters
# ============================================================

INPUT_DIM = CONFIG[
    "graph_encoder"
][
    "input_dimension"
]

HIDDEN_DIM = CONFIG[
    "graph_encoder"
][
    "hidden_dimension"
]

EMBEDDING_DIM = CONFIG[
    "graph_encoder"
][
    "embedding_dimension"
]


MARGIN = CONFIG[
    "training"
][
    "margin"
]

LEARNING_RATE = CONFIG[
    "training"
][
    "learning_rate"
]

EPOCHS = CONFIG[
    "training"
][
    "epochs"
]

SEEDS = CONFIG[
    "training"
][
    "seeds"
]

THRESHOLD = CONFIG[
    "evaluation"
][
    "threshold"
]


EXPECTED_FEATURE_DIM = CONFIG[
    "acfg"
][
    "node_feature_dimension"
]


PHASE1_MANIFEST = (
    PROJECT_ROOT
    / CONFIG[
        "manifests"
    ][
        "phase1"
    ]
)


PHASE2_MANIFEST = (
    PROJECT_ROOT
    / CONFIG[
        "manifests"
    ][
        "phase2"
    ]
)


BINARIES = {
    opt: PROJECT_ROOT / path
    for opt, path
    in CONFIG[
        "compiler"
    ][
        "binaries"
    ].items()
}


# ============================================================
# Output locations
# ============================================================

RESULTS_FINAL = (
    PROJECT_ROOT
    / "results"
    / "final"
)

RESULTS_FINAL.mkdir(
    parents=True,
    exist_ok=True,
)


PER_SEED_OUTPUT = (
    RESULTS_FINAL
    / "phase2_graph_per_seed.csv"
)


SUMMARY_OUTPUT = (
    RESULTS_FINAL
    / "final_experiment_summary.csv"
)


# ============================================================
# Validate frozen protocol
# ============================================================

def validate_config() -> None:

    print_section(
        "1. VALIDATING FROZEN CONFIGURATION"
    )

    expected = {
        "input_dim": 29,
        "hidden_dim": 32,
        "embedding_dim": 16,
        "margin": 0.5,
        "learning_rate": 0.01,
        "epochs": 100,
        "threshold": 0.5,
        "seeds": [
            42,
            123,
            2026,
            7,
            99,
        ],
    }


    actual = {
        "input_dim": INPUT_DIM,
        "hidden_dim": HIDDEN_DIM,
        "embedding_dim": EMBEDDING_DIM,
        "margin": MARGIN,
        "learning_rate": LEARNING_RATE,
        "epochs": EPOCHS,
        "threshold": THRESHOLD,
        "seeds": SEEDS,
    }


    for key, expected_value in expected.items():

        actual_value = actual[key]

        if actual_value != expected_value:

            raise RuntimeError(
                "\nFrozen configuration mismatch!\n"
                f"{key}\n"
                f"Expected: {expected_value}\n"
                f"Actual:   {actual_value}"
            )


    if CONFIG[
        "protocol"
    ][
        "phase2_training_allowed"
    ]:

        raise RuntimeError(
            "Phase-2 training must remain disabled."
        )


    if CONFIG[
        "protocol"
    ][
        "phase2_threshold_tuning_allowed"
    ]:

        raise RuntimeError(
            "Phase-2 threshold tuning must remain disabled."
        )


    print(
        "Project:",
        CONFIG["project"]["name"],
    )

    print(
        "Version:",
        CONFIG["project"]["version"],
    )

    print(
        "Status:",
        CONFIG["project"]["status"],
    )

    print()

    print(
        "Graph Encoder:",
        f"{INPUT_DIM} -> "
        f"{HIDDEN_DIM} -> "
        f"{EMBEDDING_DIM}",
    )

    print(
        "Epochs:",
        EPOCHS,
    )

    print(
        "Learning rate:",
        LEARNING_RATE,
    )

    print(
        "Margin:",
        MARGIN,
    )

    print(
        "Threshold:",
        THRESHOLD,
    )

    print(
        "Seeds:",
        SEEDS,
    )

    print(
        "\nFrozen configuration: OK"
    )


# ============================================================
# Validate files
# ============================================================

def validate_files() -> None:

    print_section(
        "2. VALIDATING REQUIRED FILES"
    )


    required_files = [
        CONFIG_PATH,
        PHASE1_MANIFEST,
        PHASE2_MANIFEST,
    ]


    for path in required_files:

        if not path.exists():

            raise FileNotFoundError(
                f"Required file missing:\n{path}"
            )

        print(
            "[OK]",
            path.relative_to(
                PROJECT_ROOT
            ),
        )


    print()


    for opt, binary_path in BINARIES.items():

        if not binary_path.exists():

            raise FileNotFoundError(
                f"Binary missing:\n"
                f"{binary_path}"
            )


        print(
            f"[OK] {opt}:",
            binary_path.relative_to(
                PROJECT_ROOT
            ),
        )


# ============================================================
# Load manifests
# ============================================================

def prepare_manifests():

    print_section(
        "3. LOADING MANIFESTS"
    )


    phase1_rows = load_manifest(
        PHASE1_MANIFEST
    )


    phase2_rows = load_manifest(
        PHASE2_MANIFEST
    )


    # --------------------------------------------------------
    # Phase-1 train rows
    # --------------------------------------------------------

    if not phase1_rows:
        raise RuntimeError(
            "Phase-1 manifest is empty."
        )


    if "split" not in phase1_rows[0]:

        raise RuntimeError(
            "Phase-1 manifest must contain a "
            "'split' column."
        )


    train_rows = [
        row
        for row in phase1_rows
        if row["split"].strip().lower()
        == "train"
    ]


    if not train_rows:

        raise RuntimeError(
            "No Phase-1 training rows found."
        )


    print(
        "Phase-1 manifest rows:",
        len(phase1_rows),
    )

    print(
        "Phase-1 TRAIN pairs:",
        len(train_rows),
    )

    print(
        "Phase-2 evaluation pairs:",
        len(phase2_rows),
    )


    # --------------------------------------------------------
    # Phase-2 balance check
    # --------------------------------------------------------

    phase2_positive = sum(
        row["label"] == 1
        for row in phase2_rows
    )


    phase2_negative = sum(
        row["label"] == 0
        for row in phase2_rows
    )


    print(
        "\nPhase-2 positive pairs:",
        phase2_positive,
    )

    print(
        "Phase-2 negative pairs:",
        phase2_negative,
    )


    if phase2_positive != 60:
        raise RuntimeError(
            "Expected 60 Phase-2 positive pairs."
        )


    if phase2_negative != 60:
        raise RuntimeError(
            "Expected 60 Phase-2 negative pairs."
        )


    if len(phase2_rows) != 120:
        raise RuntimeError(
            "Expected 120 Phase-2 pairs."
        )


    # --------------------------------------------------------
    # Leakage sanity check
    # --------------------------------------------------------

    train_families = set()

    for row in train_rows:

        family_a, _ = parse_sample_id(
            row["sample_a"]
        )

        family_b, _ = parse_sample_id(
            row["sample_b"]
        )

        train_families.add(
            family_a
        )

        train_families.add(
            family_b
        )


    phase2_families = set()

    for row in phase2_rows:

        family_a, _ = parse_sample_id(
            row["sample_a"]
        )

        family_b, _ = parse_sample_id(
            row["sample_b"]
        )

        phase2_families.add(
            family_a
        )

        phase2_families.add(
            family_b
        )


    overlap = (
        train_families
        & phase2_families
    )


    print(
        "\nPhase-1 training families:",
        len(train_families),
    )

    print(
        "Phase-2 families:",
        len(phase2_families),
    )


    if overlap:

        raise RuntimeError(
            "\nDATA LEAKAGE DETECTED!\n"
            "Families appearing in both "
            "Phase-1 training and Phase-2:\n"
            f"{sorted(overlap)}"
        )


    print(
        "\nPhase-1 / Phase-2 family overlap: 0"
    )

    print(
        "Leakage check: OK"
    )


    return (
        train_rows,
        phase2_rows,
    )


# ============================================================
# Graph cache
# ============================================================

def build_graph_cache(
    train_rows,
    phase2_rows,
):

    print_section(
        "4. BUILDING A-CFG CACHE"
    )


    sample_ids = set()


    for row in train_rows:

        sample_ids.add(
            row["sample_a"]
        )

        sample_ids.add(
            row["sample_b"]
        )


    for row in phase2_rows:

        sample_ids.add(
            row["sample_a"]
        )

        sample_ids.add(
            row["sample_b"]
        )


    sample_ids = sorted(
        sample_ids
    )


    graphs = {}


    for index, sample_id in enumerate(
        sample_ids,
        start=1,
    ):

        family, optimization = (
            parse_sample_id(
                sample_id
            )
        )


        if optimization not in BINARIES:

            raise RuntimeError(
                f"Unknown optimization level: "
                f"{optimization}"
            )


        graph = build_pyg_graph(
            str(
                BINARIES[
                    optimization
                ]
            ),
            family,
        )


        if graph.num_nodes <= 0:

            raise RuntimeError(
                f"Invalid graph: {sample_id}\n"
                "num_nodes <= 0"
            )


        if (
            graph.num_node_features
            != EXPECTED_FEATURE_DIM
        ):

            raise RuntimeError(
                f"Feature dimension mismatch "
                f"for {sample_id}\n"
                f"Expected: "
                f"{EXPECTED_FEATURE_DIM}\n"
                f"Actual: "
                f"{graph.num_node_features}"
            )


        graphs[
            sample_id
        ] = graph


        print(
            f"[{index:02d}/"
            f"{len(sample_ids):02d}] "
            f"{sample_id:<28} "
            f"nodes={graph.num_nodes:<3} "
            f"edges={graph.num_edges:<3} "
            f"features="
            f"{graph.num_node_features}"
        )


    print(
        "\nTotal cached graphs:",
        len(graphs),
    )

    print(
        "Invalid graphs: 0"
    )

    print(
        "Graph cache validation: OK"
    )


    return graphs


# ============================================================
# Graph encoding
# ============================================================

def encode_graph(
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
# Train one frozen seed
# ============================================================

def train_model(
    seed,
    train_rows,
    graphs,
):

    set_seed(seed)


    model = GraphEncoder(
        input_dim=INPUT_DIM,
        hidden_dim=HIDDEN_DIM,
        embedding_dim=EMBEDDING_DIM,
    )


    optimizer = torch.optim.Adam(
        model.parameters(),
        lr=LEARNING_RATE,
    )


    criterion = (
        torch.nn.CosineEmbeddingLoss(
            margin=MARGIN
        )
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


        epoch_loss = 0.0


        for row in local_train_rows:

            optimizer.zero_grad()


            emb_a = encode_graph(
                model,
                graphs[
                    row["sample_a"]
                ],
            )


            emb_b = encode_graph(
                model,
                graphs[
                    row["sample_b"]
                ],
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


            epoch_loss += (
                loss.item()
            )


        # Keep terminal output concise.
        if epoch in {
            1,
            25,
            50,
            75,
            100,
        }:

            avg_loss = (
                epoch_loss
                / len(
                    local_train_rows
                )
            )


            print(
                f"  Epoch "
                f"{epoch:3d}/"
                f"{EPOCHS} "
                f"loss="
                f"{avg_loss:.6f}"
            )


    return model


# ============================================================
# Evaluate Graph Encoder
# ============================================================

def evaluate_graph_model(
    model,
    phase2_rows,
    graphs,
):

    model.eval()


    labels = []

    scores = []


    with torch.no_grad():

        for row in phase2_rows:

            emb_a = encode_graph(
                model,
                graphs[
                    row["sample_a"]
                ],
            )


            emb_b = encode_graph(
                model,
                graphs[
                    row["sample_b"]
                ],
            )


            score = (
                F.cosine_similarity(
                    emb_a,
                    emb_b,
                ).item()
            )


            labels.append(
                row["label"]
            )

            scores.append(
                score
            )


    predictions = [
        1
        if score >= THRESHOLD
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


    tp = sum(
        pred == 1
        and label == 1

        for pred, label
        in zip(
            predictions,
            labels,
        )
    )


    fp = sum(
        pred == 1
        and label == 0

        for pred, label
        in zip(
            predictions,
            labels,
        )
    )


    fn = sum(
        pred == 0
        and label == 1

        for pred, label
        in zip(
            predictions,
            labels,
        )
    )


    tn = sum(
        pred == 0
        and label == 0

        for pred, label
        in zip(
            predictions,
            labels,
        )
    )


    return {
        "accuracy": accuracy,
        "precision": precision,
        "recall": recall,
        "f1": f1,
        "positive_mean": positive_mean,
        "negative_mean": negative_mean,
        "separation": separation,
        "tp": tp,
        "fp": fp,
        "fn": fn,
        "tn": tn,
    }


# ============================================================
# Handcrafted baseline
# ============================================================

def evaluate_handcrafted(
    phase2_rows,
    graphs,
):

    labels = []

    scores = []


    for row in phase2_rows:

        graph_a = graphs[
            row["sample_a"]
        ]

        graph_b = graphs[
            row["sample_b"]
        ]


        vector_a = graph_a.x.sum(
            dim=0,
            keepdim=True,
        )


        vector_b = graph_b.x.sum(
            dim=0,
            keepdim=True,
        )


        score = (
            F.cosine_similarity(
                vector_a,
                vector_b,
            ).item()
        )


        labels.append(
            row["label"]
        )

        scores.append(
            score
        )


    predictions = [
        1
        if score >= THRESHOLD
        else 0

        for score in scores
    ]


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


    result = {

        "accuracy":
            accuracy_score(
                labels,
                predictions,
            ),

        "precision":
            precision_score(
                labels,
                predictions,
                zero_division=0,
            ),

        "recall":
            recall_score(
                labels,
                predictions,
                zero_division=0,
            ),

        "f1":
            f1_score(
                labels,
                predictions,
                zero_division=0,
            ),

        "positive_mean":
            positive_mean,

        "negative_mean":
            negative_mean,

        "separation":
            positive_mean
            - negative_mean,
    }


    return result


# ============================================================
# Save per-seed results
# ============================================================

def save_per_seed_results(
    results,
):

    fieldnames = [
        "seed",
        "accuracy",
        "precision",
        "recall",
        "f1",
        "positive_mean",
        "negative_mean",
        "separation",
        "tp",
        "fp",
        "fn",
        "tn",
    ]


    with PER_SEED_OUTPUT.open(
        "w",
        newline="",
        encoding="utf-8",
    ) as f:

        writer = csv.DictWriter(
            f,
            fieldnames=fieldnames,
        )


        writer.writeheader()

        writer.writerows(
            results
        )


# ============================================================
# Summarize multi-seed metrics
# ============================================================

def summarize_graph_results(
    results,
):

    metrics = [
        "accuracy",
        "precision",
        "recall",
        "f1",
        "positive_mean",
        "negative_mean",
        "separation",
    ]


    summary = {}


    for metric in metrics:

        values = np.array(
            [
                result[metric]
                for result
                in results
            ],
            dtype=float,
        )


        summary[
            metric
        ] = {
            "mean":
                float(
                    values.mean()
                ),

            "std":
                float(
                    values.std(
                        ddof=1
                    )
                ),
        }


    return summary


# ============================================================
# Save final summary CSV
# ============================================================

def save_final_summary(
    handcrafted,
    graph_summary,
):

    rows = []


    for metric in [
        "accuracy",
        "precision",
        "recall",
        "f1",
        "positive_mean",
        "negative_mean",
        "separation",
    ]:

        rows.append(
            {
                "metric": metric,

                "handcrafted":
                    handcrafted[
                        metric
                    ],

                "graph_mean":
                    graph_summary[
                        metric
                    ][
                        "mean"
                    ],

                "graph_std":
                    graph_summary[
                        metric
                    ][
                        "std"
                    ],
            }
        )


    with SUMMARY_OUTPUT.open(
        "w",
        newline="",
        encoding="utf-8",
    ) as f:

        writer = csv.DictWriter(
            f,
            fieldnames=[
                "metric",
                "handcrafted",
                "graph_mean",
                "graph_std",
            ],
        )


        writer.writeheader()

        writer.writerows(
            rows
        )


# ============================================================
# Main experiment
# ============================================================

def main():

    print_section(
        "Mini-DeepClone PoC v1"
    )

    print(
        "FINAL REPRODUCIBLE EXPERIMENT"
    )

    print(
        "\nPhase 1: training only"
    )

    print(
        "Phase 2: independent evaluation only"
    )


    # --------------------------------------------------------
    # Validation
    # --------------------------------------------------------

    validate_config()

    validate_files()


    train_rows, phase2_rows = (
        prepare_manifests()
    )


    # --------------------------------------------------------
    # Graph cache
    # --------------------------------------------------------

    graphs = build_graph_cache(
        train_rows,
        phase2_rows,
    )


    # --------------------------------------------------------
    # Graph Encoder
    # --------------------------------------------------------

    print_section(
        "5. GRAPH ENCODER MULTI-SEED EVALUATION"
    )


    graph_results = []


    for seed in SEEDS:

        print()
        print(
            f"--- Seed {seed} ---"
        )


        model = train_model(
            seed,
            train_rows,
            graphs,
        )


        metrics = (
            evaluate_graph_model(
                model,
                phase2_rows,
                graphs,
            )
        )


        row = {
            "seed": seed,
            **metrics,
        }


        graph_results.append(
            row
        )


        print()

        print(
            f"Accuracy      : "
            f"{metrics['accuracy']:.4f}"
        )

        print(
            f"Precision     : "
            f"{metrics['precision']:.4f}"
        )

        print(
            f"Recall        : "
            f"{metrics['recall']:.4f}"
        )

        print(
            f"F1            : "
            f"{metrics['f1']:.4f}"
        )

        print(
            f"Positive mean : "
            f"{metrics['positive_mean']:.4f}"
        )

        print(
            f"Negative mean : "
            f"{metrics['negative_mean']:.4f}"
        )

        print(
            f"Separation    : "
            f"{metrics['separation']:.4f}"
        )


    # --------------------------------------------------------
    # Save graph results
    # --------------------------------------------------------

    save_per_seed_results(
        graph_results
    )


    graph_summary = (
        summarize_graph_results(
            graph_results
        )
    )


    # --------------------------------------------------------
    # Handcrafted baseline
    # --------------------------------------------------------

    print_section(
        "6. HANDCRAFTED A-CFG BASELINE"
    )


    handcrafted = (
        evaluate_handcrafted(
            phase2_rows,
            graphs,
        )
    )


    print(
        f"Accuracy      : "
        f"{handcrafted['accuracy']:.4f}"
    )

    print(
        f"Precision     : "
        f"{handcrafted['precision']:.4f}"
    )

    print(
        f"Recall        : "
        f"{handcrafted['recall']:.4f}"
    )

    print(
        f"F1            : "
        f"{handcrafted['f1']:.4f}"
    )

    print(
        f"Positive mean : "
        f"{handcrafted['positive_mean']:.4f}"
    )

    print(
        f"Negative mean : "
        f"{handcrafted['negative_mean']:.4f}"
    )

    print(
        f"Separation    : "
        f"{handcrafted['separation']:.4f}"
    )


    # --------------------------------------------------------
    # Save final comparison
    # --------------------------------------------------------

    save_final_summary(
        handcrafted,
        graph_summary,
    )


    # --------------------------------------------------------
    # Final report
    # --------------------------------------------------------

    print_section(
        "7. FINAL PHASE-2 RESULTS"
    )


    print(
        "Dataset:"
    )

    print(
        "  10 unseen function families"
    )

    print(
        "  40 graph samples"
    )

    print(
        "  120 balanced pairs"
    )

    print(
        "  60 positive / 60 negative"
    )


    print(
        "\nHandcrafted A-CFG"
    )

    print(
        f"  Accuracy   : "
        f"{handcrafted['accuracy']:.4f}"
    )

    print(
        f"  Precision  : "
        f"{handcrafted['precision']:.4f}"
    )

    print(
        f"  Recall     : "
        f"{handcrafted['recall']:.4f}"
    )

    print(
        f"  F1         : "
        f"{handcrafted['f1']:.4f}"
    )

    print(
        f"  Separation : "
        f"{handcrafted['separation']:.4f}"
    )


    print(
        "\nGraph Encoder "
        "(5 seeds, mean +/- std)"
    )


    for metric in [
        "accuracy",
        "precision",
        "recall",
        "f1",
        "separation",
    ]:

        mean = (
            graph_summary[
                metric
            ][
                "mean"
            ]
        )

        std = (
            graph_summary[
                metric
            ][
                "std"
            ]
        )


        print(
            f"  "
            f"{metric.capitalize():<10} : "
            f"{mean:.4f} +/- "
            f"{std:.4f}"
        )


    print(
        "\nSimilarity distributions:"
    )


    print(
        "  Positive : "
        f"{graph_summary['positive_mean']['mean']:.4f}"
        " +/- "
        f"{graph_summary['positive_mean']['std']:.4f}"
    )


    print(
        "  Negative : "
        f"{graph_summary['negative_mean']['mean']:.4f}"
        " +/- "
        f"{graph_summary['negative_mean']['std']:.4f}"
    )


    # --------------------------------------------------------
    # Improvement
    # --------------------------------------------------------

    graph_f1 = (
        graph_summary[
            "f1"
        ][
            "mean"
        ]
    )


    baseline_f1 = (
        handcrafted[
            "f1"
        ]
    )


    absolute_f1_gain = (
        graph_f1
        - baseline_f1
    )


    relative_f1_gain = (
        absolute_f1_gain
        / baseline_f1
    )


    print(
        "\nGraph Encoder improvement:"
    )


    print(
        f"  Absolute F1 gain : "
        f"{absolute_f1_gain:.4f}"
    )


    print(
        f"  Relative F1 gain : "
        f"{relative_f1_gain * 100:.2f}%"
    )


    print_section(
        "REPRODUCTION COMPLETE"
    )


    print(
        "Saved:"
    )

    print(
        " ",
        PER_SEED_OUTPUT.relative_to(
            PROJECT_ROOT
        ),
    )

    print(
        " ",
        SUMMARY_OUTPUT.relative_to(
            PROJECT_ROOT
        ),
    )


    print(
        "\nIMPORTANT:"
    )

    print(
        "Phase-2 remains evaluation-only."
    )

    print(
        "Do not tune PoC-v1 using these results."
    )


if __name__ == "__main__":
    main()