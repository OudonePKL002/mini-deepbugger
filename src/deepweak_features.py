from __future__ import annotations

from typing import List
from src.acfg_builder import get_opcode
import torch


SECURITY_FEATURE_ORDER = [
    "unsafe_copy_call",
    "bounded_copy_call",
    "null_check",
    "bounds_check",
    "division",
    "zero_guard",
    "arithmetic_overflow_guard",
    "range_guard",
    "dynamic_format_call",
    "literal_format_call",
]


def _clean_instruction(line: str) -> str:
    """
    Normalize one disassembly line.

    The detector is intentionally simple because
    Mini-DeepWeak PoC v0.1 targets the controlled W1 dataset.
    """

    return line.strip().lower()


def detect_security_features(
    instructions: List[str],
) -> torch.Tensor:

    """
    Return a 10-D security-aware feature vector.

    These features are an extension for Mini-DeepWeak only.
    They do NOT modify the frozen 29-D Mini-DeepClone features.
    """

    cleaned = [
        _clean_instruction(line)
        for line in instructions
    ]

    joined = "\n".join(cleaned)

    opcodes = [
        get_opcode(line)
        for line in cleaned
    ]

    features = {
        name: 0.0
        for name in SECURITY_FEATURE_ORDER
    }

    # ======================================================
    # 1. Unsafe copy API
    # ======================================================

    if "_strcpy" in joined:
        features["unsafe_copy_call"] = 1.0


    # ======================================================
    # 2. Bounded copy API
    # ======================================================

    if "_strncpy" in joined:
        features["bounded_copy_call"] = 1.0


    # ======================================================
    # 3. Null pointer guard
    #
    # Observed:
    # cbz x0, ...
    # ldr ..., [x0]
    # ======================================================

    has_cbz_pointer = any(
        opcode == "cbz"
        and "x" in line.split("\t", 1)[-1]
        for opcode, line in zip(
            opcodes,
            cleaned,
        )
    )

    has_pointer_load = any(
        "ldr" in line
        and "[x" in line
        for line in cleaned
    )

    if (
        has_cbz_pointer
        and has_pointer_load
    ):
        features["null_check"] = 1.0


    # ======================================================
    # 4. Bounds check
    #
    # Observed patterns:
    # cmp index, limit
    # b.ge / b.ls / conditional branch
    # followed by indexed memory access
    # ======================================================

    has_compare = (
        "cmp" in opcodes
    )

    CONDITIONAL_BRANCH_OPCODES = {
        "b.eq",
        "b.ne",
        "b.ge",
        "b.gt",
        "b.le",
        "b.lt",
        "b.hi",
        "b.hs",
        "b.lo",
        "b.ls",
        "cbz",
        "cbnz",
        "tbz",
        "tbnz",
    }

    has_conditional_branch = any(
        opcode in CONDITIONAL_BRANCH_OPCODES
        for opcode in opcodes
    )

    has_indexed_memory = any(
        (
            "ldr" in line
            or "str" in line
        )
        and (
            "sxtw" in line
            or "uxtw" in line
        )
        for line in cleaned
    )

    if (
        has_compare
        and has_conditional_branch
        and has_indexed_memory
    ):
        features["bounds_check"] = 1.0


    # ======================================================
    # 5. Division
    # ======================================================

    has_division = any(
        opcode in {
            "sdiv",
            "udiv",
        }
        for opcode in opcodes
    )

    if has_division:
        features["division"] = 1.0


    # ======================================================
    # 6. Zero guard
    #
    # Observed:
    # cbz denominator, ...
    # sdiv ...
    # ======================================================

    has_zero_check = any(
        opcode in {
            "cbz",
            "cbnz",
        }
        for opcode in opcodes
    )

    if (
        has_division
        and has_zero_check
    ):
        features["zero_guard"] = 1.0


    # ======================================================
    # 7. Arithmetic overflow guard
    #
    # W1 observed integer_overflow_good:
    # compare + conditional branch / conditional select
    # around arithmetic operation.
    # ======================================================

    has_arithmetic = any(
        opcode in {
            "add",
            "adds",
            "sub",
            "subs",
            "mul",
        }
        for opcode in opcodes
    )

    has_conditional_select = any(
        opcode in {
            "csel",
            "csinc",
            "cset",
        }
        for opcode in opcodes
    )

    if (
        has_arithmetic
        and has_compare
        and (
            has_conditional_branch
            or has_conditional_select
        )
    ):
        features[
            "arithmetic_overflow_guard"
        ] = 1.0


    # ======================================================
    # 8. Range guard
    #
    # Used for range checks around multiplication.
    # ======================================================

    has_mul = (
        "mul" in opcodes
    )

    if (
        has_mul
        and has_compare
        and (
            has_conditional_branch
            or has_conditional_select
        )
    ):
        features["range_guard"] = 1.0


    # ======================================================
    # 9-10. printf format semantics
    #
    # Bad:
    # input directly used as x0 before _printf
    #
    # Good:
    # literal pool / "%s" loaded as format string.
    # ======================================================

    calls_printf = "_printf" in joined

    has_format_literal = (
        '"%s"' in joined
        or "'%s'" in joined
        or "literal pool for: \"%s\"" in joined
    )

    if calls_printf:

        if has_format_literal:
            features[
                "literal_format_call"
            ] = 1.0

        else:
            features[
                "dynamic_format_call"
            ] = 1.0


    vector = torch.tensor(
        [
            features[name]
            for name
            in SECURITY_FEATURE_ORDER
        ],
        dtype=torch.float32,
    )

    return vector