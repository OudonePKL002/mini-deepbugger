import torch
import torch.nn.functional as F

from acfg_builder import (
    build_pyg_graph,
    FEATURE_ORDER,
)


VARIANTS = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}


FUNCTIONS = [
    "is_positive",
    "is_negative",
    "identity",
]


graphs = {}


print("=== Mini-DeepClone Hard Case Inspection ===")


for function_name in FUNCTIONS:

    for opt, binary_path in VARIANTS.items():

        sample_id = f"{function_name}_{opt}"

        graph = build_pyg_graph(
            binary_path,
            function_name,
        )

        graphs[sample_id] = graph

        # Aggregate all basic-block features
        vector = graph.x.sum(
            dim=0
        )

        print(
            f"\n=== {sample_id} ==="
        )

        print(
            f"Nodes={graph.num_nodes}, "
            f"Edges={graph.num_edges}"
        )

        print("Non-zero features:")

        for feature_name, value in zip(
            FEATURE_ORDER,
            vector.tolist(),
        ):

            if value != 0:
                print(
                    f"  {feature_name:25} {value}"
                )


# --------------------------------------------------
# Inspect troublesome same-optimization pairs
# --------------------------------------------------

PAIRS = [
    ("is_positive_O1", "is_negative_O1"),
    ("is_positive_O2", "is_negative_O2"),
    ("is_positive_O3", "is_negative_O3"),

    ("is_positive_O1", "identity_O1"),
    ("is_positive_O2", "identity_O2"),
    ("is_positive_O3", "identity_O3"),
]


print("\n\n=== Raw Feature Similarities ===")


for a, b in PAIRS:

    vector_a = graphs[a].x.sum(
        dim=0,
        keepdim=True,
    )

    vector_b = graphs[b].x.sum(
        dim=0,
        keepdim=True,
    )

    similarity = F.cosine_similarity(
        vector_a,
        vector_b,
    ).item()

    print(
        f"{a:22} vs "
        f"{b:22} "
        f"similarity={similarity:.4f}"
    )