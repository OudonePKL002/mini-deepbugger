from collections import Counter
import networkx as nx


# --------------------------------------------------
# 1. Instruction category definition
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


def get_opcode(instruction):
    return instruction.strip().split()[0].lower()


def categorize_opcode(opcode):

    if opcode in {"ldr", "ldur"}:
        return "load"

    if opcode in {"str", "stur"}:
        return "store"

    if opcode in {"add", "sub", "adds", "subs", "mul"}:
        return "arithmetic"

    if opcode.startswith("b."):
        return "conditional_branch"

    if opcode in {"b", "br"}:
        return "unconditional_branch"

    if opcode in {"bl", "blr"}:
        return "call"

    if opcode == "ret":
        return "return"

    if opcode in {"mov", "movz", "movk"}:
        return "move"

    return "other"


# --------------------------------------------------
# 2. Convert instructions → feature vector
# --------------------------------------------------

def extract_features(instructions):

    categories = []

    for instruction in instructions:
        opcode = get_opcode(instruction)
        category = categorize_opcode(opcode)
        categories.append(category)

    counts = Counter(categories)

    feature_vector = [
        counts.get(feature_name, 0)
        for feature_name in FEATURE_ORDER
    ]

    return feature_vector


# --------------------------------------------------
# 3. Basic blocks from classify()
# --------------------------------------------------

basic_blocks = {

    "BB0": [
        "sub sp, sp, #0x10",
        "str w0, [sp, #0x8]",
        "ldr w8, [sp, #0x8]",
        "subs w8, w8, #0xa",
        "b.le 0x1000004a4",
    ],

    "BB1": [
        "b 0x100000498",
    ],

    "BB2": [
        "mov w8, #0x1",
        "str w8, [sp, #0xc]",
        "b 0x1000004ac",
    ],

    "BB3": [
        "str wzr, [sp, #0xc]",
        "b 0x1000004ac",
    ],

    "BB4": [
        "ldr w0, [sp, #0xc]",
        "add sp, sp, #0x10",
        "ret",
    ],
}


# --------------------------------------------------
# 4. Create A-CFG
# --------------------------------------------------

G = nx.DiGraph()

for block_name, instructions in basic_blocks.items():

    features = extract_features(instructions)

    G.add_node(
        block_name,
        instructions=instructions,
        features=features,
    )


# --------------------------------------------------
# 5. Control-flow edges
# --------------------------------------------------

G.add_edge("BB0", "BB1")
G.add_edge("BB0", "BB3")

G.add_edge("BB1", "BB2")

G.add_edge("BB2", "BB4")
G.add_edge("BB3", "BB4")


# --------------------------------------------------
# 6. Print results
# --------------------------------------------------

print("=== Mini-DeepBugger A-CFG v0.1 ===")

print("\nFeature Order:")
print(FEATURE_ORDER)


print("\nNode Features:")

for node, data in G.nodes(data=True):

    print(f"\n{node}")

    print("Instructions:")

    for instruction in data["instructions"]:
        print("  ", instruction)

    print("Feature Vector:")
    print(" ", data["features"])


print("\nEdges:")

for src, dst in G.edges():
    print(src, "->", dst)


print("\nGraph Statistics:")
print("Nodes:", G.number_of_nodes())
print("Edges:", G.number_of_edges())