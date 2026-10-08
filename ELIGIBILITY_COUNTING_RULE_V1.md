# ARM64 Canonical Function Eligibility Counting Rule (Version ARM64_BOUNDARY_V1)

**Rule Identifier:** `ARM64_BOUNDARY_V1`  
**Governance Status:** FROZEN — HUMAN-APPROVED IMPLEMENTATION SPECIFICATION  
**Associated Protocol Revision:** Revision 2.2.1 / Decision 1 Operationalization  
**Creation Date:** 2026-10-08  
**Target Architecture & Toolchain:** Apple Clang (`/usr/bin/clang`), `-O0`, ARM64, boundary-preserving (`__attribute__((noinline))`)

---

## 1. Scientific & Governance Purpose

This document provides the authoritative, deterministic operationalization of the already-frozen Decision 1 phrase:
$$\text{"instruction count } \ge 3 \text{ excluding function prologue/epilogue"}$$

### Crucial Pre-Measurement Assurance
This rule is formally established and frozen **BEFORE** inspecting any candidate function eligibility outcomes, before calculating $\mathcal{F}_{\text{canonical}}(Q)$, and before synthesizing any partial-reuse targets. It does not alter the scientific threshold of Decision 1 ($|V| \ge 1 \text{ and } N_{\text{inst\_eligible}} \ge 3$), does not depend on model performance, does not alter canonical source code, and introduces no post-hoc bias.

---

## 2. Core Counting Definition

Let a parsed disassembly of function $f$ contain an ordered sequence of $N_{\text{total}}$ instructions indexed $0, 1, \dots, N_{\text{total}} - 1$:

1. $N_{\text{total}}$: The total number of instructions parsed by `extract_function` from `otool -tvV` output.
2. $P \subset \{0, \dots, N_{\text{total}} - 1\}$: The index set of instructions recognized as canonical prologue.
3. $E \subset \{0, \dots, N_{\text{total}} - 1\}$: The index set of instructions recognized as canonical epilogue.

The eligible instruction count is defined strictly as:
$$N_{\text{inst\_eligible}} = N_{\text{total}} - |P \cup E|$$

Each instruction index is removed at most once. If no recognized prologue or epilogue pattern matches, $P = \emptyset$ and/or $E = \emptyset$. No heuristic guessing or speculative elimination is permitted.

---

## 3. Critical Boundary Rule

To prevent functional program logic from being discarded merely because it uses the stack or return-like opcodes:

1. **Prologue Recognition (Contiguous Prefix):**  
   Prologue scanning operates **ONLY** on a contiguous prefix beginning strictly at instruction index $0$. The scanner advances while each instruction satisfies the **Prologue Whitelist**. Scanning **TERMINATES** at the very first instruction that does not match the whitelist.
2. **Epilogue Recognition (Contiguous Suffix):**  
   Epilogue scanning operates **ONLY** on a contiguous suffix ending strictly at the final instruction index $N_{\text{total}} - 1$. The scanner scans backward while each instruction satisfies the **Epilogue Whitelist**. Scanning **TERMINATES** at the very first instruction (scanning backward) that does not match the whitelist.
3. **Internal Instruction Preservation:**  
   Instructions matching stack-management or return patterns that appear in the middle of a function body (after prologue termination or before epilogue onset) **MUST NEVER** be removed.

---

## 4. Normalization for Matching

Prior to whitelist matching, instruction text is normalized:
- Mnemonic converted to lowercase.
- Whitespace normalized (collapsed to single spaces).
- Address and hex byte dump prefixes already stripped by the `extract_function` parser.
- Comments and inline assembler annotations stripped.
- Operand register names and immediate constants preserved.
- Register semantics preserved (no speculative aliasing).

Matching is evaluated based on `(normalized_mnemonic, normalized_operands)`.

---

## 5. Prologue Whitelist (ARM64)

Scanning forward from index $0$, instructions are consumed into $P$ if and only if they match one of the following canonical frame-management forms:

### A. Pointer-Authentication Entry Operations
- `paciasp`
- `pacibsp`

### B. Stack-Frame Allocation
- `sub sp, sp, #<immediate>`

### C. Frame-Pointer Establishment
- `mov x29, sp`
- `add x29, sp, #<immediate>`

### D. Saving Frame Record / Callee-Saved Registers to SP-Based Frame
- `stp <reg1>, <reg2>, [sp, ...]`
- `str <reg1>, [sp, ...]`
- `stur <reg1>, [sp, ...]`

**Strict Register Constraint for Store Operations:**
- Only callee-saved / frame general-purpose registers: `x19` through `x30` (including `x29` frame pointer and `x30` link register) are accepted.
- The memory base register must explicitly be `sp`.

> **IMPORTANT RULE:** Stores involving argument or scratch registers (`x0` through `x18`, `w0` through `w18`) targeting `[sp, ...]` **MUST NOT** be classified as prologue. These represent argument spills or body variables and remain counted.

---

## 6. Epilogue Whitelist (ARM64)

Scanning backward from index $N_{\text{total}} - 1$, instructions are consumed into $E$ if and only if they match one of the following canonical teardown forms:

### A. Standard Returns
- `ret`
- `ret x30`
- `retaa`
- `retab`

### B. Pointer-Authentication Exit Operations
- `autiasp`
- `autibsp`

### C. Stack-Frame Deallocation
- `add sp, sp, #<immediate>`

### D. Frame-Pointer Restoration
- `mov sp, x29`

### E. Restoring Frame Record / Callee-Saved Registers from SP-Based Frame
- `ldp <reg1>, <reg2>, [sp, ...]`
- `ldr <reg1>, [sp, ...]`
- `ldur <reg1>, [sp, ...]`

**Strict Register Constraint for Load Operations:**
- Only callee-saved / frame general-purpose registers: `x19` through `x30` (including `x29` and `x30`) are accepted.
- The memory base register must explicitly be `sp`.

---

## 7. Conservative Non-Match Policy

The rule is intentionally conservative to prevent false exclusions:
- Arbitrary `ldr`/`str`/`ldp`/`stp` instructions involving `x0`–`x18` are **NOT** excluded.
- Local variable stack slots and argument spill operations remain **COUNTED**.
- Stack operations occurring after body logic has begun remain **COUNTED**.
- Branch instructions (`b`, `bl`, `b.<cond>`, `cbz`, `cbnz`, `tbz`, `tbnz`) remain **COUNTED**.
- Arithmetic or logic instructions unrelated to frame pointer setup remain **COUNTED**.

**Default Decision:** If an instruction's role is ambiguous, it is **KEPT AND COUNTED**.

---

## 8. Multiple Return Path Policy

In functions with multiple exit paths:
- Only the **terminal contiguous suffix** ending at index $N_{\text{total}} - 1$ is excluded as the canonical epilogue.
- Early or intermediate `ret` instructions embedded within earlier basic blocks remain **COUNTED** as body instructions.
- Rationale: The non-triviality filter does not require full control-flow ABI reconstruction; a deterministic boundary scan guarantees strict reproducibility.

---

## 9. Leaf Function Policy

Leaf functions that allocate no stack frame (e.g., pure register computations) may have $|P| = 0$ and $|E| = 1$ (consisting solely of terminal `ret`).
- Example:
  ```assembly
  add w0, w0, #1
  ret
  ```
  Yields: $N_{\text{total}} = 2$, $|P| = 0$, $|E| = 1$, $N_{\text{inst\_eligible}} = 1$.
  This fails the non-triviality requirement ($N_{\text{inst\_eligible}} \ge 3$).
- Frame instructions must **NEVER** be artificially invented for leaf functions.

---

## 10. Basic Block Count Invariance

Decision 1 requires:
$$|V| \ge 1 \quad \text{and} \quad N_{\text{inst\_eligible}} \ge 3$$

The basic block count $|V|$ is determined by:
$$|V| = \text{len}(\text{build\_basic\_blocks}(\text{instructions}, \text{leaders}))$$
Prologue/epilogue exclusion applies **ONLY** to $N_{\text{inst\_eligible}}$. It does **NOT** alter the Control Flow Graph or delete nodes/edges from the CFG for the purposes of $|V|$.

---

## 11. Implementation & Audit Fields

When evaluating canonical reference binaries in Phase 2B, the deterministic extraction helper must log the following fields for every candidate function in `data/v2/manifests/dataset_v2_canonical_eligibility.csv`:

1. `instruction_count_total`: Total parsed instruction count ($N_{\text{total}}$).
2. `prologue_instruction_count`: Number of excluded prologue instructions ($|P|$).
3. `epilogue_instruction_count`: Number of excluded epilogue instructions ($|E|$).
4. `prologue_instruction_indices`: Comma-separated list of 0-based indices in $P$.
5. `epilogue_instruction_indices`: Comma-separated list of 0-based indices in $E$.
6. `instruction_count_eligible`: Resulting eligible count ($N_{\text{inst\_eligible}}$).
7. `prologue_epilogue_rule_version`: Fixed string `ARM64_BOUNDARY_V1`.

---

## 12. Freeze Status

- **Rule Identifier:** `ARM64_BOUNDARY_V1`
- **Frozen Protocol Alignment:** Aligns exactly with Revision 2.2.1 / Decision 1 Option 2.
- **Verification Guarantee:** Frozen prior to compiling canonical reference binaries and measuring candidate function eligibility.
