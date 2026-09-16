import torch

from torch_geometric.data import Data
from torch_geometric.nn import GCNConv


x = torch.randn(
    1,
    28,
)

edge_index = torch.empty(
    (2, 0),
    dtype=torch.long,
)


data = Data(
    x=x,
    edge_index=edge_index,
)


conv = GCNConv(
    in_channels=28,
    out_channels=16,
)


output = conv(
    data.x,
    data.edge_index,
)


print(
    "=== Single Node GCN Test ==="
)

print(
    "Input:",
    data.x.shape,
)

print(
    "Edges:",
    data.edge_index.shape,
)

print(
    "Output:",
    output.shape,
)

print(output)