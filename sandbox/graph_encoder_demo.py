import torch
from torch_geometric.nn import GCNConv, global_mean_pool


class GraphEncoder(torch.nn.Module):

    def __init__(
        self,
        input_dim=28,
        hidden_dim=32,
        embedding_dim=16,
    ):
        super().__init__()

        self.conv1 = GCNConv(
            input_dim,
            hidden_dim,
        )

        self.conv2 = GCNConv(
            hidden_dim,
            embedding_dim,
        )

    def forward(
        self,
        x,
        edge_index,
        batch,
    ):

        x = self.conv1(
            x,
            edge_index,
        )

        x = torch.relu(x)

        x = self.conv2(
            x,
            edge_index,
        )

        x = torch.relu(x)

        graph_embedding = global_mean_pool(
            x,
            batch,
        )

        return graph_embedding