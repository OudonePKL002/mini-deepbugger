from acfg_builder import build_pyg_graph


VARIANT_BINARIES = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}


PHASE2_FAMILIES = [
    "absolute_value",
    "square",
    "is_even",
    "is_odd",
    "average_two",
    "max_three",
    "min_three",
    "in_range",
    "clamp_zero_hundred",
    "sum_three",
]


print("=== Phase-2 Graph Sanity Check ===")


graph_count = 0
invalid_graphs = []


for family in PHASE2_FAMILIES:

    for opt, binary_path in VARIANT_BINARIES.items():

        sample_id = f"{family}_{opt}"

        graph = build_pyg_graph(
            binary_path,
            family,
        )

        graph_count += 1


        valid = (
            graph.num_nodes > 0
            and graph.num_node_features == 29
        )


        if not valid:
            invalid_graphs.append(
                sample_id
            )


        print(
            f"{sample_id:28} "
            f"nodes={graph.num_nodes:<3} "
            f"edges={graph.num_edges:<3} "
            f"features={graph.num_node_features:<3} "
            f"{'OK' if valid else 'INVALID'}"
        )


print("\n=== Summary ===")

print(
    "Total graphs:",
    graph_count,
)

print(
    "Invalid graphs:",
    len(invalid_graphs),
)


if invalid_graphs:

    print("\nInvalid samples:")

    for sample_id in invalid_graphs:
        print(sample_id)

else:

    print(
        "\nAll Phase-2 graphs are valid."
    )