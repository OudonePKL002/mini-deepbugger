import torch
from torch_geometric.data import Data
from torch_geometric.nn import GCNConv


# --------------------------------------------------
# Graph data
# --------------------------------------------------

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


# --------------------------------------------------
# Simple GCN model
# --------------------------------------------------

class SimpleGCN(torch.nn.Module):

    def __init__(self):
        super().__init__()

        self.conv1 = GCNConv(
            in_channels=9,
            out_channels=8,
        )

        self.conv2 = GCNConv(
            in_channels=8,
            out_channels=4,
        )

    def forward(self, x, edge_index):

        x = self.conv1(x, edge_index)
        x = torch.relu(x)

        x = self.conv2(x, edge_index)

        return x


# --------------------------------------------------
# Run forward pass
# --------------------------------------------------

model = SimpleGCN()

output = model(
    data.x,
    data.edge_index,
)


print("=== Mini-DeepBugger GNN Forward Demo ===")

print("\nInput shape:")
print(data.x.shape)

print("\nOutput node embeddings:")
print(output)

print("\nOutput shape:")
print(output.shape)