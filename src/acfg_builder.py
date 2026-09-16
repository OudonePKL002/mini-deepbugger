import re
import subprocess
import networkx as nx
from collections import Counter

import torch
from torch_geometric.data import Data


FEATURE_ORDER = [
    "load",
    "store",

    "arithmetic_add",
    "arithmetic_sub",
    "arithmetic_mul",

    "logical_shift_right",

    "stack_management",
    "compare",

    "branch_eq",
    "branch_ne",
    "branch_gt",
    "branch_ge",
    "branch_lt",
    "branch_le",

    "unconditional_branch",

    "cset_eq",
    "cset_ne",
    "cset_gt",
    "cset_ge",
    "cset_lt",
    "cset_le",

    "test_bit_zero",
    "test_bit_nonzero",

    "predicate_gt_zero",
    "predicate_lt_zero",

    "call",
    "return",
    "move",
    "other",
]

# --------------------------------------------------
# 1. Disassemble binary
# --------------------------------------------------

def disassemble_binary(binary_path):
    result = subprocess.run(
        ["otool", "-tvV", binary_path],
        capture_output=True,
        text=True,
        check=True,
    )

    return result.stdout


# --------------------------------------------------
# 2. Extract function with addresses
# --------------------------------------------------

def extract_function(disassembly, function_name):
    lines = disassembly.splitlines()

    target = f"_{function_name}:"

    instructions = []
    inside_function = False

    for line in lines:
        stripped = line.strip()

        if stripped == target:
            inside_function = True
            continue

        if inside_function:

            # Next function symbol
            if stripped.endswith(":"):
                break

            match = re.match(
                r"^([0-9a-fA-F]+)\s+(.+)$",
                stripped,
            )

            if match:
                address = int(
                    match.group(1),
                    16,
                )

                instruction = match.group(2)

                instructions.append(
                    (address, instruction)
                )

    return instructions


# --------------------------------------------------
# 3. Helpers
# --------------------------------------------------

def get_opcode(instruction):
    return instruction.split()[0].lower()


def get_branch_target(instruction):
    match = re.search(
        r"0x([0-9a-fA-F]+)",
        instruction,
    )

    if not match:
        return None

    return int(match.group(1), 16)


def is_conditional_branch(opcode):
    return (
        opcode.startswith("b.")
        or opcode in {
            "cbz",
            "cbnz",
            "tbz",
            "tbnz",
        }
    )


def is_unconditional_branch(opcode):
    return opcode == "b"


# --------------------------------------------------
# 4. Find Basic Block leaders
# --------------------------------------------------

def find_leaders(instructions):

    if not instructions:
        return []

    leaders = {
        instructions[0][0]
    }

    addresses = [
        address
        for address, _ in instructions
    ]

    address_set = set(addresses)

    for i, (address, instruction) in enumerate(instructions):

        opcode = get_opcode(instruction)

        if (
            is_conditional_branch(opcode)
            or is_unconditional_branch(opcode)
        ):

            target = get_branch_target(
                instruction
            )

            if target in address_set:
                leaders.add(target)

            # Instruction after branch
            if i + 1 < len(instructions):
                leaders.add(
                    instructions[i + 1][0]
                )

    return sorted(leaders)


# --------------------------------------------------
# 5. Build Basic Blocks
# --------------------------------------------------

def build_basic_blocks(
    instructions,
    leaders,
):

    leader_set = set(leaders)

    blocks = []
    current_block = []

    for address, instruction in instructions:

        if (
            address in leader_set
            and current_block
        ):
            blocks.append(current_block)
            current_block = []

        current_block.append(
            (address, instruction)
        )

    if current_block:
        blocks.append(current_block)

    return blocks

# --------------------------------------------------
# 6. Build CFG edges
# --------------------------------------------------

def build_cfg(blocks):

    graph = nx.DiGraph()

    address_to_block = {}

    for index, block in enumerate(blocks):

        block_name = f"BB{index}"

        graph.add_node(
            block_name,
            instructions=block,
        )

        for address, _ in block:
            address_to_block[address] = block_name


    for index, block in enumerate(blocks):

        block_name = f"BB{index}"

        _, last_instruction = block[-1]

        opcode = get_opcode(
            last_instruction
        )

        # Conditional branch
        if is_conditional_branch(opcode):

            target = get_branch_target(
                last_instruction
            )

            if target in address_to_block:
                graph.add_edge(
                    block_name,
                    address_to_block[target],
                )

            # Fall-through
            if index + 1 < len(blocks):
                graph.add_edge(
                    block_name,
                    f"BB{index + 1}",
                )

        # Unconditional branch
        elif is_unconditional_branch(opcode):

            target = get_branch_target(
                last_instruction
            )

            if target in address_to_block:
                graph.add_edge(
                    block_name,
                    address_to_block[target],
                )

        # Return
        elif opcode == "ret":
            pass

        # Normal fall-through
        else:
            if index + 1 < len(blocks):
                graph.add_edge(
                    block_name,
                    f"BB{index + 1}",
                )

    return graph


def categorize_instruction(instruction):

    parts = instruction.strip().split()

    if not parts:
        return "other"

    opcode = parts[0].lower()

    if opcode in {"sub", "add"} and len(parts) > 1:
        if parts[1].startswith("sp"):
            return "stack_management"

    if opcode in {"ldr", "ldur"}:
        return "load"

    if opcode in {"str", "stur"}:
        return "store"

    if opcode in {"add", "adds"}:
        return "arithmetic_add"

    if opcode == "sub":
        return "arithmetic_sub"

    if opcode in {"mul", "madd", "msub"}:
        return "arithmetic_mul"

    if opcode in {"cmp", "cmn", "subs"}:
        return "compare"

    branch_map = {
        "b.eq": "branch_eq",
        "b.ne": "branch_ne",
        "b.gt": "branch_gt",
        "b.ge": "branch_ge",
        "b.lt": "branch_lt",
        "b.le": "branch_le",
    }

    if opcode in branch_map:
        return branch_map[opcode]

    if opcode == "tbz":
        return "test_bit_zero"

    if opcode == "tbnz":
        return "test_bit_nonzero"

    if opcode in {"cset", "csetm"}:

        condition = parts[-1].lower()

        cset_map = {
            "eq": "cset_eq",
            "ne": "cset_ne",
            "gt": "cset_gt",
            "ge": "cset_ge",
            "lt": "cset_lt",
            "le": "cset_le",
        }

        return cset_map.get(condition, "other")

    if opcode in {"b", "br"}:
        return "unconditional_branch"

    if opcode in {"bl", "blr"}:
        return "call"

    if opcode == "ret":
        return "return"

    if opcode in {"mov", "movz", "movk"}:
        return "move"

    if opcode == "lsr":
        return "logical_shift_right"

    return "other"


def detect_semantic_predicates(instructions):

    predicates = []

    normalized = [
        inst.strip().lower()
        for inst in instructions
    ]

    for i, inst in enumerate(normalized):

        # 1. cmp/subs with zero
        is_zero_compare = (
            inst.startswith(("subs", "cmp"))
            and (
                inst.rstrip().endswith("#0x0")
                or inst.rstrip().endswith("#0")
            )
        )

        if is_zero_compare and i + 1 < len(normalized):

            next_inst = normalized[i + 1]

            if next_inst.startswith("cset"):

                condition = (
                    next_inst
                    .split()[-1]
                    .strip(",")
                )

                if condition == "gt":
                    predicates.append(
                        "predicate_gt_zero"
                    )

                elif condition == "lt":
                    predicates.append(
                        "predicate_lt_zero"
                    )

            elif next_inst.startswith("b.le"):
                predicates.append(
                    "predicate_gt_zero"
                )

            elif next_inst.startswith("b.ge"):
                predicates.append(
                    "predicate_lt_zero"
                )


        # 2. CMN + CSET
        if inst.startswith("cmn") and i + 1 < len(normalized):

            next_inst = normalized[i + 1]

            if next_inst.startswith("cset"):

                condition = (
                    next_inst
                    .split()[-1]
                    .strip(",")
                )

                if condition == "gt":
                    predicates.append(
                        "predicate_gt_zero"
                    )

                elif condition == "lt":
                    predicates.append(
                        "predicate_lt_zero"
                    )


        # 3. TBZ sign-bit pattern
        if (
            inst.startswith("tbz")
            and "#0x1f" in inst
        ):
            predicates.append(
                "predicate_lt_zero"
            )


        # 4. LSR sign-bit extraction
        if inst.startswith("lsr"):

            compact = inst.replace(" ", "")

            if (
                "#31" in compact
                or "#0x1f" in compact
            ):
                predicates.append(
                    "predicate_lt_zero"
                )

    return predicates


def extract_block_features(block):

    instructions = [
        instruction
        for _, instruction in block
    ]

    categories = [
        categorize_instruction(inst)
        for inst in instructions
    ]

    predicates = detect_semantic_predicates(
        instructions
    )

    counts = Counter(
        categories + predicates
    )

    return [
        counts.get(feature, 0)
        for feature in FEATURE_ORDER
    ]


def build_pyg_graph(binary_path, function_name):

    disassembly = disassemble_binary(
        binary_path
    )

    instructions = extract_function(
        disassembly,
        function_name,
    )

    leaders = find_leaders(
        instructions
    )

    blocks = build_basic_blocks(
        instructions,
        leaders,
    )

    cfg = build_cfg(
        blocks
    )

    node_names = list(cfg.nodes())

    node_to_id = {
        node: idx
        for idx, node in enumerate(node_names)
    }

    node_features = []

    for node in node_names:

        block = cfg.nodes[node]["instructions"]

        features = extract_block_features(
            block
        )

        node_features.append(
            features
        )

    x = torch.tensor(
        node_features,
        dtype=torch.float32,
    )

    edge_pairs = [
        (
            node_to_id[src],
            node_to_id[dst],
        )
        for src, dst in cfg.edges()
    ]

    if edge_pairs:

        edge_index = torch.tensor(
            edge_pairs,
            dtype=torch.long,
        ).t().contiguous()

    else:

        edge_index = torch.empty(
            (2, 0),
            dtype=torch.long,
        )

    data = Data(
        x=x,
        edge_index=edge_index,
    )

    data.function_name = function_name

    return data
