import re
import subprocess
import networkx as nx


BINARY_PATH = "sandbox/hello"
FUNCTION_NAME = "is_positive_similar"


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


# --------------------------------------------------
# 7. Run
# --------------------------------------------------

disassembly = disassemble_binary(
    BINARY_PATH
)

instructions = extract_function(
    disassembly,
    FUNCTION_NAME,
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


print(
    f"=== Automatic CFG: {FUNCTION_NAME} ==="
)

print("\nBasic Blocks:")

for node, data in cfg.nodes(data=True):

    print(f"\n{node}")

    for address, instruction in data["instructions"]:
        print(
            hex(address),
            instruction,
        )


print("\nEdges:")

for src, dst in cfg.edges():
    print(
        src,
        "->",
        dst,
    )


print("\nGraph Statistics:")
print(
    "Nodes:",
    cfg.number_of_nodes(),
)

print(
    "Edges:",
    cfg.number_of_edges(),
)