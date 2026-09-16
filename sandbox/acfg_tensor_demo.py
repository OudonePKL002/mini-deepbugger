import torch


# --------------------------------------------------
# 1. Node feature matrix
# --------------------------------------------------

FEATURE_ORDER = [
    "load",
    "store",
    "arithmetic",
    "conditional_branch",
    "unconditional_branch",
    "call",
    "return",
    "move",
    "other",
]


node_features = [
    # BB0
    [1, 1, 2, 1, 0, 0, 0, 0, 0],

    # BB1
    [0, 0, 0, 0, 1, 0, 0, 0, 0],

    # BB2
    [0, 1, 0, 0, 1, 0, 0, 1, 0],

    # BB3
    [0, 1, 0, 0, 1, 0, 0, 0, 0],

    # BB4
    [1, 0, 1, 0, 0, 0, 1, 0, 0],
]


x = torch.tensor(
    node_features,
    dtype=torch.float32
)


# --------------------------------------------------
# 2. Node ID mapping
# --------------------------------------------------

node_to_id = {
    "BB0": 0,
    "BB1": 1,
    "BB2": 2,
    "BB3": 3,
    "BB4": 4,
}


# --------------------------------------------------
# 3. Graph edges
# --------------------------------------------------

edges = [
    ("BB0", "BB1"),
    ("BB0", "BB3"),
    ("BB1", "BB2"),
    ("BB2", "BB4"),
    ("BB3", "BB4"),
]


source_nodes = []
target_nodes = []

for src, dst in edges:
    source_nodes.append(node_to_id[src])
    target_nodes.append(node_to_id[dst])


edge_index = torch.tensor(
    [
        source_nodes,
        target_nodes,
    ],
    dtype=torch.long,
)


# --------------------------------------------------
# 4. Print results
# --------------------------------------------------

print("=== Mini-DeepBugger PyTorch Graph ===")


print("\nFeature Order:")
print(FEATURE_ORDER)


print("\nNode Feature Matrix (x):")
print(x)


print("\nx shape:")
print(x.shape)


print("\nNode Mapping:")
for node, idx in node_to_id.items():
    print(node, "->", idx)


print("\nEdge Index:")
print(edge_index)


print("\nedge_index shape:")
print(edge_index.shape)