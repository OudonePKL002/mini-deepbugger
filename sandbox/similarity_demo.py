import torch
import torch.nn.functional as F


function_a = torch.tensor([
    [0.0000, 0.5370, 0.2232, 0.0103]
])

# Similar function
function_b = torch.tensor([
    [0.0200, 0.5100, 0.2100, 0.0300]
])

# Different function
function_c = torch.tensor([
    [0.7000, 0.0500, 0.1000, 0.6000]
])


similarity_ab = F.cosine_similarity(
    function_a,
    function_b
)

similarity_ac = F.cosine_similarity(
    function_a,
    function_c
)


print("=== Mini-DeepClone Similarity Demo ===")

print("\nFunction A vs Function B:")
print(similarity_ab.item())

print("\nFunction A vs Function C:")
print(similarity_ac.item())