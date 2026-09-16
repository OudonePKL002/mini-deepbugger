import torch
from torch_geometric.data import Data
from torch_geometric.nn import GCNConv, global_mean_pool


x = torch.tensor(
    [
        [1, 1, 2, 1, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 1, 0, 0, 1, 0],
        [0, 1, 0, 0, 1, 0, 0, 0, 0],
        [1, 0, 1, 0, 0, 0, 1, 0, 0],
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


# All 5 nodes belong to graph 0
batch = torch.zeros(
    x.size(0),
    dtype=torch.long,
)


data = Data(
    x=x,
    edge_index=edge_index,
)


class FunctionEncoder(torch.nn.Module):

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

    def forward(self, x, edge_index, batch):

        x = self.conv1(x, edge_index)
        x = torch.relu(x)

        x = self.conv2(x, edge_index)
        x = torch.relu(x)

        graph_embedding = global_mean_pool(
            x,
            batch,
        )

        return x, graph_embedding


model = FunctionEncoder()

node_embeddings, function_embedding = model(
    data.x,
    data.edge_index,
    batch,
)


print("=== Mini-DeepBugger Function Embedding Demo ===")

print("\nNode Embeddings Shape:")
print(node_embeddings.shape)

print("\nFunction Embedding:")
print(function_embedding)

print("\nFunction Embedding Shape:")
print(function_embedding.shape)