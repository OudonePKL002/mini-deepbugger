from acfg_builder import build_pyg_graph


VARIANTS = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}


FUNCTION_NAME = "add"


print("=== Mini-DeepClone Optimization Variant Check ===")


graphs = {}


for opt, binary_path in VARIANTS.items():

    graph = build_pyg_graph(
        binary_path,
        FUNCTION_NAME,
    )

    graphs[opt] = graph

    print(
        f"{FUNCTION_NAME}_{opt:2} "
        f"nodes={graph.num_nodes:<2} "
        f"edges={graph.num_edges:<2} "
        f"features={graph.num_node_features}"
    )


print("\nTotal variants:", len(graphs))