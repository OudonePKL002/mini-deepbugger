import csv
from collections import Counter

from acfg_builder import (
    build_pyg_graph,
    FEATURE_ORDER,
)


MANIFEST_PATH = "results/variant_pair_manifest.csv"

VARIANT_BINARIES = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}


def parse_sample_id(sample_id):
    return sample_id.rsplit("_", 1)


rows = []

with open(MANIFEST_PATH, newline="") as f:
    reader = csv.DictReader(f)

    rows = list(reader)


split_samples = {
    "train": set(),
    "test": set(),
}


for row in rows:

    split_samples[row["split"]].add(
        row["sample_a"]
    )

    split_samples[row["split"]].add(
        row["sample_b"]
    )


for split_name in ["train", "test"]:

    feature_counts = Counter()

    print(
        f"\n=== {split_name.upper()} FEATURE COVERAGE ==="
    )

    for sample_id in sorted(
        split_samples[split_name]
    ):

        family, opt = parse_sample_id(
            sample_id
        )

        graph = build_pyg_graph(
            VARIANT_BINARIES[opt],
            family,
        )

        vector = graph.x.sum(dim=0)

        for feature, value in zip(
            FEATURE_ORDER,
            vector.tolist(),
        ):

            if value > 0:
                feature_counts[feature] += 1


    for feature in FEATURE_ORDER:

        print(
            f"{feature:25} "
            f"{feature_counts[feature]}"
        )


# --------------------------------------------------
# Features appearing in test but not train
# --------------------------------------------------

def used_features(sample_ids):

    used = set()

    for sample_id in sample_ids:

        family, opt = parse_sample_id(
            sample_id
        )

        graph = build_pyg_graph(
            VARIANT_BINARIES[opt],
            family,
        )

        vector = graph.x.sum(dim=0)

        for feature, value in zip(
            FEATURE_ORDER,
            vector.tolist(),
        ):

            if value > 0:
                used.add(feature)

    return used


train_features = used_features(
    split_samples["train"]
)

test_features = used_features(
    split_samples["test"]
)


print(
    "\n=== TEST-ONLY FEATURES ==="
)

test_only = (
    test_features
    - train_features
)


if not test_only:
    print("None")

else:
    for feature in sorted(test_only):
        print(feature)