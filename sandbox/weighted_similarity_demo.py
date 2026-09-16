import torch
import torch.nn.functional as F


FEATURE_ORDER = [
    "load",
    "store",
    "arithmetic_add",
    "arithmetic_mul",
    "stack_management",
    "conditional_branch",
    "unconditional_branch",
    "call",
    "return",
    "move",
    "other",
]


# Raw feature vectors from the previous experiment
add_vector = torch.tensor(
    [2, 2, 1, 0, 2, 0, 0, 0, 1, 0, 0],
    dtype=torch.float32,
)

add_similar_vector = torch.tensor(
    [2, 2, 1, 0, 2, 0, 0, 0, 1, 0, 0],
    dtype=torch.float32,
)

multiply_vector = torch.tensor(
    [2, 2, 0, 1, 2, 0, 0, 0, 1, 0, 0],
    dtype=torch.float32,
)


# Semantic weights
WEIGHTS = torch.tensor(
    [
        0.5,  # load
        0.5,  # store
        2.0,  # arithmetic_add
        2.0,  # arithmetic_mul
        0.5,  # stack_management
        1.0,  # conditional_branch
        1.0,  # unconditional_branch
        1.0,  # call
        0.5,  # return
        1.0,  # move
        1.0,  # other
    ],
    dtype=torch.float32,
)


def cosine_similarity(a, b):
    return F.cosine_similarity(
        a.unsqueeze(0),
        b.unsqueeze(0),
    ).item()


# Raw similarities
raw_add_add = cosine_similarity(
    add_vector,
    add_similar_vector,
)

raw_add_mul = cosine_similarity(
    add_vector,
    multiply_vector,
)


# Weighted vectors
weighted_add = add_vector * WEIGHTS
weighted_add_similar = add_similar_vector * WEIGHTS
weighted_multiply = multiply_vector * WEIGHTS


# Weighted similarities
weighted_add_add = cosine_similarity(
    weighted_add,
    weighted_add_similar,
)

weighted_add_mul = cosine_similarity(
    weighted_add,
    weighted_multiply,
)


print("=== Mini-DeepClone Weighted Similarity Demo ===")

print("\nFeature Order:")
print(FEATURE_ORDER)

print("\nWeights:")
print(WEIGHTS)

print("\nRaw Vectors:")
print("add        :", add_vector)
print("add_similar:", add_similar_vector)
print("multiply   :", multiply_vector)

print("\nWeighted Vectors:")
print("add        :", weighted_add)
print("add_similar:", weighted_add_similar)
print("multiply   :", weighted_multiply)

print("\n=== Similarity Comparison ===")

print("\nRaw Similarity")
print("add vs add_similar :", raw_add_add)
print("add vs multiply    :", raw_add_mul)

print("\nWeighted Similarity")
print("add vs add_similar :", weighted_add_add)
print("add vs multiply    :", weighted_add_mul)