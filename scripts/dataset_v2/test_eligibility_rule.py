#!/usr/bin/env python3
"""
Synthetic Self-Tests for ARM64_BOUNDARY_V1 Eligibility Counting Rule.
Author: Mini-DeepBugger Research Team
"""
import sys
import os

# Add repo root to sys.path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../..")))

from scripts.dataset_v2.eligibility_audit import classify_boundary_frame_instructions

def run_self_tests():
    print("==================================================")
    print("RUNNING ARM64_BOUNDARY_V1 SYNTHETIC SELF-TESTS")
    print("==================================================")

    # ----------------------------------------------------
    # Test A: Standard framed function
    # ----------------------------------------------------
    seq_a = [
        (0x1000, "sub sp, sp, #32"),
        (0x1004, "stp x29, x30, [sp, #16]"),
        (0x1008, "add x29, sp, #16"),
        (0x100c, "mov w0, #42"),
        (0x1010, "add w0, w0, #1"),
        (0x1014, "str w0, [sp, #8]"),
        (0x1018, "ldp x29, x30, [sp, #16]"),
        (0x101c, "add sp, sp, #32"),
        (0x1020, "ret")
    ]
    res_a = classify_boundary_frame_instructions(seq_a)
    assert res_a["total_instruction_count"] == 9
    assert res_a["prologue_instruction_indices"] == [0, 1, 2]
    assert res_a["epilogue_instruction_indices"] == [6, 7, 8]
    assert res_a["prologue_instruction_count"] == 3
    assert res_a["epilogue_instruction_count"] == 3
    assert res_a["eligible_instruction_count"] == 3
    print("Test A (Standard framed function): PASSED")

    # ----------------------------------------------------
    # Test B: Argument spill
    # ----------------------------------------------------
    seq_b = [
        (0x1000, "sub sp, sp, #32"),
        (0x1004, "str w0, [sp, #12]"), # w0 is NOT callee-saved, prefix scan MUST stop here!
        (0x1008, "stp x29, x30, [sp, #16]"), # this is now internal, must remain counted!
        (0x100c, "mov w0, #10"),
        (0x1010, "add sp, sp, #32"),
        (0x1014, "ret")
    ]
    res_b = classify_boundary_frame_instructions(seq_b)
    assert res_b["total_instruction_count"] == 6
    assert res_b["prologue_instruction_indices"] == [0] # only index 0!
    assert res_b["epilogue_instruction_indices"] == [4, 5]
    assert res_b["eligible_instruction_count"] == 3
    print("Test B (Argument spill breaks prologue prefix): PASSED")

    # ----------------------------------------------------
    # Test C: Internal stack access (local variable)
    # ----------------------------------------------------
    seq_c = [
        (0x1000, "sub sp, sp, #16"),
        (0x1004, "mov w0, #1"),
        (0x1008, "str w0, [sp]"), # w0 store (local var), not callee-saved! Must remain counted.
        (0x100c, "ldr w0, [sp]"), # w0 load (local var), not callee-saved! Must remain counted.
        (0x1010, "add sp, sp, #16"),
        (0x1014, "ret")
    ]
    res_c = classify_boundary_frame_instructions(seq_c)
    assert res_c["prologue_instruction_indices"] == [0]
    assert res_c["epilogue_instruction_indices"] == [4, 5]
    assert res_c["eligible_instruction_count"] == 3 # indices 1, 2, 3
    print("Test C (Internal stack access preserved): PASSED")

    # ----------------------------------------------------
    # Test D: Leaf function
    # ----------------------------------------------------
    seq_d = [
        (0x1000, "add w0, w0, #1"),
        (0x1004, "ret")
    ]
    res_d = classify_boundary_frame_instructions(seq_d)
    assert res_d["total_instruction_count"] == 2
    assert res_d["prologue_instruction_count"] == 0
    assert res_d["epilogue_instruction_count"] == 1
    assert res_d["eligible_instruction_count"] == 1 # 1 < 3 -> would fail Decision 1
    print("Test D (Leaf function): PASSED")

    # ----------------------------------------------------
    # Test E: Multiple / early return
    # ----------------------------------------------------
    seq_e = [
        (0x1000, "sub sp, sp, #16"),
        (0x1004, "cmp w0, #0"),
        (0x1008, "b.ne 0x1014"),
        (0x100c, "ret"), # Early return in body! Must remain counted.
        (0x1010, "mov w0, #42"),
        (0x1014, "add sp, sp, #16"),
        (0x1018, "ret")
    ]
    res_e = classify_boundary_frame_instructions(seq_e)
    assert res_e["prologue_instruction_indices"] == [0]
    assert res_e["epilogue_instruction_indices"] == [5, 6]
    # Indices 1, 2, 3, 4 remain counted
    assert res_e["eligible_instruction_count"] == 4
    print("Test E (Early/multiple return preserved): PASSED")

    # ----------------------------------------------------
    # Test F: No recognized frame
    # ----------------------------------------------------
    seq_f = [
        (0x1000, "mov w0, #1"),
        (0x1004, "mov w1, #2"),
        (0x1008, "add w0, w0, w1"),
        (0x100c, "b 0x2000") # Tail call branch, not ret
    ]
    res_f = classify_boundary_frame_instructions(seq_f)
    assert res_f["prologue_instruction_count"] == 0
    assert res_f["epilogue_instruction_count"] == 0
    assert res_f["eligible_instruction_count"] == 4
    print("Test F (No recognized frame preserved): PASSED")

    # ----------------------------------------------------
    # Test G: Pointer-authentication sequence
    # ----------------------------------------------------
    seq_g = [
        (0x1000, "paciasp"),
        (0x1004, "sub sp, sp, #16"),
        (0x1008, "mov w0, #1"),
        (0x100c, "mov w1, #2"),
        (0x1010, "add w0, w0, w1"),
        (0x1014, "add sp, sp, #16"),
        (0x1018, "autiasp"),
        (0x101c, "ret")
    ]
    res_g = classify_boundary_frame_instructions(seq_g)
    assert res_g["prologue_instruction_indices"] == [0, 1]
    assert res_g["epilogue_instruction_indices"] == [5, 6, 7]
    assert res_g["eligible_instruction_count"] == 3
    print("Test G (PAC/AUT boundary handling): PASSED")

    print("\nALL 7 SYNTHETIC SELF-TESTS PASSED PERFECTLY!")

if __name__ == "__main__":
    run_self_tests()
