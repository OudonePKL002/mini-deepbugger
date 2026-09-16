import torch
from torch_geometric.data import Data


x = torch.tensor(
    [
        [1, 1, 2, 1, 0, 0, 0, 0, 0],  # BB0
        [0, 0, 0, 0, 1, 0, 0, 0, 0],  # BB1
        [0, 1, 0, 0, 1, 0, 0, 1, 0],  # BB2
        [0, 1, 0, 0, 1, 0, 0, 0, 0],  # BB3
        [1, 0, 1, 0, 0, 0, 1, 0, 0],  # BB4
    ],
    dtype=torch.float32,
)


edge_index = torch.tensor(
    [
        [0, 0, 1, 2, 3],
        [1, 3, 2, 4, 4],
    ],
    dtype=torch.long,
)


data = Data(
    x=x,
    edge_index=edge_index,
)


print("=== Mini-DeepBugger PyG Data Object ===")

print(data)

print("\nNode feature matrix:")
print(data.x)

print("\nEdge index:")
print(data.edge_index)

print("\nNumber of nodes:")
print(data.num_nodes)

print("\nNumber of edges:")
print(data.num_edges)

print("\nNumber of node features:")
print(data.num_node_features)