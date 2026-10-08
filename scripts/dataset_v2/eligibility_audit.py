#!/usr/bin/env python3
"""
ARM64 Canonical Function Eligibility Counting Rule (ARM64_BOUNDARY_V1).
Author: Mini-DeepBugger Research Team
"""
import re
from typing import List, Tuple, Dict, Any, Set

RULE_VERSION = "ARM64_BOUNDARY_V1"

CALLEE_SAVED_REGS: Set[str] = {
    f"x{i}" for i in range(19, 31) # x19 through x30
}

def clean_instruction_text(inst_str: str) -> str:
    """Strips comments and normalizes whitespace."""
    # Remove comments starting with ; or //
    line = re.sub(r";.*$", "", inst_str)
    line = re.sub(r"//.*$", "", line)
    return " ".join(line.strip().split())

def parse_opcode_operands(inst_str: str) -> Tuple[str, str]:
    """Extracts lowercase opcode and operand string."""
    cleaned = clean_instruction_text(inst_str)
    if not cleaned:
        return "", ""
    parts = cleaned.split(None, 1)
    opcode = parts[0].lower()
    operands = parts[1] if len(parts) > 1 else ""
    return opcode, operands

def is_sp_based_memory(operand: str) -> bool:
    """Checks if memory operand has explicit sp base, e.g., [sp or [sp, #...]."""
    compact = operand.replace(" ", "").lower()
    return bool(re.search(r"\[sp\b", compact))

def is_prologue_instruction(inst_str: str) -> bool:
    """
    Checks if instruction matches the ARM64_BOUNDARY_V1 prologue whitelist.
    """
    opcode, operands = parse_opcode_operands(inst_str)
    if not opcode:
        return False

    # A. Pointer-authentication entry operations
    if opcode in {"paciasp", "pacibsp"}:
        return True

    # B. Stack-frame allocation: sub sp, sp, #<immediate>
    if opcode == "sub":
        # match sub sp, sp, #...
        compact = operands.replace(" ", "").lower()
        if re.match(r"^sp,sp,#", compact):
            return True

    # C. Frame-pointer establishment: mov x29, sp or add x29, sp, #<imm>
    if opcode == "mov":
        compact = operands.replace(" ", "").lower()
        if compact == "x29,sp":
            return True
    if opcode == "add":
        compact = operands.replace(" ", "").lower()
        if re.match(r"^x29,sp,#", compact) or compact == "x29,sp":
            return True

    # D. Saving link/frame/callee-saved general registers (x19..x30) to SP-based frame
    if opcode in {"stp", "str", "stur"}:
        # Check memory base
        if not is_sp_based_memory(operands):
            return False
        # Extract register operands
        # operands for stp: reg1, reg2, [sp, ...]
        # operands for str: reg1, [sp, ...]
        parts = [p.strip() for p in operands.split(",")]
        if opcode == "stp" and len(parts) >= 3:
            r1 = parts[0].lower()
            r2 = parts[1].lower()
            if r1 in CALLEE_SAVED_REGS and r2 in CALLEE_SAVED_REGS:
                return True
        elif opcode in {"str", "stur"} and len(parts) >= 2:
            r1 = parts[0].lower()
            if r1 in CALLEE_SAVED_REGS:
                return True

    return False

def is_epilogue_instruction(inst_str: str) -> bool:
    """
    Checks if instruction matches the ARM64_BOUNDARY_V1 epilogue whitelist.
    """
    opcode, operands = parse_opcode_operands(inst_str)
    if not opcode:
        return False

    # A. Standard returns
    if opcode in {"ret", "retaa", "retab"}:
        compact = operands.replace(" ", "").lower()
        if compact in {"", "x30", "lr"}:
            return True

    # B. Pointer-authentication exit operations
    if opcode in {"autiasp", "autibsp"}:
        return True

    # C. Stack-frame deallocation: add sp, sp, #<immediate>
    if opcode == "add":
        compact = operands.replace(" ", "").lower()
        if re.match(r"^sp,sp,#", compact):
            return True

    # D. Frame-pointer restoration: mov sp, x29
    if opcode == "mov":
        compact = operands.replace(" ", "").lower()
        if compact == "sp,x29":
            return True

    # E. Restoring frame/link/callee-saved general registers (x19..x30) from SP-based frame
    if opcode in {"ldp", "ldr", "ldur"}:
        if not is_sp_based_memory(operands):
            return False
        parts = [p.strip() for p in operands.split(",")]
        if opcode == "ldp" and len(parts) >= 3:
            r1 = parts[0].lower()
            r2 = parts[1].lower()
            if r1 in CALLEE_SAVED_REGS and r2 in CALLEE_SAVED_REGS:
                return True
        elif opcode in {"ldr", "ldur"} and len(parts) >= 2:
            r1 = parts[0].lower()
            if r1 in CALLEE_SAVED_REGS:
                return True

    return False

def classify_boundary_frame_instructions(instructions: List[Tuple[int, str]]) -> Dict[str, Any]:
    """
    Applies the ARM64_BOUNDARY_V1 prefix/suffix boundary exclusion rule.
    instructions: list of (address, instruction_string)
    """
    n_total = len(instructions)
    if n_total == 0:
        return {
            "total_instruction_count": 0,
            "prologue_instruction_indices": [],
            "epilogue_instruction_indices": [],
            "prologue_instruction_count": 0,
            "epilogue_instruction_count": 0,
            "eligible_instruction_count": 0,
            "rule_version": RULE_VERSION
        }

    # 1. Forward scan contiguous prefix starting at index 0
    prologue_indices = []
    for idx in range(n_total):
        _, inst_str = instructions[idx]
        if is_prologue_instruction(inst_str):
            prologue_indices.append(idx)
        else:
            break # Stop contiguous prefix scan

    prologue_set = set(prologue_indices)

    # 2. Backward scan contiguous suffix ending at n_total - 1
    epilogue_indices = []
    for idx in range(n_total - 1, -1, -1):
        if idx in prologue_set:
            break # Do not overlap with prologue
        _, inst_str = instructions[idx]
        if is_epilogue_instruction(inst_str):
            epilogue_indices.append(idx)
        else:
            break # Stop contiguous suffix scan

    epilogue_indices.reverse() # Sort in ascending order

    p_count = len(prologue_indices)
    e_count = len(epilogue_indices)
    eligible_count = n_total - p_count - e_count

    return {
        "total_instruction_count": n_total,
        "prologue_instruction_indices": prologue_indices,
        "epilogue_instruction_indices": epilogue_indices,
        "prologue_instruction_count": p_count,
        "epilogue_instruction_count": e_count,
        "eligible_instruction_count": eligible_count,
        "rule_version": RULE_VERSION
    }
