from itertools import combinations

import torch
import torch.nn.functional as F

from acfg_builder import build_pyg_graph
from graph_encoder_demo import GraphEncoder


VARIANTS = {
    "O0": "sandbox/variants/hello_O0",
    "O1": "sandbox/variants/hello_O1",
    "O2": "sandbox/variants/hello_O2",
    "O3": "sandbox/variants/hello_O3",
}

FUNCTION_NAME = "add"


# --------------------------------------------------
# 1. Build graphs
# --------------------------------------------------

graphs = {}

for opt, binary_path in VARIANTS.items():

    graph = build_pyg_graph(
        binary_path,
        FUNCTION_NAME,
    )

    graphs[opt] = graph


# --------------------------------------------------
# 2. Simple graph-level feature baseline
#    For a one-node graph, mean == node feature vector.
# --------------------------------------------------

def graph_feature_vector(graph):

    return graph.x.mean(
        dim=0,
        keepdim=True,
    )


# --------------------------------------------------
# 3. Random graph encoder
#    Sanity check only — NOT trained.
# --------------------------------------------------

torch.manual_seed(42)

model = GraphEncoder(
    input_dim=28,
    hidden_dim=32,
    embedding_dim=16,
)

model.eval()


def graph_embedding(graph):

    batch = torch.zeros(
        graph.num_nodes,
        dtype=torch.long,
    )

    with torch.no_grad():

        embedding = model(
            graph.x,
            graph.edge_index,
            batch,
        )

    return embedding


# --------------------------------------------------
# 4. Compare all optimization pairs
# --------------------------------------------------

pairs = list(
    combinations(VARIANTS.keys(), 2)
)


print("=== add() Optimization Variant Similarity ===")


for a, b in pairs:

    # Raw A-CFG feature similarity
    feature_a = graph_feature_vector(
        graphs[a]
    )

    feature_b = graph_feature_vector(
        graphs[b]
    )

    feature_similarity = (
        F.cosine_similarity(
            feature_a,
            feature_b,
        ).item()
    )


    # Random GNN embedding similarity
    embedding_a = graph_embedding(
        graphs[a]
    )

    embedding_b = graph_embedding(
        graphs[b]
    )

    embedding_similarity = (
        F.cosine_similarity(
            embedding_a,
            embedding_b,
        ).item()
    )


    print(f"\n{a} vs {b}")

    print(
        "A-CFG feature similarity:",
        round(feature_similarity, 4),
    )

    print(
        "Random GNN similarity   :",
        round(embedding_similarity, 4),
    )