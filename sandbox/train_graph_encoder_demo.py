import random

import torch
import torch.nn.functional as F

from acfg_builder import build_pyg_graph
from graph_encoder_demo import GraphEncoder


BINARY_PATH = "sandbox/hello"

SEED = 42

random.seed(SEED)
torch.manual_seed(SEED)


# --------------------------------------------------
# 1. Positive function pairs
# --------------------------------------------------

POSITIVE_PAIRS = [
    ("add", "add_similar"),
    ("subtract", "subtract_similar"),
    ("multiply", "multiply_similar"),
    ("maximum", "maximum_similar"),
    ("minimum", "minimum_similar"),
    ("is_positive", "is_positive_similar"),
    ("is_negative", "is_negative_similar"),
    ("identity", "identity_similar"),
]


# --------------------------------------------------
# 2. Clear negative pairs
# --------------------------------------------------

NEGATIVE_PAIRS = [
    ("add", "maximum"),
    ("add", "is_positive"),
    ("subtract", "identity"),
    ("multiply", "minimum"),
    ("maximum", "identity"),
    ("minimum", "is_negative"),
    ("is_positive", "multiply"),
    ("is_negative", "add"),
]


# --------------------------------------------------
# 3. Collect all required functions
# --------------------------------------------------

function_names = sorted(
    {
        name
        for pair in POSITIVE_PAIRS + NEGATIVE_PAIRS
        for name in pair
    }
)


# --------------------------------------------------
# 4. Build graphs once
# --------------------------------------------------

graphs = {}

print("=== Building A-CFG Dataset ===")

for name in function_names:

    graph = build_pyg_graph(
        BINARY_PATH,
        name,
    )

    graphs[name] = graph

    print(
        f"{name:22}"
        f" nodes={graph.num_nodes:<2}"
        f" edges={graph.num_edges:<2}"
    )


# --------------------------------------------------
# 5. Graph Encoder
# --------------------------------------------------

model = GraphEncoder(
    input_dim=28,
    hidden_dim=32,
    embedding_dim=16,
)


optimizer = torch.optim.Adam(
    model.parameters(),
    lr=0.01,
)


criterion = torch.nn.CosineEmbeddingLoss(
    margin=0.5
)


# --------------------------------------------------
# 6. Encode one graph
# --------------------------------------------------

def encode_graph(graph):

    batch = torch.zeros(
        graph.num_nodes,
        dtype=torch.long,
    )

    return model(
        graph.x,
        graph.edge_index,
        batch,
    )


# --------------------------------------------------
# 7. Measure current similarities
# --------------------------------------------------

def evaluate_pairs():

    model.eval()

    positive_scores = []
    negative_scores = []

    with torch.no_grad():

        for a, b in POSITIVE_PAIRS:

            emb_a = encode_graph(
                graphs[a]
            )

            emb_b = encode_graph(
                graphs[b]
            )

            similarity = F.cosine_similarity(
                emb_a,
                emb_b,
            ).item()

            positive_scores.append(
                similarity
            )

        for a, b in NEGATIVE_PAIRS:

            emb_a = encode_graph(
                graphs[a]
            )

            emb_b = encode_graph(
                graphs[b]
            )

            similarity = F.cosine_similarity(
                emb_a,
                emb_b,
            ).item()

            negative_scores.append(
                similarity
            )

    mean_positive = sum(
        positive_scores
    ) / len(positive_scores)

    mean_negative = sum(
        negative_scores
    ) / len(negative_scores)

    return (
        mean_positive,
        mean_negative,
    )


# --------------------------------------------------
# 8. Similarity before training
# --------------------------------------------------

before_positive, before_negative = (
    evaluate_pairs()
)


print("\n=== Before Training ===")

print(
    "Mean positive similarity:",
    round(before_positive, 4),
)

print(
    "Mean negative similarity:",
    round(before_negative, 4),
)


# --------------------------------------------------
# 9. Train
# --------------------------------------------------

EPOCHS = 100


print("\n=== Training ===")


for epoch in range(
    1,
    EPOCHS + 1,
):

    model.train()

    training_pairs = []

    for pair in POSITIVE_PAIRS:
        training_pairs.append(
            (pair[0], pair[1], 1.0)
        )

    for pair in NEGATIVE_PAIRS:
        training_pairs.append(
            (pair[0], pair[1], -1.0)
        )

    random.shuffle(
        training_pairs
    )

    total_loss = 0.0


    for a, b, label in training_pairs:

        optimizer.zero_grad()

        emb_a = encode_graph(
            graphs[a]
        )

        emb_b = encode_graph(
            graphs[b]
        )

        target = torch.tensor(
            [label],
            dtype=torch.float32,
        )

        loss = criterion(
            emb_a,
            emb_b,
            target,
        )

        loss.backward()

        optimizer.step()

        total_loss += loss.item()


    if (
        epoch == 1
        or epoch % 10 == 0
    ):

        average_loss = (
            total_loss
            / len(training_pairs)
        )

        print(
            f"Epoch {epoch:3d} "
            f"Loss={average_loss:.4f}"
        )


# --------------------------------------------------
# 10. Evaluate after training
# --------------------------------------------------

after_positive, after_negative = (
    evaluate_pairs()
)


print("\n=== After Training ===")

print(
    "Mean positive similarity:",
    round(after_positive, 4),
)

print(
    "Mean negative similarity:",
    round(after_negative, 4),
)


print("\n=== Separation ===")

print(
    "Before:",
    round(
        before_positive
        - before_negative,
        4,
    )
)

print(
    "After :",
    round(
        after_positive
        - after_negative,
        4,
    )
)