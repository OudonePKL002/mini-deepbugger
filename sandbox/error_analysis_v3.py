import pandas as pd


CSV_PATH = "results/deepclone_baseline_v3_results.csv"

THRESHOLD = 0.90


df = pd.read_csv(CSV_PATH)


df["prediction"] = (
    df["weighted_similarity"] >= THRESHOLD
).astype(int)


# True Positive
tp = df[
    (df["label"] == 1)
    & (df["prediction"] == 1)
]

# False Positive
fp = df[
    (df["label"] == 0)
    & (df["prediction"] == 1)
]

# False Negative
fn = df[
    (df["label"] == 1)
    & (df["prediction"] == 0)
]

# True Negative
tn = df[
    (df["label"] == 0)
    & (df["prediction"] == 0)
]


print("=== Mini-DeepClone Error Analysis ===")

print("\nThreshold:")
print(THRESHOLD)

print("\nTP:", len(tp))
print("FP:", len(fp))
print("FN:", len(fn))
print("TN:", len(tn))


print("\n=== Positive Pair Scores ===")

positive_pairs = df[
    df["label"] == 1
][
    [
        "function_a",
        "function_b",
        "weighted_similarity",
        "prediction",
    ]
]

print(
    positive_pairs
    .sort_values(
        "weighted_similarity",
        ascending=False,
    )
    .to_string(index=False)
)


print("\n=== False Negatives ===")

if len(fn) == 0:
    print("None")
else:
    print(
        fn[
            [
                "function_a",
                "function_b",
                "weighted_similarity",
            ]
        ].to_string(index=False)
    )


print("\n=== False Positives ===")

if len(fp) == 0:
    print("None")
else:
    print(
        fp[
            [
                "function_a",
                "function_b",
                "weighted_similarity",
            ]
        ]
        .sort_values(
            "weighted_similarity",
            ascending=False,
        )
        .to_string(index=False)
    )