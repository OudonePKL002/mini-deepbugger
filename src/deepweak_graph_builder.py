from __future__ import annotations

import copy

import torch

from src.acfg_builder import (
    disassemble_binary,
    extract_function,
    find_leaders,
    build_basic_blocks,
    build_pyg_graph,
    get_opcode,
)

from src.deepweak_features import (
    detect_security_features,
    SECURITY_FEATURE_ORDER,
)


SHARED_FEATURE_DIM = 29
SECURITY_FEATURE_DIM = len(
    SECURITY_FEATURE_ORDER
)

DEEPWEAK_FEATURE_DIM = (
    SHARED_FEATURE_DIM
    + SECURITY_FEATURE_DIM
)


def build_deepweak_graph(
    binary_path: str,
    function_name: str,
):
    """
    Build a Mini-DeepWeak A-CFG.

    Shared Mini-DeepBugger representation:
        29-D semantic A-CFG features

    DeepWeak extension:
        10-D security-aware features

    Final:
        39-D node features

    IMPORTANT:
    This wrapper does NOT modify the frozen
    Mini-DeepClone acfg_builder.py.
    """

    # ======================================================
    # 1. Build frozen shared graph (29-D)
    # ======================================================

    shared_graph = build_pyg_graph(
        binary_path,
        function_name,
    )


    if (
        shared_graph.num_node_features
        != SHARED_FEATURE_DIM
    ):
        raise RuntimeError(
            f"Expected {SHARED_FEATURE_DIM}-D "
            f"shared features, got "
            f"{shared_graph.num_node_features}"
        )


    # ======================================================
    # 2. Reuse existing binary parser
    # ======================================================

    disassembly = disassemble_binary(
        binary_path
    )


    instructions = extract_function(
        disassembly,
        function_name,
    )


    if not instructions:
        raise RuntimeError(
            f"No instructions extracted for "
            f"{function_name}"
        )


    leaders = find_leaders(
        instructions
    )


    blocks = build_basic_blocks(
        instructions,
        leaders,
    )


    # ======================================================
    # 3. Validate block alignment
    #
    # shared_graph.x has one row per basic block.
    # ======================================================

    if len(blocks) != shared_graph.num_nodes:

        raise RuntimeError(
            "Basic-block alignment mismatch\n"
            f"Function: {function_name}\n"
            f"Blocks: {len(blocks)}\n"
            f"Graph nodes: "
            f"{shared_graph.num_nodes}"
        )


    # ======================================================
    # 4. Security-aware features
    #
    # Block-local information is combined with
    # function-level security context.
    # ======================================================

    all_function_instructions = [
        instruction
        for _, instruction in instructions
    ]

    function_security = detect_security_features(
        all_function_instructions
    )


    security_vectors = []


    for block in blocks:

        block_instructions = [
            instruction
            for _, instruction in block
        ]


        block_security = detect_security_features(
            block_instructions
        )


        # Start from block-local features.
        security_vector = block_security.clone()


        # --------------------------------------------------
        # Function-context propagation
        #
        # Some security relations span basic blocks.
        #
        # Example:
        #
        # Block A:
        #     cbz denominator
        #
        # Block B:
        #     sdiv ...
        #
        # Function-level detector sees guarded division.
        # Propagate zero_guard onto the division block.
        # --------------------------------------------------

        block_opcodes = []

        from src.acfg_builder import get_opcode

        for instruction in block_instructions:
            block_opcodes.append(
                get_opcode(instruction)
            )


        # Feature indices
        feature_index = {
            name: i
            for i, name in enumerate(
                SECURITY_FEATURE_ORDER
            )
        }


        # --------------------------------------------------
        # Guarded division
        # --------------------------------------------------

        has_division_here = any(
            opcode in {
                "sdiv",
                "udiv",
            }
            for opcode in block_opcodes
        )


        if (
            has_division_here
            and function_security[
                feature_index["zero_guard"]
            ] == 1
        ):
            security_vector[
                feature_index["zero_guard"]
            ] = 1.0

        # ======================================================
        # Null-check propagation
        #
        # Function pattern:
        #     cbz pointer
        #          ↓
        #     dereference
        #
        # Attach null_check to the block that performs
        # the pointer dereference.
        # ======================================================

        has_pointer_dereference_here = any(
            (
                opcode in {
                    "ldr",
                    "ldrb",
                    "ldrh",
                    "ldrsw",
                    "str",
                    "strb",
                    "strh",
                }
                and "[x" in instruction.lower()
            )
            for opcode, instruction
            in zip(
                block_opcodes,
                block_instructions,
            )
        )


        if (
            has_pointer_dereference_here
            and function_security[
                feature_index["null_check"]
            ] == 1
        ):
            security_vector[
                feature_index["null_check"]
            ] = 1.0

        # ======================================================
        # Bounds-check propagation
        #
        # Function pattern:
        #     cmp index, limit
        #     conditional branch
        #          ↓
        #     indexed memory access
        #
        # Attach bounds_check to the block that performs
        # the indexed access.
        # ======================================================

        has_indexed_memory_here = any(
            (
                opcode in {
                    "ldr",
                    "ldrb",
                    "ldrh",
                    "ldrsw",
                    "str",
                    "strb",
                    "strh",
                }
                and (
                    "sxtw" in instruction.lower()
                    or "uxtw" in instruction.lower()
                )
            )
            for opcode, instruction
            in zip(
                block_opcodes,
                block_instructions,
            )
        )


        if (
            has_indexed_memory_here
            and function_security[
                feature_index["bounds_check"]
            ] == 1
        ):
            security_vector[
                feature_index["bounds_check"]
            ] = 1.0


        # --------------------------------------------------
        # Other function-level API semantics
        #
        # These can safely be preserved if detected
        # inside this block already.
        # More CFG-aware relations will be added only
        # after validation.
        # --------------------------------------------------


        security_vectors.append(
            security_vector
        )

    # ======================================================
    # Convert security vectors to matrix
    #
    # Shape:
    # [num_nodes, 10]
    # ======================================================

    security_matrix = torch.stack(
        security_vectors,
        dim=0,
    )


    if (
        security_matrix.shape[0]
        != shared_graph.num_nodes
    ):
        raise RuntimeError(
            "Security feature node count mismatch.\n"
            f"Expected nodes: {shared_graph.num_nodes}\n"
            f"Security rows: {security_matrix.shape[0]}"
        )


    if (
        security_matrix.shape[1]
        != SECURITY_FEATURE_DIM
    ):
        raise RuntimeError(
            "Security feature dimension mismatch.\n"
            f"Expected: {SECURITY_FEATURE_DIM}\n"
            f"Actual: {security_matrix.shape[1]}"
        )

    # ======================================================
    # 5. Combine 29-D + 10-D
    # ======================================================

    graph = copy.deepcopy(
        shared_graph
    )


    graph.x = torch.cat(
        [
            shared_graph.x,
            security_matrix,
        ],
        dim=1,
    )


    # ======================================================
    # 6. Final validation
    # ======================================================

    if (
        graph.num_node_features
        != DEEPWEAK_FEATURE_DIM
    ):

        raise RuntimeError(
            f"Expected "
            f"{DEEPWEAK_FEATURE_DIM}-D "
            f"DeepWeak features, got "
            f"{graph.num_node_features}"
        )


    return graph