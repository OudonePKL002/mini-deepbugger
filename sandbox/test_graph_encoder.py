import torch

from acfg_builder import build_pyg_graph
from graph_encoder_demo import GraphEncoder


BINARY_PATH = "sandbox/hello"


FUNCTIONS = [
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


# Reproducibility for this demo
torch.manual_seed(42)


model = GraphEncoder(
    input_dim=29,
    hidden_dim=32,
    embedding_dim=16,
)

model.eval()


embeddings = {}


print("=== Mini-DeepClone Graph Encoder Test ===")


with torch.no_grad():

    for name in FUNCTIONS:

        graph = build_pyg_graph(
            BINARY_PATH,
            name,
        )

        # All nodes belong to one graph
        batch = torch.zeros(
            graph.num_nodes,
            dtype=torch.long,
        )

        embedding = model(
            graph.x,
            graph.edge_index,
            batch,
        )

        embeddings[name] = embedding.squeeze(0)

        print(
            f"{name:22} "
            f"graph=({graph.num_nodes} nodes, "
            f"{graph.num_edges} edges) "
            f"embedding={tuple(embedding.shape)}"
        )


print("\nTotal embeddings:", len(embeddings))