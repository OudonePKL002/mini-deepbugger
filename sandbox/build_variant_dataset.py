from itertools import combinations

from acfg_builder import build_pyg_graph


VARIANTS = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}


FUNCTION_FAMILIES = [
    "add",
    "subtract",
    "multiply",
    "maximum",
    "minimum",

    "is_zero",
    "is_nonzero",
    "greater_than_ten",
    "sign_bit",
    "is_sum_positive",

    "is_positive",
    "is_negative",
    "identity",
]


graphs = {}


print("=== Mini-DeepClone Optimization Dataset ===")


# --------------------------------------------------
# Build all 32 graph samples
# --------------------------------------------------

for function_name in FUNCTION_FAMILIES:

    for opt, binary_path in VARIANTS.items():

        sample_id = (
            f"{function_name}_{opt}"
        )

        graph = build_pyg_graph(
            binary_path,
            function_name,
        )

        graphs[sample_id] = {
            "graph": graph,
            "family": function_name,
            "optimization": opt,
        }

        print(
            f"{sample_id:22} "
            f"nodes={graph.num_nodes:<2} "
            f"edges={graph.num_edges:<2} "
            f"features={graph.num_node_features}"
        )


# --------------------------------------------------
# Create positive pairs
# --------------------------------------------------

positive_pairs = []


for function_name in FUNCTION_FAMILIES:

    samples = [
        f"{function_name}_{opt}"
        for opt in VARIANTS
    ]

    for a, b in combinations(
        samples,
        2,
    ):

        positive_pairs.append(
            (a, b)
        )


print("\n=== Dataset Statistics ===")

print(
    "Function families:",
    len(FUNCTION_FAMILIES),
)

print(
    "Optimization levels:",
    len(VARIANTS),
)

print(
    "Graph samples:",
    len(graphs),
)

print(
    "Positive pairs:",
    len(positive_pairs),
)


print("\nFirst 10 Positive Pairs:")

for pair in positive_pairs[:10]:
    print(
        pair[0],
        "<->",
        pair[1],
    )