# DeepBugger Extension Project — Final Master Plan v4

**Project:** DeepBugger — Evidence-Based Detection of Transformed and Partially Reused Binary Code  
**Document role:** Single-source working reference for research, implementation, evaluation, and paper preparation  
**Working date:** 2026-10-22  
**Primary source:** `DeepBugger_연구계획서-261013.docx`  
**Supporting source:** `DeepBugger Proposal Presentation.pptx`  
**Technical baseline:** Mini-DeepBugger / APSEC shared semantic A-CFG framework  

---

# 0. Source Fidelity and How to Use This File

This document intentionally separates three kinds of information.

## 0.1 Proposal-Derived Content

Content explicitly stated in the professor's original proposal.

Examples:

- research problem;
- research questions;
- planned architecture;
- dataset targets;
- planned thresholds;
- evaluation settings;
- timeline;
- target findings;
- limitations;
- ethics;
- Open Science plan.

## 0.2 Presentation Interpretation

Content emphasized or summarized in the user's presentation.

Example:

> C3 / Evidence Aggregator is treated as the main technical core, while C1 and C2 explain why it is needed.

This is a useful research-story interpretation, but it is not a verbatim hierarchy stated in the original proposal.

## 0.3 Working Engineering Decision

Operational decisions added to make the project executable and reproducible.

Examples:

- repository organization;
- freeze gates;
- manifest field names;
- minimum viable pilot datasets;
- implementation order;
- schema validation;
- experiment bookkeeping.

These decisions are derived from the proposal but are not professor-authored requirements.

## 0.4 Source-Safety Rule

If the proposal does not explicitly define a detail:

> **Do not guess it.**

Instead:

1. mark it as unresolved;
2. confirm it with the professor if necessary;
3. freeze the definition in the appropriate schema/protocol before generating data or running final experiments.

---

# 1. Non-Negotiable Research Principle

> **Similar ≠ Reused**

Traditional binary code similarity detection (BCSD) mainly asks:

> “Are these binary functions semantically similar?”

DeepBugger asks a provenance-oriented question:

> “Is there reliable technical evidence that this code came from the same source lineage?”

The distinction must be preserved in:

- ground truth;
- training;
- evaluation;
- aggregation;
- reporting;
- claims in the final paper.

---

# 2. Formal Relation Types

## 2.1 Positive — Same-Lineage Transformed Reuse

Examples:

- same source compiled with different compilers;
- different optimization levels;
- different architectures;
- source-level transformations;
- semantics-preserving transformations.

Label:

> **Reuse positive**

## 2.2 Positive — Partial Reuse

Examples:

- only part of a program is reused;
- only part of a function is reused;
- inlining changes function boundaries;
- splitting/outlining changes boundaries;
- wrappers are added.

Label:

> **Reuse positive with reuse-ratio / region ground truth**

## 2.3 Hard Negative — Independent Implementation

Example:

- same algorithm/specification;
- independently written;
- no shared source lineage.

Label:

> **Hard negative**

This category is central to provenance specificity.

## 2.4 Confounder — Common-Library Sharing

Examples:

- zlib;
- libpng;
- mbedTLS;
- libc;
- other third-party/common libraries.

Interpretation:

- common-library sharing may reflect shared origin at the library level;
- it is weak evidence of direct A ↔ B reuse;
- it should be down-weighted rather than treated as strong direct reuse evidence.

Label:

> **Confounder / evidence attenuation**

## 2.5 Negative — Unrelated Code

Different functionality and different lineage.

Label:

> **Negative**

## 2.6 Important Licensing Clarification

Licensed or license-compliant open-source reuse is still technically:

> **Reuse positive**

DeepBugger detects technical provenance/reuse.

It does **not** decide:

- whether reuse is lawful;
- whether a license was violated;
- whether infringement occurred;
- whether copying was intentional.

---

# 3. Scope Guard

DeepBugger is a system for **technical reuse evidence**, not a legal decision system.

## 3.1 In Scope

- provenance-based binary reuse detection;
- function retrieval;
- transformed-code matching;
- partial reuse;
- region-level evidence;
- program-level evidence aggregation;
- low-base-rate false-positive control;
- abstention / review-required decisions;
- address-level evidence;
- adversarial robustness;
- large-scale retrieval;
- vulnerable-code reuse tracking.

## 3.2 Out of Scope

- legal plagiarism judgment;
- copyright-infringement judgment;
- intent attribution;
- source-level plagiarism detection;
- packing;
- encryption;
- self-modifying code;
- virtualization-based obfuscation.

## 3.3 Working Scope Interpretation

The following are working project decisions rather than verbatim proposal requirements:

- DeepWeak-style security-pattern classification is **not** the main provenance pipeline.
- The main continuation path is from Mini-DeepBugger / DeepClone-style function similarity and the shared 29-D semantic A-CFG.
- AsiaCCS Dataset V2 work is not the starting point for this extension unless explicitly reintroduced by the professor.

---

# 4. Threat Model

## 4.1 Analyst / Defender

The analyst has:

- target binary `A`;
- reference binary collection `B = {B1, ..., BN}`.

The analyst does **not** have for target `A`:

- source code;
- symbols;
- build information.

Reference-side debug information may be used only for **ground-truth construction**, not model input.

## 4.2 Adaptive Reuser / Attacker

The attacker may apply:

- compiler changes;
- optimization changes;
- inlining;
- splitting/outlining;
- wrappers;
- expression rewriting;
- control-flow rewriting;
- O-LLVM-style obfuscation;
- semantics-preserving CFG manipulation;
- unrelated-code dilution.

An adaptive attacker may know:

- DeepBugger features;
- aggregation rules;
- operating thresholds.

## 4.3 Design Priority

> Prefer low false-positive operation and abstention over aggressive recall.

This priority should influence:

- threshold selection;
- low-base-rate evaluation;
- abstention;
- final claims.

---

# 5. Formal System Output

For each target binary `A` and reference program `Bi`, DeepBugger should output:

## 5.1 Three-Level Decision

```text
Reuse Evidence
Review Required
Insufficient Evidence
```

## 5.2 Evidence Locations

Matched:

- function addresses;
- region addresses;
- basic-block addresses / IDs.

Conceptually:

```text
E(A, Bi)
```

## 5.3 Program Score

Directional score:

```text
S(A → Bi)
```

## 5.4 Evidence Components

At minimum:

- matching quality / similarity;
- rarity;
- reference-side coverage;
- call context;
- common-library information;
- abstention reason;
- leave-one-component-out score changes.

## 5.5 Low-Base-Rate Evaluation

Proposal-planned base rates:

```text
π ∈ {1%, 5%}
```

---

# 6. From Mini-DeepBugger to DeepBugger

## 6.1 Mini-DeepBugger Baseline

- 29-D semantic A-CFG node features;
- GCN function encoder;
- function-level binary similarity;
- cross-optimization evaluation;
- APSEC baseline.

## 6.2 DeepBugger Extension

- provenance-aware representation;
- 36-D node representation;
- 128-D function embedding;
- lineage-aware contrastive learning;
- large-scale retrieval;
- conditional region matching;
- program-level evidence aggregation;
- abstention;
- address-level reporting;
- adversarial evaluation;
- scale evaluation.

---

# 7. Research Questions

## RQ1 — Measurement

How and where do current BCSD models plus standard aggregation methods fail for provenance-based reuse detection?

## RQ2 — Representation

Is there a trade-off between semantic robustness and provenance specificity?

## RQ3 — Partial Reuse

Can region-level evidence recover recall when function boundaries change because of inlining, splitting, or wrappers?

## RQ4 — Decision

Can rarity, directional coverage, call context, and abstention reduce false positives at low base rates, and does the effect remain retriever-agnostic?

## RQ5 — Adversarial Robustness

How well can an adaptive attacker evade DeepBugger using obfuscation, CFG manipulation, dilution, and related transformations, and at what cost?

## RQ6 — Practicality

Can DeepBugger scale to large candidate pools and support vulnerable-code reuse tracking?

---

# 8. Expected Contributions

## C1 — ProvBench

A provenance-grounded benchmark containing:

- transformed same-lineage positives;
- partial reuse;
- independent implementations;
- common-library confounders;
- unrelated negatives;
- low-positive-rate streams.

## C2 — Measurement Study

Show why strong function-level similarity performance does not necessarily guarantee reliable program-level reuse decisions.

## C3 — Evidence Aggregator

**Presentation interpretation:** this is treated as the main technical core.

Components:

- region evidence;
- rarity weighting;
- reference-side directional coverage;
- matching quality;
- call-context consistency;
- abstention;
- address-level evidence.

## C4 — Adversarial, Scale, and Security Evaluation

Evaluate:

- adaptive evasion;
- dilution;
- CFG manipulation;
- obfuscation;
- retriever-agnostic behavior;
- large-scale retrieval;
- vulnerable-code reuse cases.

---

# 9. Target System Pipeline

```text
Stripped Native Binary
        ↓
Function / Basic Block / CFG / Call Graph Recovery
        ↓
Ghidra Headless
        ↓
36-D Semantic Representation
(29-D frozen Mini-DeepBugger + 7-D extension)
        ↓
128-D Function Embedding
        ↓
FAISS HNSW Top-K Retrieval
        ↓
Conditional Region Matching
        ↓
Program-Level Evidence Aggregation
        ↓
Rarity + Reference-Side Coverage + Matching Quality + Call Context
        ↓
Abstention
        ↓
Three-Level Decision
        ↓
Reuse Evidence / Review Required / Insufficient Evidence
        ↓
Address-Level Evidence Report
```

---

# 10. Preprocessing and Extraction Integrity

## 10.1 Main Input Formats

Proposal scope:

- stripped ELF x86-64;
- stripped ELF ARM64.

## 10.2 Regression Compatibility

Mach-O ARM64 is retained for Mini-DeepBugger regression compatibility.

## 10.3 Main Analysis Tool

Use:

> **Ghidra headless**

Recover:

- function boundaries;
- basic blocks;
- CFG;
- call graph.

## 10.4 Regression Check

Maintain agreement checks with the existing Mini-DeepBugger `otool` path where appropriate.

## 10.5 Extraction Failures

Critical rule:

> **Extraction failures remain in the denominator of end-to-end evaluation.**

Do not silently discard difficult samples.

Record failure type, such as:

- function-boundary failure;
- indirect-branch recovery failure;
- malformed graph;
- missing build artifact;
- unsupported tool/architecture case.

---

# 11. Representation and Encoder

## 11.1 Frozen Base

Freeze the original Mini-DeepBugger:

> **29-D semantic A-CFG features**

## 11.2 Seven-Dimensional Extension

Proposal wording:

- def-use edge counts;
- external-call categories.

Final input:

> **36-D node representation**

## 11.3 Encoder

Proposal-planned:

```text
36 → 128 → 128 → 128
```

with:

- 3-layer residual GCN;
- attention pooling;
- 128-D L2-normalized embedding.

---

# 12. Lineage-Aware Contrastive Learning

## 12.1 Positive Set

For sample `i`, positive set `P(i)` contains transformed variants from the same canonical source.

## 12.2 Lineage-Negative Set

`L(i)` contains independent implementations of the same specification.

Important leakage rule:

> Lineage hard negatives must be mined only from the **training split**.

Do not mine them from dev/test.

## 12.3 Full Proposal Loss

```text
L =
L_SupCon(z; P)
+
λ_lin · (1 / |L(i)|)
· Σ_{j ∈ L(i)}
  max(0, cos(z_i, z_j) − m)^2
```

Interpretation:

- `L_SupCon` pulls same-lineage transformed variants together.
- The lineage-negative penalty pushes independent implementations apart when cosine similarity exceeds margin `m`.

## 12.4 Proposal-Planned Settings

```text
temperature τ = 0.07
lineage-negative margin m = 0.3
λ_lin controls lineage penalty
λ_lin = 0 → no-lineage-negative ablation
```

Proposal-planned positive augmentation also includes:

- deltaCFG-style semantics-preserving CFG transformations.

These are planned settings, not measured final findings.

---

# 13. Candidate Retrieval

Proposal-planned:

```text
FAISS HNSW
M = 32
efSearch = 128
Top-K = 10
```

Reference-program function embeddings are indexed for retrieval.

---

# 14. Conditional Region Matching

Region matching is used only when function-boundary mismatch is suspected.

## 14.1 Boundary-Mismatch Trigger

Proposal-planned trigger if either:

1. basic-block count ratio `< 0.5` or `> 2`; or
2. one query function corresponds to two or more reference functions in score range `0.55–0.80`.

## 14.2 Region Construction

Proposal-planned:

- dominator-tree-based;
- single-entry CFG regions;
- minimum region size = 3 basic blocks;
- subgraph embedding with the same encoder;
- greedy non-overlapping many-to-many matching.

## 14.3 Scope Rule

Region matching is:

> **conditional**

not mandatory for every candidate.

---

# 15. Program-Level Evidence Aggregation

## 15.1 Rarity

Proposal:

```text
u(f) = log((N + 1) / (df(f) + 1)) + 1
```

Where:

- `N` = number of reference programs;
- `df(f)` = number of reference programs containing function cluster `c(f)`.

## 15.2 Function Clusters

Proposal-planned:

```text
embedding cosine >= 0.85
```

using connected components.

## 15.3 Common-Library Attenuation

Known common-library functions:

```text
evidence weight × 0.2
```

This is attenuation, not hard exclusion.

## 15.4 Instruction Weight

`w(f)` is based on instruction count.

Proposal-planned cap:

```text
w(f) <= 512
```

Do not count the same instruction address twice.

## 15.5 Reference-Side Coverage

```text
C_B^u =
Σ covered reference functions u(g)·w(g)
-----------------------------------------
Σ all reference functions u(g)·w(g)
```

This is the primary coverage term.

## 15.6 Target-Side Coverage

```text
C_A =
Σ covered target functions w(f)
-------------------------------
Σ all target functions w(f)
```

`C_A` is mainly interpretive.

It is vulnerable to dilution because unrelated target code increases the denominator.

## 15.7 Matching Quality

`Q` = rarity/size-weighted average similarity among matched functions/regions.

## 15.8 Call Context

`V` = preserved-call-edge ratio among matched pairs.

## 15.9 Program Score

```text
S(A → B) = C_B^u × [ αQ + (1 − α)V ]
```

Proposal-planned:

```text
α = 0.6
```

Freeze `α` on development data.

---

# 16. Abstention and Evidence Reporting

## 16.1 Proposal-Planned Abstention Conditions

Return **Review Required** if any condition is met:

1. target function extraction coverage `< 0.85`;
2. `>= 60%` of evidence mass is concentrated in one function;
3. top-1 vs top-2 reference score gap `< 0.05`;
4. `>= 50%` of evidence is common-library-tagged.

Thresholds:

> dev only → freeze → never re-tune on final test.

## 16.2 Evidence Report

Include:

- matched function/region addresses;
- block IDs;
- similarity;
- rarity;
- common-library tag;
- score components;
- leave-one-component-out score changes;
- abstention reason.

Evidence is technical observation, not formal proof of:

- semantic equivalence;
- plagiarism;
- infringement;
- intent.

---

# 17. ProvBench

## 17.1 D1 — Controlled Transformations

Proposal target:

- 2,150 C/C++ source families;
- Clang 15;
- GCC 12;
- O0–O3;
- x86-64;
- ARM64;
- four transformation categories:
  - control-flow rewriting;
  - inlining toggle;
  - wrapper insertion;
  - expression rewriting.

Target total:

> **137,600 function variants**

Main use:

- RQ2;
- representation learning;
- transformation robustness.

## 17.2 D2 — External Validation

Proposal target:

- fixed unseen BinKit 2.0 subset;
- **100,000 functions**.

Main use:

- OOD evaluation;
- scale;
- RQ6.

## 17.3 D3 — Program Reuse

Proposal describes:

- 64 host programs;
- donor reuse ratios:
  - 5%;
  - 10%;
  - 20%;
  - 40%;
  - 60%;
- transformed reuse;
- independent implementations;
- common-library-sharing cases;
- unrelated pairs.

Proposal target:

```text
3,000 main pairs
(5 types × 600)
```

### Important Source-Fidelity Warning

The current proposal text says:

> **5 types × 600**

but does **not** explicitly define in the same passage exactly what those five types are.

Therefore:

> **Do not interpret “5 types” as “5 relation types” unless confirmed.**

Before D3 generation:

1. confirm the exact five-type definition with the professor if needed;
2. write the definition into `PROVBENCH_SCHEMA.md`;
3. freeze it;
4. only then generate the full D3 dataset.

Additional proposal target:

> **12,000 low-base-rate stream negatives**

Main use:

- RQ1;
- RQ3;
- RQ4;
- RQ5.

## 17.4 D4 — Security Cases

Proposal target:

- 34 public CVE vulnerable-function reuse cases;
- OpenSSL;
- libxml2;
- BusyBox;
- libpng;
- vulnerable functions plus inlining variants.

Main use:

- vulnerable-code reuse tracking;
- RQ6.

---

# 18. Independent-Implementation Sources

Proposal-suggested sources:

- different accepted C submissions for the same programming-contest task;
- different OSS projects independently implementing:
  - hashes;
  - checksums;
  - encodings;
  - parsers;
- Rosetta Code multiple C implementations of the same task.

Required precautions:

- lineage inspection;
- license inspection;
- commit-history inspection;
- near-duplicate filtering;
- hidden-copy filtering.

Goal:

> confirm genuine independence rather than copied lineage.

---

# 19. Label-Quality Protocol

Proposal-planned difficult-case review:

- 240 difficult cases;
- two independent annotators.

Proposal target values:

```text
Cohen's κ = 0.81
uncertain/abstained labels = 9.6%
```

Critical rule:

> These are **target values**, not measured results.

Report actual agreement after annotation.

---

# 20. Dataset Split and Leakage Control

Proposal target:

```text
Train / Dev / Test = 60 / 20 / 20
```

Split by:

- project lineage;
- canonical source family.

Requirements:

- forks remain in the same group;
- vendored libraries remain in the same group;
- no lineage overlap across train/dev/test;
- remove function names;
- remove paths;
- remove compiler strings;
- use stripped binaries.

Proposal-required manifest information includes:

- compiler;
- flags;
- transformation ID;
- SHA-256.

Freeze manifests before tuning.

---

# 21. Recommended Working Metadata Schema

This is a **working engineering design**, not verbatim proposal field naming.

```text
sample_id
canonical_source_id
lineage_id
project_id
compiler
compiler_version
optimization
architecture
transformation_id
relation_type
reuse_ratio
donor_id
host_id
library_tag
sha256
split
```

Recommended operational additions:

```text
binary_path
source_family_id
build_id
toolchain_id
extraction_status
extraction_failure_type
ground_truth_source
annotation_status
annotator_ids
```

---

# 22. Function-Level Baselines

Proposal-planned:

- non-learned 29-D cosine;
- pooled MLP;
- Mini-DeepBugger GCN;
- Gemini;
- jTrans;
- HermesSim.

External-model reporting must distinguish:

- author checkpoint;
- retrained / fine-tuned implementation.

Proposal note:

- jTrans planned comparison is x86-64-limited.

---

# 23. Program-Level Baselines

Proposal-planned:

- BinDiff matching ratio;
- KEENHash;
- LibAM-style area aggregation;
- BinaryAI-style locality aggregation.

Critical rule:

> Reimplementations must be explicitly labeled as reimplementations.

---

# 24. RQ-Specific Metric Map

Do not assume one operating point applies to every RQ.

## RQ1 — Provenance Measurement

Core proposal setting includes:

```text
FPR at TPR = 0.80
precision at π = 1%
precision at π = 5%
PR-AUC
```

Program-level low-base-rate precision may be computed using:

```text
precision =
π·TPR
-------------------------------
π·TPR + (1−π)·FPR
```

Also evaluate the relation between:

- function-level performance;
- program-level provenance performance.

Proposal uses Kendall rank correlation for this measurement study.

## RQ2 — Representation

Proposal table uses:

- Recall@1;
- Recall@10;
- MRR;
- worst-group Recall@10;
- independent-implementation `FPR@TPR0.9`.

Also analyze robustness-specificity relation using rank correlation.

## RQ3 — Partial Reuse

Use:

- TPR@FPR 1%;
- region precision;
- region recall;
- reuse-ratio-specific results.

## RQ4 — Decision / Aggregation

Use:

- PR-AUC;
- common-library FPR;
- independent-implementation FPR;
- overall FPR;
- precision at π=1%;
- precision at π=5%;
- risk–coverage;
- AURC;
- retriever-agnostic comparison.

## RQ5 — Adversarial Robustness

Use:

- TPR@FPR 1%;
- evasion success rate;
- abstention transition rate where applicable;
- attack cost / budget.

## RQ6 — Scale and Security Application

Use:

- exact Recall@10;
- HNSW Recall@10;
- ANN recall loss;
- function-query p95 latency;
- program-query p95 latency;
- index memory;
- CVE Top-1 / Top-K retrieval;
- address/region evidence success where applicable.

---

# 25. Statistical Protocol

Proposal-planned:

- 5 seeds for core models;
- cluster bootstrap;
- 2,000 bootstrap resamples;
- 95% confidence intervals;
- cluster unit: project / original-source family;
- Clopper–Pearson interval for sparse FPR;
- Holm correction for non-prespecified multiple comparisons.

---

# 26. Proposal-Planned Experimental Environment

Planned environment:

- Ubuntu 22.04;
- NVIDIA A100 80GB × 1–2;
- RAM 256GB;
- Ghidra 11.x;
- PyTorch 2.x;
- PyTorch Geometric;
- FAISS.

This is a proposal-planned environment, not an immutable hardware requirement.

## Reproducibility Rule

Every experiment should record:

- YAML config;
- Git commit/tag;
- dataset SHA-256;
- manifest version;
- model checkpoint ID;
- random seed;
- environment/version metadata.

---

# 27. Adversarial Evaluation Matrix

## 27.1 Planned Transformation Conditions

- compiler / optimization variation;
- inlining;
- splitting;
- wrappers;
- O-LLVM instruction substitution;
- bogus control flow;
- control-flow flattening;
- combined O-LLVM transformations;
- deltaCFG-style semantics-preserving CFG manipulation;
- unrelated-code dilution.

## 27.2 Adaptive Attacker

Adaptive attacker knows:

- features;
- aggregation rules;
- thresholds.

Proposal-planned code-size budgets:

```text
+20%
+50%
+100%
```

Proposal adaptive-evasion table also assumes:

```text
execution overhead <= 50%
```

Important:

> All attack-result values in proposal experiment tables are targets until measured.

---

# 28. Scale Evaluation

Proposal target pool sizes:

```text
10^3
10^4
10^5
10^6 functions
```

Measure:

- exact Recall@10;
- HNSW Recall@10;
- ANN recall loss;
- function-query p95 latency;
- program-query p95 latency;
- index memory;
- degradation due to harder negatives as pool size increases.

---

# 29. Target Findings / Planned Hypotheses

These are **proposal targets**, not completed findings.

## F1 — RQ1 Target

Function-retrieval quality may fail to predict provenance-level reuse-decision quality.

Proposal expects common-library and independent-implementation cases to dominate false positives.

## F2 — RQ2 Target

Semantic robustness and provenance specificity may trade off.

Lineage-negative learning is expected to reduce independent-implementation false positives while preserving most retrieval performance.

## F3 — RQ3 Target

For partial reuse:

> important evidence may live at the region level rather than only the whole-function level.

Region matching is expected to improve performance under inlining/splitting.

## F4 — RQ4 Target

Low-base-rate precision is expected to depend strongly on:

- rarity;
- call context;
- abstention.

## F5 — RQ5 Target

Target-side coverage is expected to fail under dilution, while reference-side coverage should remain more robust.

## F6 — RQ6 Target

Evidence aggregation is expected to remain practical because expensive analysis is applied only to top candidates rather than the entire reference pool.

## Target-Finding Rule

If real experiments do not support one of F1–F6:

> rewrite or remove the finding.

Do not force the experiment to preserve the target narrative.

---

# 30. Target → Measured-Result Replacement Matrix

This section operationalizes the proposal's Appendix-B logic.

## Abstract / Introduction / Contribution Summary

Replace:

- all proposal target numbers;
- all target comparisons;
- all target-effect sizes.

If measured results differ:

- update contribution wording;
- update Key Findings;
- update Abstract and Introduction consistently.

## RQ1

Replace:

- function metrics;
- FPR;
- low-base-rate precision;
- Kendall correlation.

If function-level performance strongly predicts program-level provenance performance:

> revise F1.

Do not continue claiming that function retrieval quality is insufficient if the measured evidence says otherwise.

## RQ2

Replace:

- Recall@K;
- MRR;
- independent-implementation FPR;
- robustness-specificity correlation.

If the expected trade-off is not observed:

> report compatibility rather than claiming a trade-off.

## RQ3

Replace:

- partial-reuse TPR;
- region precision;
- region recall.

If region matching gives little/no improvement:

> reduce the region-matching contribution.

C3 may need to emphasize rarity/aggregation/abstention instead.

## RQ4

Replace:

- component ablations;
- retriever-agnostic results;
- program-level comparisons;
- AURC / risk–coverage.

If the aggregator is strongly retriever-dependent:

> remove the model-agnostic / retriever-agnostic claim.

## RQ5

Replace:

- robustness TPR;
- evasion success;
- abstention transitions.

If adaptive evasion remains strong:

> report it prominently as a limitation.

## RQ6

Replace:

- latency;
- ANN loss;
- scale;
- D4 security-case results.

If 10^6-scale evaluation is not feasible:

> report the largest scale actually completed, such as 10^5, without implying 10^6 validation.

---

# 31. Experimental Targets vs Measured Results

## Golden Scientific Rule

> **TARGET VALUES ≠ FINAL EXPERIMENTAL RESULTS**

Most numerical values in proposal:

- Abstract;
- Introduction;
- experiment tables;
- target findings;

are planned targets.

They must be replaced after real experiments.

## Existing Historical Measurement

Mini-DeepBugger measured baseline values may be used as historical results.

Example:

```text
DeepClone GCN F1 ≈ 0.7759
```

Do not convert proposal targets into “results” simply because they already appear in a table.

---

# 32. Failure-Analysis Plan

The proposal already identifies failure categories worth tracking.

Treat all percentages shown in proposal failure tables as **targets** until measured.

## Planned Failure Categories

### A. Ultra-Low Reuse Miss

Example:

```text
reuse <= 5%
```

Possible cause:

- evidence mass too small;
- abstention triggered.

### B. Control-Flow-Flattening Miss

Possible cause:

- CFG structure destroyed.

Possible future direction:

- data-flow-oriented evidence;
- micro-trace / execution-semantic features.

### C. Independent-Implementation False Positive

Possible cause:

- idiomatic implementations of standard algorithms remain structurally/semantically similar.

Possible future direction:

- category-aware rarity;
- stronger lineage evidence.

### D. Large-Caller Inlining Miss

Possible cause:

- reused code becomes embedded inside a very large caller.

Possible future direction:

- caller-internal slicing.

### E. Preprocessing Failure

Possible cause:

- function-boundary recovery;
- indirect branches;
- extraction/tool failure.

Critical rule:

> preprocessing failures stay in the denominator.

## Required Final Failure Analysis

For final results, report:

- category count;
- category share;
- representative examples;
- primary cause;
- possible mitigation.

Use measured values only.

---

# 33. Limitations

## 33.1 Architecture / Language

Main scope:

- x86-64;
- ARM64.

Not covered:

- Rust;
- Go.

## 33.2 Synthetic Program Reuse

D3 reuse insertion is controlled/synthetic.

Real reuse may also include:

- refactoring;
- new features;
- broader restructuring.

External validity should be supported with:

- D2;
- D4;
- independent OSS implementations.

## 33.3 Independent-Implementation Labels

Risk:

> hidden copying may be labeled incorrectly as independent implementation.

Mitigation:

- lineage checks;
- commit-history checks;
- duplicate filtering;
- two-person review.

Residual error may conservatively inflate FPR.

## 33.4 Abstention Cost

Higher precision may require:

- lower automatic coverage;
- more human review.

The operating point should be reported rather than hidden.

---

# 34. Threats to Validity

## Internal Validity

- hyperparameters → dev only;
- thresholds → dev only;
- freeze protocol before test;
- final held-out test once.

## Construct Validity

External baselines:

- distinguish author checkpoint vs retrained result.

Reimplementations:

- label LibAM-style / BinaryAI-style systems accurately.

## External Validity

Proposal plans cross-checking using:

- EXHIB independent-implementation data;
- BinKit external data.

---

# 35. Ethics

Proposal-derived constraints:

- do not publish plagiarism accusations against identifiable people/companies;
- real-world cases should use licensed reuse or already-patched public CVEs;
- do not redistribute original contest submissions;
- adaptive evasion tooling may aid concealment, so release only what reproducibility requires;
- DeepBugger output is technical evidence, not legal judgment.

---

# 36. Open Science

Proposal intends to release:

- code;
- model weights;
- ProvBench manifests;
- build scripts;
- derived features;
- experiment configurations;
- table-regeneration scripts;
- figure-regeneration scripts.

Submission plan:

- anonymous artifact;
- Open Science appendix;
- permanent release after acceptance where appropriate.

---

# 37. Working Implementation Plan

> The following phases are working research-management decisions derived from the professor's timeline.

---

## Phase 0 — Baseline Reproduction and Freeze

Goal:

> create an immutable Mini-DeepBugger baseline before extension.

Freeze:

- 29-D extractor;
- DeepClone model;
- manifests;
- configs;
- seeds;
- thresholds;
- historical results;
- regression tests.

Deliverables:

```text
BASELINE_FREEZE.md
frozen configs
frozen manifests
frozen results
regression report
Git tag
```

---

## Phase 1 — Provenance Schema Freeze

Freeze:

- relation definitions;
- lineage definition;
- canonical-source definition;
- library policy;
- annotation rules;
- D3 “5 types” definition;
- train/dev/test grouping;
- extraction-status schema.

Deliverables:

```text
PROVBENCH_SCHEMA.md
annotation policy
schema validator
leakage checker
manifest template
```

---

## Phase 2 — D1/D3 Pilot + RQ1

Tasks:

- D1 pilot;
- D3 pilot;
- independent-implementation collection;
- common-library confounders;
- low-base-rate streams;
- current retrievers;
- baseline aggregation;
- FPR decomposition.

Freeze RQ1 protocol before testing new provenance encoder.

---

## Phase 3 — Provenance Encoder / RQ2

Tasks:

- +7-D features;
- 36-D nodes;
- residual GCN;
- attention pooling;
- 128-D embedding;
- lineage-aware loss;
- ablations.

Evaluate both:

- robustness;
- provenance specificity.

---

## Phase 4 — Region Matching + Aggregation / RQ3–RQ4

Implement:

- mismatch trigger;
- region generation;
- region matching;
- rarity;
- directional coverage;
- quality;
- call context;
- abstention;
- evidence report.

Maintain:

> retriever-agnostic aggregator interface.

---

## Phase 5 — External Retrievers + RQ5/RQ6

Run:

- Gemini;
- jTrans;
- HermesSim;
- DeepBugger encoder;
- adversarial transformations;
- adaptive evasion;
- dilution;
- scale;
- latency;
- memory.

---

## Phase 6 — D4 + Final Statistics + Paper

Run:

- CVE reuse cases;
- final failure analysis;
- confidence intervals;
- final figures/tables;
- limitations;
- ethics;
- Open Science;
- anonymous artifact;
- manuscript;
- two internal mock reviews.

---

# 38. Proposal Timeline

| Period | Main Goal | Freeze / Deliverable |
|---|---|---|
| Oct 13–25 | Mini-DeepBugger reproduction and freeze | baseline freeze |
| Oct 26–Nov 22 | ProvBench D1/D3 + independent implementations + RQ1 | dataset v1 freeze |
| Nov 23–Dec 13 | lineage-aware learning + region matching + RQ2–RQ4 | dev result freeze |
| Dec 14–Jan 3 | external retrievers + adversarial + scale + final test | final result freeze |
| Jan 4–17 | D4 + statistics + figures + manuscript + mock reviews | complete manuscript |
| Jan 19 / 26 / 29 | registration / paper / artifact | submission freeze |

These dates are proposal-derived planning dates.

Before acting on conference deadlines:

> verify the current official CFP.

---

# 39. Submission Strategy — Proposal-Derived

The proposal recommends the following path:

1. **USENIX Security 2027 Cycle 2**
2. if early rejection: **ACM CCS 2027 Cycle B**
3. if final rejection: **NDSS 2028 Summer Cycle** or **USENIX Security 2028 Cycle 1**

Important:

- no double submission;
- do not submit the same manuscript concurrently;
- verify the current CFP and resubmission rules before each submission.

The master research protocol should remain conference-independent where possible.

---

# 40. Bibliography and Citation Safety

The proposal itself flags bibliographic items requiring verification.

Before final submission:

1. verify every paper title;
2. verify every author list;
3. verify venue;
4. verify publication year;
5. verify DOI when used;
6. verify arXiv-only vs formally published status;
7. verify external repositories/software URLs;
8. re-number references after removing internal placeholder gaps.

Special proposal flags include bibliographic verification for items such as:

- BINREX;
- LibAM publication venue.

Critical rule:

> **Never invent a citation or leave an unverified bibliographic claim in the final paper.**

Use authoritative sources such as:

- publisher pages;
- DBLP;
- official conference proceedings;
- official project repositories.

---

# 41. Freeze Rules

## Rule 1 — Baseline Freeze

Preserve Mini-DeepBugger exactly.

## Rule 2 — Dataset Freeze

Version manifests and generation scripts.

## Rule 3 — Dev Before Test

Tune only on train/dev.

## Rule 4 — Final Test Once

Do not retune after final held-out evaluation.

## Rule 5 — Target ≠ Result

Never report targets as findings.

## Rule 6 — Negative Results Are Valid

Change the claim, not the held-out evaluation.

## Rule 7 — Extraction Failures Count

Keep preprocessing failures in end-to-end denominators.

## Rule 8 — Reimplementations Are Labeled

Never present a reimplementation as the original system.

## Rule 9 — Unresolved Definitions Are Frozen Before Generation

Especially:

- D3 “5 types”;
- annotation policy;
- common-library policy;
- lineage grouping.

---

# 42. Suggested Repository Structure

```text
Mini-DeepBugger/
├── frozen_baseline/
│   ├── configs/
│   ├── manifests/
│   ├── models/
│   ├── results/
│   └── BASELINE_FREEZE.md
│
├── deepbugger/
│   ├── preprocessing/
│   ├── features36/
│   ├── encoder/
│   ├── retrieval/
│   ├── region_matching/
│   ├── aggregation/
│   ├── abstention/
│   └── reporting/
│
├── provbench/
│   ├── schemas/
│   ├── annotations/
│   ├── d1/
│   ├── d2/
│   ├── d3/
│   ├── d4/
│   ├── manifests/
│   └── validation/
│
├── experiments/
│   ├── rq1/
│   ├── rq2/
│   ├── rq3/
│   ├── rq4/
│   ├── rq5/
│   └── rq6/
│
├── external_baselines/
├── results/
├── scripts/
└── docs/
```

Working recommendation only.

---

# 43. Immediate Next Actions

1. Audit Mini-DeepBugger repository.
2. Freeze baseline files.
3. Reproduce historical results.
4. Create `BASELINE_FREEZE.md`.
5. Tag baseline.
6. Create `PROVBENCH_SCHEMA.md`.
7. Resolve exact D3 “5 types”.
8. Freeze lineage rules.
9. Freeze common-library treatment.
10. Add extraction-status fields.
11. Define annotation protocol.
12. Build D1 pilot.
13. Build D3 pilot.
14. Construct low-base-rate streams.
15. Run existing retrievers.
16. Freeze RQ1 protocol.
17. Measure RQ1.
18. Only then evaluate the new provenance encoder against the frozen protocol.

---

# 44. Decision Gates

## Gate A — Baseline Freeze

Proceed when:

- baseline reproduces;
- configs/manifests are frozen;
- regression checks pass.

## Gate B — Schema Freeze

Proceed when:

- relation definitions are fixed;
- D3 five-type definition is fixed;
- lineage rules are fixed;
- leakage checks exist;
- annotation rules exist.

## Gate C — RQ1 Protocol Freeze

Proceed when:

- RQ1 data is frozen;
- base-rate construction is frozen;
- baseline aggregation is frozen;
- failure categories are frozen.

Do not change held-out evaluation to force F1.

## Gate D — Development Freeze

Freeze:

- encoder;
- loss settings;
- retrieval;
- region triggers;
- rarity;
- common-library attenuation;
- score formula;
- abstention thresholds.

## Gate E — Final Test

Run once.

Record:

- commit/tag;
- config;
- data hash;
- model checkpoint;
- result artifacts;
- environment.

Then freeze results.

---

# 45. Master Research Checklist

Before any implementation or claim, ask:

1. Does this distinguish similarity from provenance?
2. Which RQ does it answer?
3. Which contribution does it support?
4. Is the data lineage-separated?
5. Is there leakage risk?
6. Is the number a target or measurement?
7. Was tuning restricted to train/dev?
8. Are extraction failures counted?
9. Does it reduce/explain a known false-positive mode?
10. Does aggregation remain retriever-agnostic where claimed?
11. Is common-library evidence attenuated appropriately?
12. Can the evidence be reviewed by a human?
13. Is wording technical rather than legal?
14. Are reimplemented baselines clearly labeled?
15. Is Mini-DeepBugger still frozen?
16. Can the experiment be reproduced from config + commit + data hash?
17. Has the relevant target finding been replaced with measured evidence?
18. Have references been verified?

---

# 46. Current Working Priority

> **Reproduce → Freeze → Define Provenance → Freeze Schema → Build D1/D3 Pilot → Freeze RQ1 Protocol → Measure RQ1 → Extend Encoder**

Do not treat the 36-D / 128-D model as the first milestone.

First milestone:

> **trustworthy frozen baseline + provenance-grounded measurement protocol**

---

# 47. Final Research Story

DeepBugger evolves from:

> **“How similar are these binary functions?”**

to:

> **“Is there reliable technical evidence that code in target A actually came from reference program B?”**

Research story:

1. **C1 / ProvBench**  
   Define provenance-aware ground truth.

2. **C2 / Measurement**  
   Show where similarity does or does not fail for reuse decisions.

3. **C3 / Evidence Aggregation**  
   Combine partial-region evidence, rarity, reference-side coverage, matching quality, call context, and abstention.

4. **C4 / Robustness and Practicality**  
   Evaluate adversarial behavior, scale, retriever independence, and vulnerable-code reuse.

The final paper must follow measured evidence even if the evidence contradicts the proposal's targets.

---

# 48. Golden Rules

1. **Similar ≠ Reused.**
2. **Common library ≠ strong direct A ↔ B reuse evidence.**
3. **Licensed reuse is still technical reuse.**
4. **Target values ≠ measured results.**
5. **Do not guess undefined dataset structure.**
6. **Training lineage negatives come only from training data.**
7. **Dev before test.**
8. **Final held-out test once.**
9. **Negative results are valid.**
10. **Extraction failures stay in the denominator.**
11. **Technical evidence ≠ legal judgment.**
12. **Reimplementations must be labeled.**
13. **Freeze before extending.**
14. **Verify references before submission.**
15. **Reproducibility is part of the contribution.**
