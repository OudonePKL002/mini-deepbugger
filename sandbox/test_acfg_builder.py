from acfg_builder import build_pyg_graph


BINARY_PATH = "sandbox/hello"


functions = [
    "add",
    "add_similar",

    "subtract",
    "subtract_similar",

    "multiply",
    "multiply_similar",

    "maximum",
    "maximum_similar",

    "minimum",
    "minimum_similar",

    "is_positive",
    "is_positive_similar",

    "is_negative",
    "is_negative_similar",

    "identity",
    "identity_similar",
]


print("=== Mini-DeepBugger A-CFG Dataset Check ===")


graphs = {}


for name in functions:

    graph = build_pyg_graph(
        BINARY_PATH,
        name,
    )

    graphs[name] = graph

    print(
        f"{name:22} "
        f"nodes={graph.num_nodes:<3} "
        f"edges={graph.num_edges:<3} "
        f"features={graph.num_node_features}"
    )


print("\nTotal graphs:", len(graphs))