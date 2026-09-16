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
# 1. Train pairs
# --------------------------------------------------

TRAIN_POSITIVE_PAIRS = [
    ("add", "add_similar"),
    ("subtract", "subtract_similar"),
    ("multiply", "multiply_similar"),
    ("maximum", "maximum_similar"),
    ("minimum", "minimum_similar"),
]


TRAIN_NEGATIVE_PAIRS = [
    ("add", "maximum"),
    ("add", "minimum"),
    ("subtract", "maximum"),
    ("multiply", "minimum"),
    ("maximum", "subtract"),
    ("minimum", "multiply"),
]


# --------------------------------------------------
# 2. Test pairs
# --------------------------------------------------

TEST_POSITIVE_PAIRS = [
    ("is_positive", "is_positive_similar"),
    ("is_negative", "is_negative_similar"),
    ("identity", "identity_similar"),
]


TEST_NEGATIVE_PAIRS = [
    ("is_positive", "is_negative_similar"),
    ("is_negative", "identity_similar"),
    ("identity", "is_positive_similar"),
]


# --------------------------------------------------
# 3. Collect functions
# --------------------------------------------------

ALL_PAIRS = (
    TRAIN_POSITIVE_PAIRS
    + TRAIN_NEGATIVE_PAIRS
    + TEST_POSITIVE_PAIRS
    + TEST_NEGATIVE_PAIRS
)


function_names = sorted(
    {
        name
        for pair in ALL_PAIRS
        for name in pair
    }
)


# --------------------------------------------------
# 4. Build graphs
# --------------------------------------------------

graphs = {}


print("=== Building A-CFG Graphs ===")


for name in function_names:

    graph = build_pyg_graph(
        BINARY_PATH,
        name,
    )

    graphs[name] = graph

    print(
        f"{name:22} "
        f"nodes={graph.num_nodes:<2} "
        f"edges={graph.num_edges:<2}"
    )


# --------------------------------------------------
# 5. Model
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
# 6. Encode graph
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
# 7. Evaluate pairs
# --------------------------------------------------

def evaluate_pair_set(
    positive_pairs,
    negative_pairs,
):

    model.eval()

    positive_scores = []
    negative_scores = []

    with torch.no_grad():

        for a, b in positive_pairs:

            emb_a = encode_graph(
                graphs[a]
            )

            emb_b = encode_graph(
                graphs[b]
            )

            score = F.cosine_similarity(
                emb_a,
                emb_b,
            ).item()

            positive_scores.append(
                score
            )


        for a, b in negative_pairs:

            emb_a = encode_graph(
                graphs[a]
            )

            emb_b = encode_graph(
                graphs[b]
            )

            score = F.cosine_similarity(
                emb_a,
                emb_b,
            ).item()

            negative_scores.append(
                score
            )


    mean_positive = (
        sum(positive_scores)
        / len(positive_scores)
    )

    mean_negative = (
        sum(negative_scores)
        / len(negative_scores)
    )

    separation = (
        mean_positive
        - mean_negative
    )

    return (
        mean_positive,
        mean_negative,
        separation,
        positive_scores,
        negative_scores,
    )


# --------------------------------------------------
# 8. Before training
# --------------------------------------------------

print("\n=== Before Training ===")


train_before = evaluate_pair_set(
    TRAIN_POSITIVE_PAIRS,
    TRAIN_NEGATIVE_PAIRS,
)


test_before = evaluate_pair_set(
    TEST_POSITIVE_PAIRS,
    TEST_NEGATIVE_PAIRS,
)


print("\nTrain")
print(
    "Positive:",
    round(train_before[0], 4),
)
print(
    "Negative:",
    round(train_before[1], 4),
)
print(
    "Separation:",
    round(train_before[2], 4),
)


print("\nTest")
print(
    "Positive:",
    round(test_before[0], 4),
)
print(
    "Negative:",
    round(test_before[1], 4),
)
print(
    "Separation:",
    round(test_before[2], 4),
)


# --------------------------------------------------
# 9. Training
# --------------------------------------------------

EPOCHS = 100


training_pairs = []


for a, b in TRAIN_POSITIVE_PAIRS:
    training_pairs.append(
        (a, b, 1.0)
    )


for a, b in TRAIN_NEGATIVE_PAIRS:
    training_pairs.append(
        (a, b, -1.0)
    )


print("\n=== Training ===")


for epoch in range(
    1,
    EPOCHS + 1,
):

    model.train()

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

        avg_loss = (
            total_loss
            / len(training_pairs)
        )

        print(
            f"Epoch {epoch:3d} "
            f"Loss={avg_loss:.4f}"
        )


# --------------------------------------------------
# 10. After training
# --------------------------------------------------

print("\n=== After Training ===")


train_after = evaluate_pair_set(
    TRAIN_POSITIVE_PAIRS,
    TRAIN_NEGATIVE_PAIRS,
)


test_after = evaluate_pair_set(
    TEST_POSITIVE_PAIRS,
    TEST_NEGATIVE_PAIRS,
)


print("\nTrain")
print(
    "Positive:",
    round(train_after[0], 4),
)
print(
    "Negative:",
    round(train_after[1], 4),
)
print(
    "Separation:",
    round(train_after[2], 4),
)


print("\nTest")
print(
    "Positive:",
    round(test_after[0], 4),
)
print(
    "Negative:",
    round(test_after[1], 4),
)
print(
    "Separation:",
    round(test_after[2], 4),
)


# --------------------------------------------------
# 11. Detailed test scores
# --------------------------------------------------

print("\n=== Test Positive Pairs ===")

for pair, score in zip(
    TEST_POSITIVE_PAIRS,
    test_after[3],
):

    print(
        f"{pair[0]:20} "
        f"vs {pair[1]:20} "
        f"{score:.4f}"
    )


print("\n=== Test Negative Pairs ===")

for pair, score in zip(
    TEST_NEGATIVE_PAIRS,
    test_after[4],
):

    print(
        f"{pair[0]:20} "
        f"vs {pair[1]:20} "
        f"{score:.4f}"
    )