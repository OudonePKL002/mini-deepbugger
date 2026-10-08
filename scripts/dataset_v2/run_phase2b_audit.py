#!/usr/bin/env python3
"""
Phase 2B — Canonical Reference Build & Final F_canonical Eligibility Freeze
AsiaCCS 2027 Mini-DeepBugger Research Program

This script:
1. Validates the frozen canonical source corpus integrity.
2. Runs the ARM64_BOUNDARY_V1 synthetic self-tests.
3. Builds the 21 canonical reference binaries using /usr/bin/clang -O0 -Wall -Wextra.
4. Executes the deterministic unit-test harness for each binary (requiring 21/21 PASS).
5. Disassembles each binary using /usr/bin/otool -tvV.
6. Evaluates Decision 1 eligibility for all 336 planned candidate functions.
7. Produces:
   - data/v2/manifests/dataset_v2_canonical_eligibility.csv
   - data/v2/manifests/dataset_v2_fcanonical.csv
   - data/v2/manifests/dataset_v2_reference_builds.csv
8. Generates:
   - data/v2/checksums/CANONICAL_ELIGIBILITY_SHA256SUMS.txt
"""

import os
import sys
import csv
import json
import hashlib
import subprocess
from typing import Dict, List, Tuple, Any

# Ensure repo root is on sys.path
REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))
sys.path.insert(0, REPO_ROOT)

from src.acfg_builder import extract_function, find_leaders, build_basic_blocks
from scripts.dataset_v2.eligibility_audit import classify_boundary_frame_instructions, RULE_VERSION
from scripts.dataset_v2.test_eligibility_rule import run_self_tests

COMPILER_PATH = "/usr/bin/clang"
OTOOL_PATH = "/usr/bin/otool"
COMPILE_FLAGS = ["-O0", "-Wall", "-Wextra"]
LINK_FLAGS = ["-lm"]
ARCHITECTURE = "ARM64"
OPTIMIZATION = "-O0"
EXPECTED_PYTHON = "3.12.12"

def sha256_file(filepath: str) -> str:
    hasher = hashlib.sha256()
    with open(filepath, "rb") as f:
        while chunk := f.read(65536):
            hasher.update(chunk)
    return hasher.hexdigest()

def get_toolchain_info() -> Tuple[str, str, str]:
    # clang info
    clang_res = subprocess.run([COMPILER_PATH, "--version"], capture_output=True, text=True, check=True)
    clang_version_line = clang_res.stdout.splitlines()[0].strip()
    target_triple = "unknown"
    for line in clang_res.stdout.splitlines():
        if "Target:" in line:
            target_triple = line.split("Target:")[1].strip()
            break
            
    # otool info
    otool_res = subprocess.run([OTOOL_PATH, "--version"], capture_output=True, text=True)
    otool_text = otool_res.stdout.strip() or otool_res.stderr.strip()
    otool_version_line = otool_text.splitlines()[0].strip() if otool_text else "unknown"
    
    return clang_version_line, target_triple, otool_version_line

def verify_source_checksums():
    checksum_file = os.path.join(REPO_ROOT, "data/v2/checksums/CANONICAL_SOURCE_SHA256SUMS.txt")
    with open(checksum_file, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            expected_sha, rel_path = line.split()
            full_path = os.path.join(REPO_ROOT, rel_path)
            actual_sha = sha256_file(full_path)
            assert actual_sha == expected_sha, f"Checksum mismatch for {rel_path}: expected {expected_sha}, got {actual_sha}"
    print("[PASS] Frozen canonical source checksums verified (0 mismatches).")

def main():
    print("==================================================")
    print("STARTING PHASE 2B CANONICAL ELIGIBILITY AUDIT")
    print("==================================================")
    
    # 1. Verify source corpus checksums
    verify_source_checksums()
    
    # 2. Run synthetic self-tests
    print("\nRunning ARM64_BOUNDARY_V1 self-tests...")
    run_self_tests()
    
    # 3. Toolchain info
    compiler_version, target_triple, otool_version = get_toolchain_info()
    print(f"\nCompiler: {COMPILER_PATH} ({compiler_version})")
    print(f"Target: {target_triple}")
    print(f"Otool: {OTOOL_PATH} ({otool_version})")
    print(f"Python: {sys.version.split()[0]}")
    
    # 4. Load corpus manifest
    corpus_csv = os.path.join(REPO_ROOT, "data/v2/manifests/dataset_v2_corpus.csv")
    with open(corpus_csv, "r") as f:
        corpus_entries = list(csv.DictReader(f))
    assert len(corpus_entries) == 21, f"Expected 21 corpus programs, found {len(corpus_entries)}"
    
    # 5. Build and validate reference binaries
    ref_builds_dir = os.path.join(REPO_ROOT, "data/v2/reference_builds")
    os.makedirs(ref_builds_dir, exist_ok=True)
    
    ref_build_records = []
    disassembly_map = {}
    
    print("\nBuilding and testing 21 reference binaries...")
    for prog in corpus_entries:
        pid = prog["program_id"]
        pname = prog["program_name"]
        split = prog["split"]
        src_path = os.path.join(REPO_ROOT, prog["source_file"])
        test_path = os.path.join(REPO_ROOT, prog["test_file"])
        
        prog_dir = os.path.join(ref_builds_dir, pname)
        os.makedirs(prog_dir, exist_ok=True)
        bin_path = os.path.join(prog_dir, f"{pname}_ref")
        dis_path = os.path.join(prog_dir, f"{pname}_ref.dis")
        test_out_path = os.path.join(prog_dir, "test_output.txt")
        
        src_sha = sha256_file(src_path)
        
        # Compile command
        cmd = [COMPILER_PATH] + COMPILE_FLAGS + [src_path, test_path, "-o", bin_path] + LINK_FLAGS
        build_res = subprocess.run(cmd, capture_output=True, text=True)
        if build_res.returncode != 0:
            print(f"BUILD FAILED for {pname}:\n{build_res.stderr}")
            sys.exit(1)
            
        # Run unit tests
        test_res = subprocess.run([bin_path], capture_output=True, text=True)
        with open(test_out_path, "w") as tf:
            tf.write(test_res.stdout)
            if test_res.stderr:
                tf.write("\nSTDERR:\n" + test_res.stderr)
                
        if test_res.returncode != 0:
            print(f"TEST FAILED for {pname} (exit {test_res.returncode}):\n{test_res.stderr}")
            sys.exit(1)
            
        # Disassemble
        dis_res = subprocess.run([OTOOL_PATH, "-tvV", bin_path], capture_output=True, text=True, check=True)
        with open(dis_path, "w") as df:
            df.write(dis_res.stdout)
        disassembly_map[pid] = (dis_res.stdout, os.path.relpath(bin_path, REPO_ROOT), sha256_file(bin_path))
        
        bin_sha = sha256_file(bin_path)
        ref_build_records.append({
            "program_id": pid,
            "split": split,
            "compiler_path": COMPILER_PATH,
            "compiler_version": compiler_version,
            "target_triple": target_triple,
            "architecture": ARCHITECTURE,
            "optimization": OPTIMIZATION,
            "compile_flags": " ".join(COMPILE_FLAGS),
            "link_flags": " ".join(LINK_FLAGS),
            "source_sha256": src_sha,
            "binary_path": os.path.relpath(bin_path, REPO_ROOT),
            "binary_sha256": bin_sha,
            "test_exit_code": test_res.returncode,
            "eligibility_rule_version": RULE_VERSION
        })
        print(f"  [{split:11s}] {pid} ({pname}): BUILD PASS, TEST PASS (exit {test_res.returncode})")
        
    print("\n[PASS] All 21 reference builds succeeded and passed deterministic tests (21/21 PASS).")
    
    # 6. Evaluate candidate functions from identity CSV
    identity_csv = os.path.join(REPO_ROOT, "data/v2/manifests/dataset_v2_function_identity.csv")
    with open(identity_csv, "r") as f:
        candidate_entries = list(csv.DictReader(f))
    assert len(candidate_entries) == 336, f"Expected 336 candidate functions, found {len(candidate_entries)}"
    
    eligibility_records = []
    program_eligible_map = {p["program_id"]: [] for p in corpus_entries}
    
    failure_counts = {
        "extraction_failure": 0,
        "basic_block_count < 1": 0,
        "instruction_count_eligible < 3": 0
    }
    
    print("\nExtracting and auditing 336 candidate benchmark functions...")
    for cand in candidate_entries:
        pid = cand["program_id"]
        split = cand["split"]
        fname = cand["function_name"]
        
        disasm_text, rel_bin_path, bin_sha = disassembly_map[pid]
        instructions = extract_function(disasm_text, fname)
        
        extraction_success = len(instructions) > 0
        if not extraction_success:
            basic_block_count = 0
            n_total = 0
            p_count = 0
            e_count = 0
            p_indices = []
            e_indices = []
            n_eligible = 0
            eligibility_pass = False
            failure_reason = "extraction_failure"
            failure_counts["extraction_failure"] += 1
        else:
            leaders = find_leaders(instructions)
            blocks = build_basic_blocks(instructions, leaders)
            basic_block_count = len(blocks)
            
            audit = classify_boundary_frame_instructions(instructions)
            n_total = audit["total_instruction_count"]
            p_count = audit["prologue_instruction_count"]
            e_count = audit["epilogue_instruction_count"]
            p_indices = audit["prologue_instruction_indices"]
            e_indices = audit["epilogue_instruction_indices"]
            n_eligible = audit["eligible_instruction_count"]
            
            if basic_block_count < 1:
                eligibility_pass = False
                failure_reason = "basic_block_count < 1"
                failure_counts["basic_block_count < 1"] += 1
            elif n_eligible < 3:
                eligibility_pass = False
                failure_reason = "instruction_count_eligible < 3"
                failure_counts["instruction_count_eligible < 3"] += 1
            else:
                eligibility_pass = True
                failure_reason = "NONE"
                program_eligible_map[pid].append(fname)
                
        eligibility_records.append({
            "program_id": pid,
            "split": split,
            "function_id": fname,
            "source_symbol": fname,
            "candidate_function": fname,
            "extraction_success": str(extraction_success),
            "basic_block_count": basic_block_count,
            "instruction_count_total": n_total,
            "prologue_instruction_count": p_count,
            "epilogue_instruction_count": e_count,
            "prologue_instruction_indices": json.dumps(p_indices),
            "epilogue_instruction_indices": json.dumps(e_indices),
            "instruction_count_eligible": n_eligible,
            "prologue_epilogue_rule_version": RULE_VERSION,
            "eligibility_pass": str(eligibility_pass),
            "eligibility_failure_reason": failure_reason,
            "canonical_reference_binary": rel_bin_path,
            "canonical_reference_binary_sha256": bin_sha
        })
        
    total_eligible = sum(1 for r in eligibility_records if r["eligibility_pass"] == "True")
    total_ineligible = len(eligibility_records) - total_eligible
    print(f"Candidate audit complete: {total_eligible}/336 ELIGIBLE, {total_ineligible}/336 INELIGIBLE.")
    
    # 7. Write dataset_v2_canonical_eligibility.csv
    elig_csv_path = os.path.join(REPO_ROOT, "data/v2/manifests/dataset_v2_canonical_eligibility.csv")
    fieldnames_elig = [
        "program_id",
        "split",
        "function_id",
        "source_symbol",
        "candidate_function",
        "extraction_success",
        "basic_block_count",
        "instruction_count_total",
        "prologue_instruction_count",
        "epilogue_instruction_count",
        "prologue_instruction_indices",
        "epilogue_instruction_indices",
        "instruction_count_eligible",
        "prologue_epilogue_rule_version",
        "eligibility_pass",
        "eligibility_failure_reason",
        "canonical_reference_binary",
        "canonical_reference_binary_sha256"
    ]
    with open(elig_csv_path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames_elig)
        writer.writeheader()
        writer.writerows(eligibility_records)
    print(f"Wrote {elig_csv_path}")
    
    # 8. Write dataset_v2_fcanonical.csv
    fcanonical_csv_path = os.path.join(REPO_ROOT, "data/v2/manifests/dataset_v2_fcanonical.csv")
    fcanonical_records = []
    for prog in corpus_entries:
        pid = prog["program_id"]
        split = prog["split"]
        eligible_funcs = program_eligible_map[pid]
        final_m = len(eligible_funcs)
        ineligible_cnt = 16 - final_m
        m_mod_4 = final_m % 4
        exact_quartiles = "YES" if m_mod_4 == 0 else "NO"
        fcanonical_records.append({
            "program_id": pid,
            "split": split,
            "planned_candidate_count": 16,
            "canonical_eligible_count": final_m,
            "ineligible_count": ineligible_cnt,
            "M_mod_4": m_mod_4,
            "exact_quartiles_possible": exact_quartiles,
            "canonical_function_ids": ";".join(eligible_funcs),
            "reference_binary_sha256": disassembly_map[pid][2]
        })
    fieldnames_fcanonical = [
        "program_id",
        "split",
        "planned_candidate_count",
        "canonical_eligible_count",
        "ineligible_count",
        "M_mod_4",
        "exact_quartiles_possible",
        "canonical_function_ids",
        "reference_binary_sha256"
    ]
    with open(fcanonical_csv_path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames_fcanonical)
        writer.writeheader()
        writer.writerows(fcanonical_records)
    print(f"Wrote {fcanonical_csv_path}")
    
    # 9. Write dataset_v2_reference_builds.csv
    ref_builds_csv_path = os.path.join(REPO_ROOT, "data/v2/manifests/dataset_v2_reference_builds.csv")
    fieldnames_ref = [
        "program_id",
        "split",
        "compiler_path",
        "compiler_version",
        "target_triple",
        "architecture",
        "optimization",
        "compile_flags",
        "link_flags",
        "source_sha256",
        "binary_path",
        "binary_sha256",
        "test_exit_code",
        "eligibility_rule_version"
    ]
    with open(ref_builds_csv_path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames_ref)
        writer.writeheader()
        writer.writerows(ref_build_records)
    print(f"Wrote {ref_builds_csv_path}")
    
    # 10. Generate CANONICAL_ELIGIBILITY_SHA256SUMS.txt
    checksum_items = []
    
    # Manifests
    for p in [elig_csv_path, fcanonical_csv_path, ref_builds_csv_path]:
        rel = os.path.relpath(p, REPO_ROOT)
        checksum_items.append((sha256_file(p), rel))
        
    # Scripts
    for script_name in ["eligibility_audit.py", "test_eligibility_rule.py", "run_phase2b_audit.py"]:
        sp = os.path.join(REPO_ROOT, "scripts/dataset_v2", script_name)
        if os.path.exists(sp):
            rel = os.path.relpath(sp, REPO_ROOT)
            checksum_items.append((sha256_file(sp), rel))
            
    # Reference builds (binaries and disassemblies and test outputs)
    for root, _, files in os.walk(ref_builds_dir):
        for file in sorted(files):
            fp = os.path.join(root, file)
            rel = os.path.relpath(fp, REPO_ROOT)
            checksum_items.append((sha256_file(fp), rel))
            
    # Sort deterministically by relative path
    checksum_items.sort(key=lambda x: x[1])
    
    sums_file_path = os.path.join(REPO_ROOT, "data/v2/checksums/CANONICAL_ELIGIBILITY_SHA256SUMS.txt")
    with open(sums_file_path, "w") as sf:
        for csum, rel in checksum_items:
            sf.write(f"{csum}  {rel}\n")
    print(f"Wrote {sums_file_path} ({len(checksum_items)} entries)")
    
    print("\n==================================================")
    print("PHASE 2B AUDIT SUMMARY")
    print("==================================================")
    print(f"Total programs: {len(corpus_entries)}")
    print(f"Total planned candidates: {len(candidate_entries)}")
    print(f"Total eligible functions: {total_eligible}")
    print(f"Total ineligible functions: {total_ineligible}")
    print(f"Failure breakdown: {failure_counts}")
    m_16_count = sum(1 for r in fcanonical_records if r["canonical_eligible_count"] == 16)
    m_mod_0_count = sum(1 for r in fcanonical_records if r["M_mod_4"] == 0)
    print(f"Programs with M = 16: {m_16_count}/21")
    print(f"Programs with M mod 4 = 0: {m_mod_0_count}/21")
    print(f"Programs requiring realized-ratio rounding: {21 - m_mod_0_count}/21")

if __name__ == "__main__":
    main()
