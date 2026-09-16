import pandas as pd

from sklearn.metrics import (
    accuracy_score,
    precision_score,
    recall_score,
    f1_score,
)


CSV_PATH = "results/deepclone_baseline_results.csv"


df = pd.read_csv(CSV_PATH)


print("=== Mini-DeepClone Baseline Evaluation ===")

print("\nNumber of pairs:")
print(len(df))

print("\nLabel distribution:")
print(df["label"].value_counts().sort_index())


thresholds = [
    0.3,
    0.4,
    0.5,
    0.6,
    0.7,
    0.8,
    0.9,
]


results = []


for threshold in thresholds:

    predictions = (
        df["weighted_similarity"] >= threshold
    ).astype(int)

    accuracy = accuracy_score(
        df["label"],
        predictions,
    )

    precision = precision_score(
        df["label"],
        predictions,
        zero_division=0,
    )

    recall = recall_score(
        df["label"],
        predictions,
        zero_division=0,
    )

    f1 = f1_score(
        df["label"],
        predictions,
        zero_division=0,
    )

    results.append({
        "threshold": threshold,
        "accuracy": accuracy,
        "precision": precision,
        "recall": recall,
        "f1": f1,
    })


results_df = pd.DataFrame(results)


print("\n=== Threshold Results ===")

print(
    results_df.round(4).to_string(
        index=False
    )
)


best_row = results_df.loc[
    results_df["f1"].idxmax()
]


print("\n=== Best Threshold by F1 ===")

print(
    best_row.round(4)
)