# Mini-DeepBugger: Protocol V2 — AsiaCCS 2027 Experimental Protocol
**Document Identifier:** `EXPERIMENT_PROTOCOL_V2`  
**Revision:** 2.2.1  
**Status:** APPROVED / FROZEN — DATASET V2 CONSTRUCTION AUTHORIZED  
**Target Milestone:** ACM AsiaCCS 2027 Submission (December 11, 2026)  
**Git Branch:** `exp/asiaccs-v2`  
**Frozen Baseline Anchor:** Commit `04baeddf8116e632be6c8aeaa183872c9a8b8a6f` (Tag: `next-baseline-freeze`)  
**Experimental Cutoff:** November 30, 2026  

---

## Protocol Freeze Record

- **Freeze Date:** 2026-10-08  
- **Frozen Baseline Anchor:** Commit `04baeddf8116e632be6c8aeaa183872c9a8b8a6f` (Tag: `next-baseline-freeze`)  
- **Protocol Git Branch:** `exp/asiaccs-v2`  
- **Frozen Decisions (Approved by Supervisor 2026-10-08):**  
  - **Decision 1 (Eligible Function Non-Triviality Rule):** Evaluated strictly once on the canonical reference build (`/usr/bin/clang`, `-O0`, boundary-preserving `__attribute__((noinline))`, ARM64). Rule: $|V| \ge 1$ and instruction count $\ge 3$ excluding prologue/epilogue (cyclomatic complexity is a derived diagnostic metadata field only, not an eligibility gate). Canonical ground-truth denominator $M = |\mathcal{F}_{\text{canonical}}(Q)|$ is permanently frozen across all subsequent builds and variants.  
  - **Decision 7 (Canonical Program Allocation):** Total ~21 canonical programs partitioned as 7 `train` / 7 `dev` / 7 `sealed_test` (yielding $C(7,2) = 21$ canonical unrelated pairs per partition). Dataset V2 `train` is used strictly for uniqueness/reference corpus statistics ($\text{freq}_{\text{ref}}$) and never retrains the baseline `GraphEncoder` (which is reconstructed solely on Phase-1 data).  
  - **Decision 8 (Partial-Reuse Target Construction Procedure):** Deterministic SHA-256 PRNG selection ($\text{SHA256}(\text{canonical\_id} \parallel \text{nominal\_reuse\_level} \parallel \text{base\_dataset\_seed})$); transitive dependency closure within $Q$ accounted in $k$; intra-split donor pairing (donor $D$ strictly from same partition); directional ground truth $R^*(Q, T) = k / M$; realized reuse ratio recorded in manifests; frozen base dataset seed: `base_dataset_seed = 20261008`.  
- **Open Decisions (Gated for Development-Set Selection):**  
  - **Decision 2:** Complexity weighting formulation $C(f)$  
  - **Decision 3:** Uniqueness weighting clustering threshold $\tau_{\text{unique}}$  
  - **Decision 4:** Background calibration distribution model  
  - **Decision 5:** Positive / negative classification policy (Policy A vs. Policy B)  
  - **Decision 6:** Function-level match acceptance threshold $\tau_{\text{function}}$  
  - **Decision 9:** Method-specific program-level decision threshold policy ($\theta_{\text{B1}}, \theta_{\text{B2}}, \theta_{\text{B3}}, \alpha_{\text{bg}}$)  
- **Frozen Base Dataset Seed:** `base_dataset_seed = 20261008`  
- **Pre-Freeze Dataset Assurance:** *No Dataset V2 artifact existed before this protocol freeze.*  

### Freeze Erratum 1 — 2026-10-08

- **Dataset V2 Train/Reference Scope:** Dataset V2 train/reference programs do **not** retrain or reconstruct the baseline `GraphEncoder`. The baseline `GraphEncoder` is reconstructed **ONLY** using the frozen Phase-1 training procedure and Phase-1 training data defined by the verified clean baseline. Dataset V2 train/reference programs **MUST NOT** replace Phase-1 baseline training data. Dataset V2 train/reference is used strictly for uniqueness/reference corpus statistics, train-only frequency estimation ($\text{freq}_{\text{ref}}(f)$), and approved Dataset V2 development/reference operations. Retraining the baseline `GraphEncoder` on Dataset V2 is **NOT** authorized by this protocol.  
- **Decision 1 Non-Triviality Exactness:** Decision 1 wording was aligned exactly with the approved Option 2 non-triviality rule: $|V| \ge 1$ and instruction count $\ge 3$ excluding function prologue/epilogue. Cyclomatic complexity is tracked as a derived diagnostic metadata field, and is **NOT** an independent eligibility gate.  

---

## Revision 2.2.1 Freeze Patch Summary

Revision 2.2.1 applies targeted precision patches addressing supervisor audit review items prior to protocol freeze:

1. **Canonical Eligibility Reference-Build Resolution:**  
   Resolved the conceptual inconsistency between source-level identity and binary-level complexity metrics by establishing a two-stage definition: (A) Source-level identification establishes user-defined candidate functions; (B) Canonical reference-build evaluation (`/usr/bin/clang`, `-O0`, boundary-preserving `__attribute__((noinline))`, ARM64) applies the finalized non-triviality rule once to permanently freeze $\mathcal{F}_{\text{canonical}}(Q)$ and denominator $M = |\mathcal{F}_{\text{canonical}}(Q)|$. Denominator $M$ is immutable across all compilers, optimization levels (-O3), transformations, and inlining conditions. All subsequent compiled variants affect only $\mathcal{F}_{\text{extracted}}(Q_{\text{variant}})$. Updated Decision 1 accordingly.
2. **Method-Specific Program-Level Thresholds ($\theta_{\text{B1}}, \theta_{\text{B2}}, \theta_{\text{B3}}, \alpha_{\text{bg}}$):**  
   Replaced the unified symbol $\theta_{\text{program}}$ with method-specific thresholds $\theta_{\text{B1}}$, $\theta_{\text{B2}}$, $\theta_{\text{B3}}$ (for B1, B2, B3) and tail significance $\alpha_{\text{bg}}$ (for Proposed). Clarified that fair comparison requires the **same threshold-selection policy** on `dev` (e.g. identical low-FPR operating point where sample size supports it), rather than identical numerical values across different score scales. Retained threshold-independent PR-AUC reporting. Updated Decision 9 accordingly.
3. **Pinned Official Python Runtime:**  
   Removed "Python 3.12/3.14" references and officially pinned the experimental environment runtime to **Python 3.12.12**. Clarified that Python 3.14 is excluded from the official frozen environment unless separately validated.
4. **Cautious Reproducibility & Backend Reporting:**  
   Replaced overstrong claims with cautious scientific language: *"Fixed seeds support reproducible model reconstruction under the frozen software/hardware backend. Official runs must verify repeated-run consistency. Bitwise-identical behavior across different backends or accelerators is not assumed."* Mandated environment logging for Python, PyTorch, PyG, device/backend, architecture, and deterministic backend flags.
5. **Dataset-Construction Base Seed Finalization Governance:**  
   Explicitly specified that `base_dataset_seed` must be formally finalized in conjunction with Decision 8 before any Dataset V2 source subset is generated, and recorded in manifests (not chosen automatically).

---

## Revision 2.2 Change Summary

In response to the pre-freeze technical audit and human supervisory review, Revision 2.2 incorporated the following scientific and technical precision enhancements:

1. **Model Provenance & Reconstruction Language Correction:**  
   Clarified that the clean baseline on `main` does not track a serialized `.pt` GraphEncoder checkpoint. The baseline relies on a **frozen architecture** (`src/graph_encoder.py`) and a **frozen baseline training procedure** (`experiments/run_final_experiment.py`) executed on the clean Phase-1 training split (`data/variant_pair_manifest.csv`) with CosineEmbeddingLoss (`margin=0.5`), Adam (`lr=0.01`), 100 epochs, and clean baseline seeds `[42, 123, 2026, 7, 99]`. Trained GraphEncoder instances are reconstructed deterministically in memory. Explicitly stated: *Historical checkpoints from downstream experimental branches are excluded from AsiaCCS V2.*
2. **Dual Random-Seed Governance:**  
   Formally decoupled the baseline encoder reconstruction seeds (`[42, 123, 2026, 7, 99]`) from dataset-construction PRNG seeding. Replaced Python built-in `hash()` (vulnerable to `PYTHONHASHSEED` randomization) with a deterministic cryptographic derivation: $\text{SHA256}(\text{canonical\_id} \mid \text{nominal\_reuse\_level} \mid \text{base\_dataset\_seed})$ mapped to an integer PRNG seed.
3. **Canonical Eligibility vs. Variant Observability Decoupling:**  
   Decoupled the source-level **Canonical Eligible Set** $\mathcal{F}_{\text{canonical}}(Q)$ (which permanently fixes ground-truth denominator $M = |\mathcal{F}_{\text{canonical}}(Q)|$) from the **Variant-Extracted Set** $\mathcal{F}_{\text{extracted}}(Q_{\text{variant}})$ observable in compiled binaries. Mandated tracking `canonical_eligible_count`, `extracted_eligible_count`, `extraction_rate`, and `missing_function_ids` in manifests so compiler optimizations or inlining never alter ground-truth denominators.
4. **Preserved Inlining Robustness Experiment:**  
   Maintained the approved inlining experiment without globally imposing `-fno-inline`. Partitioned evaluation into: (A) Primary boundary-preserving conditions (GCC vs Clang, -O0 vs -O3) with preserved function boundaries for controlled program matching; and (B) Inlining robustness stress conditions (`inline` vs `noinline`) reported separately. Optimization levels strictly remain `-O0` and `-O3`.
5. **Synthesized Target Operational Validation:**  
   Corrected validation requirements for composite partial-reuse targets $T = \text{reused}(Q) \cup \text{donor}(D)$. Replaced impossible whole-program I/O equivalence requirements with regression-based operational validation: unit-level correctness of reused functions, unit-level correctness of donor functions, valid dependency closure, clean compilation/linking, and crash-free test harness execution. Whole-program equivalence is restricted strictly to 100% full reuse.
6. **Strict Cross-Split Donor Isolation:**  
   Enforced that donor programs $D$ must strictly belong to the same dataset split as $Q$ (`train` donors for `train` queries, `dev` donors for `dev`, `sealed_test` donors for `sealed_test`), prohibiting any cross-split contamination.
7. **Compiler Toolchain & Path Pinning:**  
   Documented that `/usr/bin/gcc` on macOS is an Apple Clang wrapper. Formally pinned genuine GNU GCC at `/opt/homebrew/bin/gcc-15` (Homebrew GCC 15.1.0) and `/usr/bin/clang` (Apple Clang 21.0.0), prohibiting bare `gcc` invocation. Mandated logging compiler paths, version banners, target triples, and `otool` provenance.
8. **Program-Level Decision Threshold Block (`[DECISION 9]`):**  
   Added a dedicated decision block distinguishing function-level threshold $\tau_{\text{function}}$ from program-level evidence decision threshold $\theta_{\text{program}}$ (for B1/B2/B3) and tail significance $\alpha_{\text{bg}}$ (for Proposed). Mandated tuning and freezing thresholds strictly on `dev` before sealed testing.
9. **Canonical-Pair Statistical Unit of Analysis:**  
   Defined the canonical program pair $(P_i, P_j)$ as the primary statistical unit of analysis to prevent pseudo-replication from multiple compiler, optimization, and transformation variants. Grounded low-FPR resolution in canonical degrees of freedom ($C(7,2) = 21$ pairs per partition).
10. **Multi-Tier Reproducibility Standards:**  
    Structured reproducibility criteria into Model Reproducibility, Dataset Reproducibility, and Toolchain Reproducibility, adopting cautious language regarding backend-dependent determinism.

---

## Revision 2.1 Change Summary

Revision 2.1 performs a rigorous pre-freeze cleanup addressing supervisory review items while strictly preserving the approved PowerPoint research scope:

1. **LaTeX & Escape Character Sanitization:**  
   Cleaned all malformed escape sequences and non-printable control characters (e.g., unintended ASCII bell `\x07` in `\alpha` and tab characters in `\text`), ensuring valid mathematical typography throughout the protocol.
2. **Exact vs. Realized Reuse-Ratio Accounting:**  
   Established the $M \pmod 4 = 0$ preference rule for canonical programs so that 25%, 50%, and 75% quartiles can be realized exactly. For programs where integer division requires rounding ($k = \text{round}(\alpha \cdot M)$), mandated recording the true realized ratio $R^*(Q, T) = k / M$ in manifests alongside `nominal_reuse_level`, `reused_function_count`, `eligible_query_function_count`, and `realized_reuse_ratio`. Prohibited falsely labeling realized $k/M$ values as exact nominal percentages. Realized ratios are the definitive ground truth for MAE/RMSE/Pearson/Spearman evaluation.
3. **Strict Dependency-Closure Accounting:**  
   Formalized dependency-closure rules: any internal eligible callee function required by a selected reused function must be explicitly counted toward $k$ and $R^*(Q, T)$. Prohibited hidden copied eligible functions. Candidate subsets are valid only when the closed set satisfies intended ground-truth rules.
4. **Bipartite Matching vs. Evidence Aggregation Decoupling:**  
   Clarified that B1, B2, B3, and Proposed execute one-to-one maximum-weight bipartite assignment over the **identical raw function cosine-similarity matrix**. Complexity weights $C(f)$ and uniqueness weights $U(f)$ do not alter function embeddings or the bipartite matching objective; they are applied strictly post-matching to compute program-level evidence $E_{\text{weighted}}$.
5. **Updated Working Split Recommendation (7 / 7 / 7 Target):**  
   Updated the working split recommendation to approximately 21 canonical programs partitioned as 7 train / 7 dev / 7 sealed test. Documented that 7 dev and 7 test programs provide $C(7,2) = 21$ canonical unrelated pairs each, delivering ~4.76% raw false-positive resolution at canonical pair level (superior to 6-program splits).
6. **Cautious Scientific Terminology & Cryptographic Terminology:**  
   Adopted cautious phrasing ("substantial high-reuse cases") rather than claims of actionable plagiarism. Standardized "Cryptographic Checksums / Hashes" (avoiding calling ordinary SHA-256 files digital signatures) and clarified that Git tags are "treated as immutable by project policy".
7. **Recommendation Label Standardization:**  
   Standardized open decision recommendation labels to "Current Working Recommendation for Supervisor Review" across all open decisions to reflect that final decisions remain pending human authorization.

---

## Revision 2 Change Summary

In response to initial human supervisory review, Revision 2 incorporated the following mandatory methodological corrections:

1. **Separation of Count-Based Reuse Ratio from Weighted Evidence:**  
   Resolved the mathematical inconsistency between count-based ground truth ($R^*(Q, T)$) and weighted prediction. Defined two distinct outputs:
   - **$R\_hat\_count(Q, T)$ (Count-Based Reuse Ratio Estimate):** Unweighted proportion of accepted matched eligible query functions over total eligible query functions. Primary dependent variable for **RQ2** (evaluated via MAE, RMSE, Pearson correlation, and Spearman correlation). Answers: *"How much eligible-function reuse occurred?"*
   - **$E\_weighted(Q, T)$ (Weighted Program-Level Reuse Evidence):** Complexity and uniqueness weighted matching score. Primary metric for **RQ3**, false-positive analysis, and background calibration. Answers: *"How strong and unusual is the observed reuse evidence?"* $E\_weighted$ is explicitly **not** described as ground-truth reuse percentage.
2. **Deterministic Partial-Reuse Variant Construction Procedure:**  
   Added a dedicated subsection (Section 7) establishing: directional reuse $R^*(Q, T)$ (where $R^*(Q, T)$ does not necessarily equal $R^*(T, Q)$), candidate function selection, donor/unrelated padding, static dependency handling, frozen random seeds, deterministic integer rounding, clean compilation requirements, regression-test validation, and SHA-256 manifest logging.
3. **Corrected Embedding Normalization Description:**  
   Removed claims that `GraphEncoder` outputs normalized embeddings. Clarified that: *The frozen GraphEncoder produces a raw 16-dimensional embedding. Cosine similarity applies L2 normalization implicitly during similarity computation.* No normalization head or layer is introduced.
4. **Empirical Background Tail Probability Calibration:**  
   Replaced informal percentile rank descriptions with a formal non-parametric formulation: **Empirical Background Tail Probability** $p_{\text{bg}}(S) = (1 + \sum \mathbb{I}[S_k \ge S]) / (K + 1)$ and calibrated evidence $E_{\text{cal}} = 1 - p_{\text{bg}}(S)$, fit strictly on development data. Brier Score reporting is made strictly conditional on genuinely probabilistic calibration.
5. **Sample-Size Supported Low-FPR Operating Points:**  
   Removed mandatory fixed requirements for 1% and 5% FPR. Formulated reporting as: *TPR at predefined low-FPR operating points supported by the available number of independent negative pairs.*
6. **Refined Positive/Negative Classification Policy:**  
   Preserved Policy A (0% vs. 25–100%) as primary; refined Policy B for high-reuse sensitivity by defining 0% as negative, 25% as intermediate/excluded, and 50–100% as positive. Prohibited classifying known 25% reuse as negative.
7. **Independent AsiaCCS V2 Sealed-Test Incident Policy:**  
   Removed all references and dependencies on historical prior-stage incident recovery documentation. Established a self-contained policy for handling technical runtime failures with an absolute parameter freeze.
8. **Standardized Transformation & Validation Terminology:**  
   Replaced "loop interchange" with "equivalent loop-form rewriting (for <-> while)" and replaced claims of proving semantic equivalence with "regression-based validation of preserved observable behavior for the predefined test suite".
9. **Expanded Decision Framework (8 Open Decisions):**  
   Added formal decision blocks for Function Accepted-Match Threshold $\tau$ (`[DECISION 6]`), Dataset Allocation (`[DECISION 7]`), and Partial-Reuse Construction Procedure (`[DECISION 8]`). Recorded supervisor recommendations without automatically finalizing decisions.

---

## 1. Purpose

The primary purpose of this protocol is to define the scientific, methodological, and experimental standards for extending Mini-DeepBugger from a function-level binary similarity proof-of-concept into a robust binary code similarity framework capable of plagiarism-oriented code reuse analysis for ACM AsiaCCS 2027.

This document establishes:
1. Operational definitions and formal criteria for binary function similarity, directional partial program reuse, and calibrated reuse evidence.
2. The design, construction, and split-governance rules for the new Partial-Reuse Dataset V2.
3. The four-stage ablation framework (B1, B2, B3, and Proposed) isolating the contributions of one-to-one bipartite matching, complexity weighting, uniqueness weighting, and background calibration.
4. Evaluation protocols for both Level-A (function-level similarity and retrieval) and Level-B (program-level reuse ratio estimation and false-positive risk control).
5. Cryptographic reproducibility, split sealing, and audit standards to guarantee zero data leakage and prevent post-test tuning.

---

## 2. Scope

The research scope of this protocol is strictly bounded by the approved AsiaCCS 2027 research plan:
- **Central Research Topic:** Robust binary code similarity under cross-compiler, cross-optimization, and semantics-preserving code transformations.
- **Program-Level Extension:** Plagiarism-oriented code reuse analysis built directly upon function-level similarity representations.
- **Scientific Positioning:** Mini-DeepBugger is formulated as a *robust binary code similarity framework providing calibrated program-level reuse evidence*. It is **not** an automated plagiarism detector making legal or academic determinations; rather, it provides forensic evidence and reuse ratio estimates toward plagiarism identification.
- **Applicable Boundary:** This protocol applies solely to experiments, datasets, benchmarks, and evaluations conducted for the ACM AsiaCCS 2027 deadline (December 11, 2026).

---

## 3. Frozen Baseline

The experimental framework is built strictly upon the verified, uncorrupted Mini-DeepBugger baseline:
- **Repository Baseline Commit:** `04baeddf8116e632be6c8aeaa183872c9a8b8a6f`
- **Milestone Tag:** `next-baseline-freeze`
- **Frozen Architecture:** The frozen 2-layer Graph Convolutional Network (`GraphEncoder`) defined in `src/graph_encoder.py`:
  $$\text{GCNConv}(29, 32) \rightarrow \text{ReLU} \rightarrow \text{GCNConv}(32, 16) \rightarrow \text{ReLU} \rightarrow \text{global\_mean\_pool}$$
  operating on 29-dimensional semantic Attributed Control Flow Graphs (A-CFGs) without normalization heads or metric-learning projections.
- **Frozen Baseline Training Procedure:** In accordance with the baseline audit, the baseline does not utilize a serialized `.pt` checkpoint file on `main`. Instead, trained `GraphEncoder` instances are reconstructed deterministically in memory via `experiments/run_final_experiment.py`:
  - *Training Split:* Phase-1 train split from `data/variant_pair_manifest.csv` (10 function families compiled across O0–O3 under Clang).
  - *Loss Function:* `torch.nn.CosineEmbeddingLoss(margin=0.5)`.
  - *Optimizer:* Adam optimizer, learning rate $\eta = 0.01$, 100 epochs.
  - *Baseline Reconstruction Seeds:* Fixed 5-seed suite: `[42, 123, 2026, 7, 99]`.
- **Exclusion of Downstream Artifacts:** *Historical checkpoints from downstream experimental branches are excluded from AsiaCCS V2.* Specifically, the `PooledMLPEncoder` checkpoints trained with Supervised Contrastive Learning in historical branches (`checkpoints/final/pooled_mlp_seed*.pt`) from the opened independent test are strictly excluded. AsiaCCS V2 relies exclusively on the frozen architecture and training procedure of the clean baseline.

---

## 4. Research Questions

This study investigates three interconnected research questions:

### RQ1 — Robust Similarity
> **Can Mini-DeepBugger still detect semantically similar binary code after compiler, optimization, or selected semantics-preserving code changes?**
- **Focus:** Evaluates function-level similarity stability when binaries are subjected to Clang vs. GCC compilation, `-O0` vs. `-O3` optimization levels, and selected semantics-preserving control-flow rewrites and inlining variants.
- **Primary Evidence:** F1-score, ROC-AUC, Recall@1, and Mean Reciprocal Rank (MRR).

### RQ2 — Partial Reuse
> **Can function-level similarity be combined to estimate the proportion of eligible functions reused between two programs?**
- **Focus:** Evaluates program-level aggregation accuracy across five controlled reuse levels (0%, 25%, 50%, 75%, 100%) using unweighted, count-based reuse estimation.
- **Scientific Question:** *"How much eligible-function reuse occurred?"*
- **Primary Evidence:** Mean Absolute Error (MAE), Root Mean Squared Error (RMSE), Pearson correlation coefficient ($r$), and Spearman rank correlation coefficient ($\rho$).

### RQ3 — False Positives & Background Calibration
> **Can uniqueness-aware matching and background calibration reduce false positives on unrelated programs?**
- **Focus:** Evaluates whether weighting functions by their distinctive information and normalizing scores against empirical background distributions from unrelated software effectively suppresses false positive alerts.
- **Scientific Question:** *"How strong and unusual is the observed reuse evidence?"*
- **Primary Evidence:** False Positive Rate (FPR) on 0% reuse pairs, Precision-Recall AUC (PR-AUC), F1-score, True Positive Rate (TPR), True Positive Rate at predefined low-FPR operating points supported by the available number of independent negative pairs, and Brier Score (*strictly conditional on probabilistic calibration*).

---

## 5. Proposed Framework

The extended Mini-DeepBugger framework executes an end-to-end processing pipeline transforming binary executables into calibrated program-level reuse evidence:

```
Binary Program
      │
      ▼
Function Extraction ──────────► Filter Eligible Functions (Canonical F_canonical)
      │
      ▼
Semantic A-CFGs ──────────────► 29-D Node Attributes + Control-Flow Edges
      │
      ▼
Function Embeddings ──────────► Reconstructed GCN Encoder (Raw 16-D Embeddings)
      │
      ▼
Function Similarity ──────────► Pairwise Cosine Metric (Implicit L2 Normalization)
      │
      ▼
One-to-One Matching ──────────► Maximum-Weight Bipartite Matching (Raw Cosine Matrix)
      │
      ├───► Count-Based Ratio Estimate (R_hat_count) ────► RQ2: Reuse Proportion
      │
      └───► Coverage + Uniqueness (E_weighted) ──────────► Background Calibration
                                                                   │
                                                                   ▼
                                                          RQ3: Calibrated Evidence
```

### Key Methodological Components:
1. **Foundation Preservation:** The frozen 29-D semantic A-CFG and 2-layer GCN encoder extract raw 16-D function embeddings.
2. **One-to-One Bipartite Matching:** Given a query program $Q$ and target program $T$, each eligible query function is matched to at most one eligible target function using maximum-weight bipartite assignment (Hungarian / Linear Sum Assignment) executed over the **raw function cosine-similarity matrix**. Matching assignment is identical across B1, B2, B3, and Proposed.
3. **Decoupled Dual Outputs:**
   - **Count-Based Reuse Ratio ($R\_hat\_count$):** Directly estimates physical reuse proportion to address **RQ2** (*How much eligible-function reuse occurred?*).
   - **Weighted Evidence ($E\_weighted$):** Incorporates structural complexity and corpus uniqueness applied strictly post-matching to address **RQ3** (*How strong and unusual is the observed reuse evidence?*).
4. **Background Calibration:** Aggregated evidence scores are calibrated against empirical null distributions generated from disjoint, unrelated software pairs.

---

## 6. Dataset V2 Definition

To evaluate partial reuse without data contamination, a completely new, controlled dataset designated **Dataset V2** will be generated.

### 6.1 Program Corpus
- **Target Size:** Approximately 21 new canonical, self-contained C programs (meeting the ~20 canonical programs objective).
- **Domain Coverage:** Algorithmic routines, data structures, cryptographic primitives, string processing, networking utilities, and system utilities.
- **Isolation Constraint:** The 24 transformation programs and 528 binaries from the previous independent test (`data/manifests/transformation_dataset_v1.csv` and `data/independent_test/`) **MUST NOT** be reused for training, development, tuning, threshold selection, calibration selection, or model selection.

### 6.2 Controlled Partial-Reuse Levels
For each evaluated program pair $(Q, T)$, reuse is evaluated across five discrete nominal levels:
- **0% Nominal Reuse:** Disjoint programs sharing 0 eligible functions.
- **25% Nominal Reuse:** Approximately 25% of eligible functions reused.
- **50% Nominal Reuse:** Approximately 50% of eligible functions reused.
- **75% Nominal Reuse:** Approximately 75% of eligible functions reused.
- **100% Nominal Reuse:** Exactly 100% of eligible functions reused (full reuse under compiler/code variations).

### 6.3 Exact vs. Realized Ground-Truth Reuse Ratio
For the primary controlled benchmark, canonical source programs should preferably be selected with a canonical eligible-function count $M = |\mathcal{F}_{\text{canonical}}(Q)|$ such that:
$$M \pmod 4 = 0$$
(e.g., $M = 16, 20, 24, 28$), allowing 25%, 50%, and 75% nominal quartiles to be realized exactly as integers ($k = 0.25M, 0.50M, 0.75M$).

If a program has an eligible function count not divisible by 4, integer rounding $k = \text{round}(\alpha \cdot M)$ is applied. In all cases, ground truth is formally defined as the **realized ground truth**:
$$R^*(Q, T) = \frac{|\mathcal{F}_{\text{reused}}(Q, T)|}{|\mathcal{F}_{\text{canonical}}(Q)|} = \frac{k}{M}$$
Manifest records must explicitly document both nominal and realized figures:
- `nominal_reuse_level` (e.g., 0.25)
- `reused_function_count` ($k$)
- `canonical_eligible_query_count` ($M$)
- `realized_reuse_ratio` ($k / M$)

The realized ratio $R^*(Q, T)$ is the definitive value against which all regression metrics (MAE, RMSE, Pearson $r$, Spearman $\rho$) are calculated. Samples must never be falsely reported as exact nominal quartiles if their realized ratio differs.

### 6.4 Directional Ground-Truth Reuse Property
Reuse is strictly directional from query program $Q$ to target program $T$:
$$R^*(Q, T) \text{ is directional: } R^*(Q, T) \text{ does not necessarily equal } R^*(T, Q).$$
If $Q$ contains 16 eligible functions and $T$ borrows 8 functions from $Q$ alongside 24 native functions ($|\mathcal{F}_{\text{canonical}}(T)| = 32$), then $R^*(Q, T) = 8 / 16 = 0.50$ (50%), whereas $R^*(T, Q) = 8 / 32 = 0.25$ (25%).

---

## 7. Partial-Reuse Variant Construction Procedure

To ensure reproducibility, target program variants $T$ exhibiting controlled reuse levels relative to query program $Q$ must be constructed deterministically.

### 7.1 Construction Protocol
1. **Source Identification:** Let $Q$ be a canonical program with canonical source eligible function set $\mathcal{F}_{\text{canonical}}(Q) = \{f_1, f_2, \dots, f_M\}$, where $M = |\mathcal{F}_{\text{canonical}}(Q)|$.
2. **Subset Cardinality & Rounding Rule:** For target reuse fraction $\alpha \in \{0.0, 0.25, 0.50, 0.75, 1.0\}$, compute target count $k = \text{round}(\alpha \cdot M)$ with $0 \le k \le M$.
3. **Deterministic SHA-256 PRNG Selection:**  
   To prevent platform-dependent hash randomization (e.g., Python `PYTHONHASHSEED`), selection seeds must not use Python's built-in `hash()`. Instead, a deterministic cryptographic integer seed is derived:
   $$\text{seed\_material} = \text{canonical\_id} \parallel \text{nominal\_reuse\_level} \parallel \text{base\_dataset\_seed}$$
   $$\text{digest} = \text{SHA-256}(\text{seed\_material UTF-8})$$
   $$\text{seed\_int} = \text{int}(\text{digest}[0:8], 16)$$
   The PRNG is initialized with `seed_int` to select the candidate subset $\mathcal{F}_{\text{reused}} \subseteq \mathcal{F}_{\text{canonical}}(Q)$ of size $k$.  
   *Governance Rule:* The parameter `base_dataset_seed` is an explicit governance field that **MUST be formally finalized in conjunction with Decision 8** before generating any Dataset V2 source subset, and logged in manifests. It is not chosen automatically.
4. **Dependency-Closure Accounting:**
   - Any internal project-defined eligible callee function required by a selected function in $\mathcal{F}_{\text{reused}}$ **MUST** count toward $k$ and $R^*(Q, T)$.
   - Dependency closure must be computed before finalizing the reused subset.
   - A candidate subset is valid only when the final dependency-closed set of project-defined functions exactly equals $k$.
   - Runtime/library helpers excluded by the eligible-function policy do not count toward reuse ratio.
   - The complete closure must be logged to the manifest; no uncounted copied eligible functions are permitted.
5. **Donor / Non-Reused Function Replacement & Strict Cross-Split Isolation:**  
   To construct a realistic target program $T$ with total function count comparable to $M$, the remaining $M - k$ functions are drawn from an unrelated donor program $D$ belonging to a disjoint canonical family:
   $$\mathcal{F}_{\text{target}} = \mathcal{F}_{\text{reused}} \cup \mathcal{F}_{\text{donor\_subset}}$$
   where $\mathcal{F}_{\text{donor\_subset}} \subset \mathcal{F}_{\text{canonical}}(D)$ has size $M - k$.  
   *Strict Split Isolation Rule:* **The donor program $D$ MUST belong to the identical dataset split as $Q$:**
   $$\text{Query } Q \in \text{train} \implies \text{Donor } D \in \text{train}$$
   $$\text{Query } Q \in \text{dev} \implies \text{Donor } D \in \text{dev}$$
   $$\text{Query } Q \in \text{sealed\_test} \implies \text{Donor } D \in \text{sealed\_test}$$
   Under no circumstances may a target program synthesize components across partition boundaries. Donor assignments must be pre-frozen in manifests.
6. **Compilation & Linking Success Requirement:** Each synthesized target $T$ must compile and link cleanly under both Clang and GNU GCC without unresolved symbols or link errors.
7. **Regression-Based Operational Validation:**  
   Because partial-reuse target $T$ contains donor functions from $D$, requiring $T$ to exhibit whole-program behavioral equivalence to $Q$ is scientifically impossible for $k < M$. Operational correctness is verified via regression-based operational validation:
   - Reused functions from $Q$ must pass their isolated predefined unit test suites.
   - Donor functions from $D$ must pass their isolated predefined unit test suites.
   - Dependency closure must be fully resolved.
   - The composite binary must compile and link without errors.
   - An automated test harness must execute all included benchmark functions without crashes.
   - Whole-program behavioral correspondence to $Q$ is required strictly for the 100% full-reuse condition.
8. **Manifest & SHA-256 Logging:** The exact composition of $T$ (source functions, donor functions, seeds, compiler flags) is written to `manifests/dataset_v2_construction.csv` and sealed via SHA-256 checksums.

> `[FROZEN — APPROVED BY SUPERVISOR 2026-10-08]`  
> **Supervisory Decision on Partial-Reuse Construction Details (`[DECISION 8]`):**  
> - **Status:** FROZEN and APPROVED by human supervisor on 2026-10-08 prior to Dataset V2 generation.  
> - **Subset Selection Strategy:** Deterministic SHA-256 PRNG selection:  
>   $$\text{seed\_material} = \text{canonical\_id} \parallel \text{nominal\_reuse\_level} \parallel \text{base\_dataset\_seed}$$  
>   $$\text{digest} = \text{SHA-256}(\text{seed\_material UTF-8}), \quad \text{seed\_int} = \text{int}(\text{digest}[0:8], 16)$$  
> - **Base Dataset Seed:** Permanently frozen at `base_dataset_seed = 20261008` and recorded in manifests.  
> - **Dependency Closure:** Selected subset $\mathcal{F}_{\text{reused}}$ includes the initial selected function and all internal project-defined transitive dependencies within $Q$. The closed set must exactly equal realized count $k$.  
> - **Intra-Split Donor Pairing:** Donor program $D$ must strictly originate from the identical dataset partition ($Q \in \text{train} \implies D \in \text{train}$; $Q \in \text{dev} \implies D \in \text{dev}$; $Q \in \text{sealed\_test} \implies D \in \text{sealed\_test}$). Cross-split donor borrowing is strictly prohibited.  
> - **Directional Ground Truth:** Ground truth is strictly directional: $R^*(Q, T) = k / M$, while donor-side ground truth is $R^*(D, T) = k_{\text{donor}} / M_{\text{donor}}$. Both nominal level and realized ratio ($k / M$) are recorded in dataset manifests.  

---

## 8. Eligible-Function Definition

To prevent arbitrary sample selection, the boundary of what constitutes an "eligible function" must be mathematically and operationally frozen prior to dataset generation.

### 8.1 Two-Stage Canonical Eligibility vs. Variant Observability Decoupling
To resolve the operational inconsistency between source-level candidate identification and binary complexity metrics, eligibility is formally defined in two sequential stages:

1. **Stage A — Source-Level Function Identity:**  
   The canonical C source files define the closed list of project-defined candidate functions.
   - *Inclusion:* User-defined algorithmic routines, modules, and internal utilities originating in canonical source code.
   - *Exclusion:* External dynamic or statically linked standard library functions (libc routines: `printf`, `malloc`, `memcpy`, etc.), compiler-generated startup routines (`_start`, `frame_dummy`, `register_tm_clones`), and explicitly non-benchmark infrastructure/test scaffolding functions.

2. **Stage B — Canonical Reference-Build Non-Triviality Evaluation:**  
   The non-triviality rule (Section 8.2 / Decision 1) is evaluated **EXACTLY ONCE** on a designated, standardized **Canonical Reference Build**:
   - **Compiler:** `/usr/bin/clang` (Apple Clang 21.0.0)
   - **Optimization Level:** `-O0` (Unoptimized baseline, preserving function boundaries and stack frames)
   - **Boundary Condition:** Boundary-preserving (`__attribute__((noinline))` for benchmark functions)
   - **Architecture:** ARM64 (host reference architecture)  
   A source candidate function that satisfies the finalized non-triviality threshold in this canonical reference build becomes a member of the **Canonical Eligible Set** $\mathcal{F}_{\text{canonical}}(Q)$.

**Permanent Denominator Immutability:**  
Once established via the canonical reference build, the membership of $\mathcal{F}_{\text{canonical}}(Q)$ and the ground-truth denominator:
$$M = |\mathcal{F}_{\text{canonical}}(Q)|$$
are **frozen and permanently immutable**. Denominator $M$ **MUST NOT** change under GCC compilation, Clang variations, `-O3` optimization, control-flow transformations, inline/noinline stress conditions, or disassembly extraction failures.

3. **Variant Observability ($\mathcal{F}_{\text{extracted}}(Q_{\text{variant}})$):**  
   All subsequent compiled binary variants (across GCC, Clang, -O0, -O3, transformations, and inlining conditions) affect **ONLY** the observable variant-extracted set $\mathcal{F}_{\text{extracted}}(Q_{\text{variant}})$:
   - Binary extraction is an empirical property of each compiled variant.
   - For every compiled variant, the dataset manifest must record:
     - `canonical_eligible_count` ($M$)
     - `extracted_eligible_count` ($M_{\text{ext}} = |\mathcal{F}_{\text{extracted}}|$)
     - `extraction_rate` ($M_{\text{ext}} / M$)
     - `missing_function_ids` (canonical functions not observable due to inlining, dead-code elimination, or extraction failures).
   - If an eligible function is missing or inlined in a specific variant, it remains unmatched in bipartite matching, but **never** alters the fixed denominator $M$.

### 8.2 Minimum Complexity & Size Threshold Policy
Trivial stub functions (e.g., single-instruction return functions or pass-through wrappers) can distort reuse metrics and artificially inflate similarity.

> `[FROZEN — APPROVED BY SUPERVISOR 2026-10-08]`  
> **Supervisory Decision on Eligible Function Minimum Size Threshold (`[DECISION 1]`):**  
> - **Status:** FROZEN and APPROVED by human supervisor on 2026-10-08 prior to Dataset V2 generation.  
> - **Canonical Function Identity:** Established from source-level project-defined benchmark functions (excluding standard library routines, compiler runtime helpers, and non-benchmark scaffolding).  
> - **Canonical Reference Build:** Evaluated strictly once under:  
>   - Compiler: `/usr/bin/clang` (Apple Clang 21.0.0)  
>   - Optimization: `-O0`  
>   - Architecture: ARM64  
>   - Boundary Condition: Boundary-preserving (`__attribute__((noinline))`)  
> - **Approved Non-Triviality Rule (Option 2):** A candidate function is admitted to $\mathcal{F}_{\text{canonical}}(Q)$ if and only if in the canonical reference build:  
>   $$|V| \ge 1 \quad \text{and} \quad N_{\text{inst}} \ge 3 \text{ (excluding prologue/epilogue)}$$  
>   This excludes empty dummy stubs (`ret` only) while preserving compact algorithmic kernels. (Cyclomatic complexity is tracked as a derived diagnostic metadata field in manifests, but is NOT an independent eligibility gate).  
> - **Denominator Immutability:** Ground-truth denominator $M = |\mathcal{F}_{\text{canonical}}(Q)|$ is permanently frozen for program $Q$ across all subsequent compiler toolchains, optimization levels (-O3), source transformations, and inlining stress conditions.

---

## 9. Dataset Split Governance & Allocation

To eliminate any potential for data snooping or distribution leakage, Dataset V2 must adhere to strict split governance:

```
data/v2/
├── train/          (Reference / train canonical programs & variants for corpus statistics)
├── dev/            (Development & hyperparameter calibration programs)
├── sealed_test/    (Sealed independent test partition — 100% inaccessible during dev)
├── manifests/      (Immutable CSV manifests recording splits, families, and labels)
└── checksums/      (Cryptographic SHA-256 validation manifests)
```
*(Note: These directories will be created during Phase 2 upon protocol approval).*

### 9.1 Strict Governance Rules
1. **Source-Family Disjointness:** Canonical source programs assigned to `train`, `dev`, and `sealed_test` must be strictly mutually exclusive. No function or canonical program in `sealed_test` may appear in `train` or `dev`.
2. **Frozen Manifests:** Split assignments will be locked in `manifests/dataset_v2_splits.csv` before compiling variants and treated as immutable by project policy.
3. **Cryptographic Checksums / Hashes:** Master SHA-256 checksum manifests (`checksums/DATASET_V2_SHA256SUMS.txt`) will be generated immediately upon dataset construction.
4. **Automated Leakage Audit:** Prior to running experiments, an automated leakage detection script must verify that:
   $$\text{Family}(\text{train}) \cap \text{Family}(\text{dev}) = \emptyset$$
   $$\text{Family}(\text{train}) \cap \text{Family}(\text{sealed\_test}) = \emptyset$$
   $$\text{Family}(\text{dev}) \cap \text{Family}(\text{sealed\_test}) = \emptyset$$
5. **Sealed-Test Isolation:** The `data/v2/sealed_test/` partition remains strictly sealed and inaccessible until all baseline methods, weighting rules, and background calibration parameters are completely frozen.

### 9.2 Canonical Program Allocation
> `[FROZEN — APPROVED BY SUPERVISOR 2026-10-08]`  
> **Supervisory Decision on Dataset V2 Split Allocation (`[DECISION 7]`):**  
> - **Status:** FROZEN and APPROVED by human supervisor on 2026-10-08 prior to Dataset V2 generation.  
> - **Approved Canonical Allocation:** Total ~21 canonical, self-contained C programs partitioned as:  
>   - **7 Canonical Programs $\rightarrow$ `train` (Reference Corpus):** Used strictly for uniqueness/reference corpus statistics, train-only frequency estimation ($\text{freq}_{\text{ref}}(f)$), and approved Dataset V2 development/reference operations. Dataset V2 train/reference programs **MUST NOT** replace Phase-1 baseline training data. The baseline GraphEncoder is reconstructed **ONLY** using the frozen Phase-1 training procedure and Phase-1 training data defined by the verified clean baseline; retraining the baseline GraphEncoder on Dataset V2 is **NOT** authorized by this protocol.  
>   - **7 Canonical Programs $\rightarrow$ `dev` (Validation & Calibration):** Yields $C(7,2) = 21$ canonical unrelated pairs to fit the empirical background null distribution $\mathcal{D}_{\text{bg}}$, tune function-level threshold $\tau_{\text{function}}$, and calibrate method-specific decision thresholds.  
>   - **7 Canonical Programs $\rightarrow$ `sealed_test` (Independent Test Partition):** Yields 21 canonical unrelated pairs, providing approximately 4.76% ($1/21$) raw false-positive resolution at canonical pair level. Strictly isolated and sealed until all models and thresholds are frozen.  
> - **Statistical Grounding:** Low-FPR claims are grounded in canonical pair diversity ($C(7,2) = 21$ independent pairs per partition) rather than inflated variant counts.  

---

## 10. Build, Transformation & Toolchain Matrix

Every canonical program in Dataset V2 will be compiled across controlled compilation, optimization, and transformation dimensions:

### 10.1 Pinned Compiler Toolchain
To eliminate toolchain ambiguity and compiler wrapper aliasing (e.g., macOS `/usr/bin/gcc` aliasing Apple Clang), all builds must invoke explicit absolute executable paths:
- **Clang Condition:**  
  - Executable: `/usr/bin/clang`
  - Version: `Apple clang version 21.0.0 (clang-2100.1.1.101)`
  - Target Triple: `arm64-apple-darwin25.6.0`
- **GNU GCC Condition:**  
  - Executable: `/opt/homebrew/bin/gcc-15`
  - Version: `gcc-15 (Homebrew GCC 15.1.0) 15.1.0`
  - Target Triple: `aarch64-apple-darwin24`
  - *Prohibition:* Bare `gcc` must **NOT** be invoked on macOS because it routes to Apple Clang.
- **Disassembly Engine:**  
  - Executable: `/usr/bin/otool`
  - Version: `llvm-otool(1) cctools-1040, LLVM version 21.0.0`
- **Approved Optimization Scope:**  
  Optimization levels strictly remain:
  - `-O0` (Unoptimized baseline, preserving stack layout and direct instruction flow)
  - `-O3` (Aggressive optimization: vectorization, loop unrolling, instruction scheduling)  
  *(Note: Intermediate levels -O1 and -O2 are excluded from the AsiaCCS V2 scope).*

### 10.2 Experimental Conditions: Boundary-Preserving vs. Inlining Stress
In accordance with the approved research plan, the inlining experiment is preserved and structured into two distinct evaluation conditions:
1. **Primary Boundary-Preserving Condition:**  
   Used for principal cross-compiler (Clang vs. GCC) and cross-optimization (-O0 vs. -O3) program-level reuse comparisons (RQ1, RQ2, RQ3). Eligible benchmark functions maintain preserved function boundaries (using `__attribute__((noinline))` where necessary) to ensure controlled bipartite matching over full function sets.
2. **Inlining Robustness / Stress Condition:**  
   Evaluates resilience when selected eligible functions are subjected to:
   - Force inline: `__attribute__((always_inline))`
   - Prevent inline: `__attribute__((noinline))`  
   If inlining causes a function body to merge into callers and disappear from binary symbols:
   - Canonical ground truth remains unchanged ($M = |\mathcal{F}_{\text{canonical}}|$ is fixed).
   - The function is recorded as unextracted in `missing_function_ids`.
   - Results are reported in a dedicated Inlining Robustness analysis without distorting the primary program-level benchmark.

### 10.3 Semantics-Preserving Transformations
Selected semantics-preserving source and structural transformations will be evaluated:
1. **Control-Flow Rewrites:**
   - Equivalent loop-form rewriting (`for` $\leftrightarrow$ `while`).
   - Equivalent conditional restructuring (`if-else` reordering, ternary expression substitution, equivalent switch-case translation).
2. **Regression-Based Operational Validation:**  
   Every canonical program must include a deterministic regression test harness. A transformation is admitted into Dataset V2 **only if** the compiled binary passes 100% of regression unit tests, providing regression-based validation of preserved observable behavior for the predefined test suite.

---

## 11. Level-A Evaluation: Function-Level Similarity

Level-A evaluation measures the robustness of function embeddings and retrieval across compiler variations.

### 11.1 Evaluation Conditions
- **Cross-Compiler:** Clang $\leftrightarrow$ GCC (at identical optimization levels).
- **Cross-Optimization:** `-O0` $\leftrightarrow$ `-O3` (within the same compiler).
- **Cross-Transformation:** Original $\leftrightarrow$ Transformed (under identical compiler and optimization settings).
- **Inlining Stress:** Inlined $\leftrightarrow$ Non-inlined (evaluated over observable functions).

### 11.2 Retrieval Protocol & Implicit Normalization
The reconstructed `GraphEncoder` produces a raw 16-dimensional embedding. Cosine similarity applies L2 normalization implicitly during similarity computation:
$$s(f_q, f_g) = \cos(\mathbf{e}_q, \mathbf{e}_g) = \frac{\mathbf{e}_q \cdot \mathbf{e}_g}{\|\mathbf{e}_q\|_2 \|\mathbf{e}_g\|_2}$$
where $\mathbf{e}_q, \mathbf{e}_g \in \mathbb{R}^{16}$. No normalization layer or embedding head is added to the frozen encoder.

### 11.3 Function Similarity Threshold Selection
A matched pair $(f_q, f_t)$ is accepted as a valid match if $s(f_q, f_t) \ge \tau_{\text{function}}$.

> `[OPEN — GATED FOR DEV SELECTION]`  
> **Supervisory Decision on Function Accepted-Match Threshold $\tau_{\text{function}}$ (`[DECISION 6]`):**  
> - **Status:** OPEN — Gated for empirical selection on the `dev` partition prior to accessing `sealed_test`.  
> - The similarity threshold $\tau_{\text{function}}$ must be selected using `train`/`dev` splits only (e.g. optimizing F1 or Youden's $J$ on validation pairs).  
> - The `sealed_test` must not influence $\tau_{\text{function}}$.  
> - $\tau_{\text{function}}$ must be frozen before final testing and applied consistently across B1, B2, B3, and Proposed comparisons.  
> - *Candidate Grid:* $\tau_{\text{function}} \in [0.50, 0.85]$ in steps of 0.05.  

### 11.4 Level-A Metrics
- **Recall@1:** Fraction of queries where the true semantic clone ranks at rank 1.
- **Mean Reciprocal Rank (MRR):** $\text{MRR} = \frac{1}{|Q|} \sum_{i=1}^{|Q|} \frac{1}{\text{rank}_i}$.
- **Precision, Recall, and F1-Score:** Evaluated across function pairs at threshold $\tau_{\text{function}}$.
- **ROC-AUC:** Area under the ROC curve across all evaluated function pairs.

---

## 12. Level-B Evaluation: Program-Level Reuse Estimation & Evidence

Level-B evaluation measures program-level analysis across two distinct objectives: physical reuse proportion estimation (RQ2) and calibrated reuse evidence (RQ3).

### 12.1 Decoupled Program-Level Outputs
Let $\mathcal{M} = \{(f_q, f_t)\}$ denote the one-to-one matched pairs produced by maximum-weight bipartite matching between $Q$ and $T$ computed over the **same raw cosine-similarity matrix** $S = [s(f_q, f_t)]$. The accepted match set is:
$$\mathcal{M}_{\text{accepted}} = \{(f_q, f_t) \in \mathcal{M} \mid s(f_q, f_t) \ge \tau_{\text{function}}\}$$

#### A. Count-Based Reuse Ratio Estimate ($R\_hat\_count$) — Primary for RQ2
$$R\_hat\_count(Q, T) = \frac{|\mathcal{M}_{\text{accepted}}|}{|\mathcal{F}_{\text{canonical}}(Q)|}$$
- **Role:** Directly estimates the physical proportion of eligible functions reused.
- **Scientific Meaning:** RQ2 asks how much eligible-function reuse occurred.
- **Denominator Rule:** The denominator is strictly the canonical count $|\mathcal{F}_{\text{canonical}}(Q)| = M$.
- **Target Comparison:** Evaluated against realized ground-truth $R^*(Q, T)$ using **MAE, RMSE, Pearson ($r$), and Spearman ($\rho$)**.

#### B. Weighted Program-Level Reuse Evidence ($E\_weighted$) — Primary for RQ3
$$E\_weighted(Q, T) = \frac{\sum_{(f_q, f_t) \in \mathcal{M}_{\text{accepted}}} w(f_q)}{\sum_{f \in \mathcal{F}_{\text{canonical}}(Q)} w(f)}$$
where $w(f) = C(f) \cdot U(f)$ incorporates complexity $C(f)$ and uniqueness $U(f)$ applied strictly post-matching.
- **Role:** Measures the structural significance and rarity of the matched functions.
- **Scientific Meaning:** RQ3 asks how strong and unusual the reuse evidence is.
- **Target Comparison:** Input to background calibration; evaluated via **FPR, PR-AUC, and low-FPR TPR**.
- **Rule:** $E\_weighted$ must NOT be described as ground-truth reuse percentage.

---

## 13. Main Experimental Comparison

To rigorously isolate the contribution of each proposed enhancement, the protocol establishes a controlled 4-method ablation ladder. All four methods operate on the **identical dataset splits**, the **identical function embeddings**, and the **identical one-to-one bipartite matching assignment** (derived from the raw cosine-similarity matrix):

| Method Identifier | Matching Algorithm | Function Weighting Scheme | Calibration Strategy | Methodological Role |
| :--- | :--- | :--- | :--- | :--- |
| **B1 (Baseline 1)** | Bipartite (Hungarian) | None ($w(f) = 1$) | Fixed threshold $\theta_{\text{B1}}$ | Unweighted bipartite baseline |
| **B2 (Baseline 2)** | Bipartite (Hungarian) | Structural Complexity ($w(f) = C(f)$) | Fixed threshold $\theta_{\text{B2}}$ | Isolates complexity weighting effect (**B1 $\rightarrow$ B2**) |
| **B3 (Baseline 3)** | Bipartite (Hungarian) | Complexity + Uniqueness ($w(f) = C(f) \cdot U(f)$) | Fixed threshold $\theta_{\text{B3}}$ | Isolates uniqueness weighting effect (**B2 $\rightarrow$ B3**) |
| **Proposed** | Bipartite (Hungarian) | Complexity + Uniqueness ($w(f) = C(f) \cdot U(f)$) | Background-Calibrated ($p_{\text{bg}} \le \alpha_{\text{bg}}$) | Isolates background calibration effect (**B3 $\rightarrow$ Proposed**) |

### Controlled Experimental Significance:
- **B1 $\rightarrow$ B2:** Demonstrates whether weighting substantial functions higher than small stubs improves reuse evidence.
- **B2 $\rightarrow$ B3:** Demonstrates whether down-weighting ubiquitous helper functions suppresses false similarity on unrelated software.
- **B3 $\rightarrow$ Proposed:** Demonstrates whether empirical background calibration suppresses false positives across varying program sizes.
- **Assignment Invariance:** Complexity $C(f)$ and uniqueness $U(f)$ weights do NOT alter the bipartite matching objective; they are applied strictly post-matching during evidence accumulation.

### Method-Specific Program-Level Decision Threshold Policy:
To evaluate binary classification metrics (FPR, TPR, Precision, Recall, F1 for RQ3), method-specific operating thresholds are defined:
- **For B1:** Classify as positive if $E_{\text{raw}}(Q, T) \ge \theta_{\text{B1}}$.
- **For B2:** Classify as positive if $E_{\text{complexity}}(Q, T) \ge \theta_{\text{B2}}$.
- **For B3:** Classify as positive if $E_{\text{weighted}}(Q, T) \ge \theta_{\text{B3}}$.
- **For Proposed:** Classify as positive if $p_{\text{bg}}(E) \le \alpha_{\text{bg}}$ (equivalently, $E_{\text{cal}} \ge 1 - \alpha_{\text{bg}}$).

**Methodological Fair-Comparison Principle:**  
Because $E_{\text{raw}}$, $E_{\text{complexity}}$, $E_{\text{weighted}}$, and $E_{\text{cal}}$ operate on different numerical score scales, fair evaluation does **not** mean applying an identical numerical value across methods. Rather, fair comparison requires applying the **SAME PREDECLARED THRESHOLD-SELECTION POLICY** on the development set (`dev`):
- *Uniform Selection Policy:* Select each method-specific threshold ($\theta_{\text{B1}}$, $\theta_{\text{B2}}$, $\theta_{\text{B3}}$, $\alpha_{\text{bg}}$) on the `dev` partition using an identical predeclared objective (e.g., the threshold achieving a common predeclared low-FPR operating point on dev unrelated pairs, or maximizing F1 under Policy A).
- *Freezing Rule:* All method-specific thresholds must be formally frozen prior to accessing the `sealed_test` partition and never adjusted post-test.
- *Complementary Metric:* In addition to threshold-dependent metrics at operating points, threshold-independent Precision-Recall AUC (PR-AUC) must be reported across all methods.

> `[OPEN — GATED FOR DEV SELECTION]`  
> **Supervisory Decision on Method-Specific Program-Level Decision Thresholds (`[DECISION 9]`):**  
> - **Status:** OPEN — Gated for empirical selection on the `dev` partition under a uniform predeclared policy prior to accessing `sealed_test`.  
> - Selection criterion for method-specific thresholds ($\theta_{\text{B1}}$, $\theta_{\text{B2}}$, $\theta_{\text{B3}}$) and tail significance $\alpha_{\text{bg}}$ on `dev` split.  
> - *Candidate Options:*  
>   - Option A (Tune each threshold on dev to maximize F1 under Policy A).  
>   - Option B (Select each threshold corresponding to a common predeclared low-FPR target on dev unrelated pairs, where sample size supports it - Current Working Recommendation for Supervisor Review).  
>   - Option C (Fixed standard significance level $\alpha_{\text{bg}} = 0.05$ with corresponding dev operating thresholds for baselines).  
> - *Policy Rule:* All method-specific thresholds must be frozen before sealed-test execution. Fair comparison is enforced via a uniform development selection policy, accompanied by threshold-independent PR-AUC reporting.  

---

## 14. Complexity Weighting Policy

The complexity weight $C(f)$ assigns higher importance to functions containing substantial operational logic.

### 14.1 Baseline Candidate Measures
Inspection of the existing `src/acfg_builder.py` baseline reveals several directly extractable complexity signals:
1. **Basic Block Count ($|V|$):** Number of nodes in A-CFG (`data.num_nodes`).
2. **Control-Flow Edge Count ($|E|$):** Number of edges in A-CFG (`data.edge_index.shape[1]`).
3. **Total Instruction Count ($N_{\text{inst}}$):** Assembly instructions disassembled across basic blocks.
4. **Cyclomatic Complexity ($M$):** $M = |E| - |V| + 2$.

### 14.2 Formulation Options & Decision Required
> `[OPEN — GATED FOR DEV SELECTION]`  
> **Supervisory Decision on Complexity Weighting Function (`[DECISION 2]`):**  
> - **Status:** OPEN — Gated for empirical selection on the `dev` partition prior to accessing `sealed_test`.  
> - **Option A (Log-Scaled Instruction Count):** $C(f) = \log_2(1 + N_{\text{inst}}(f))$. Robust against volume outliers; ignores branching.  
> - **Option B (Sublinear Graph & Cyclomatic Combination):** $C(f) = \sqrt{|V(f)| \cdot M(f)}$. Couples size with decision density.  
> - **Option C (Log-Scaled Basic Block Count - Current Working Recommendation for Supervisor Review):** $C(f) = \ln(1 + |V(f)|)$. Simplest, mathematically stable, directly available from PyG graph metadata (`data.num_nodes`).  

---

## 15. Uniqueness Weighting Policy

The uniqueness weight $U(f)$ down-weights functions that frequently recur across diverse programs (boilerplate, common utility patterns) while boosting rare, distinctive application logic.

### 15.1 Conceptual Formulation
Adopting an Inverse Document Frequency (IDF) formulation:
$$U(f) = \ln\left(1 + \frac{N_{\text{ref}}}{1 + \text{freq}_{\text{ref}}(f)}\right)$$
where:
- $N_{\text{ref}}$ is the total number of canonical programs in the reference corpus (`train` partition).
- $\text{freq}_{\text{ref}}(f)$ is the number of programs in the reference corpus containing a function semantically similar to $f$ ($s(f, f') \ge \tau_{\text{unique}}$).

### 15.2 Split Leakage Prevention Rule
To guarantee zero leakage:
1. **Reference Corpus Isolation:** The reference frequency $\text{freq}_{\text{ref}}(f)$ **MUST ONLY** be computed over the `train` partition.
2. **Prohibition:** Under no circumstances may program frequencies be calculated, adjusted, or updated using the `dev` or `sealed_test` partitions.

> `[OPEN — GATED FOR DEV SELECTION]`  
> **Supervisory Decision on Uniqueness Clustering Threshold (`[DECISION 3]`):**  
> - **Status:** OPEN — Gated for empirical selection on the `dev` partition (using `train` corpus frequencies) prior to accessing `sealed_test`.  
> - Threshold $\tau_{\text{unique}}$ defining function equivalence for corpus frequency.  
> - *Candidate Options:* $\tau_{\text{unique}} \in \{0.75, 0.80, 0.85\}$ (Current Working Recommendation for Supervisor Review: IDF-like uniqueness using `train`/reference corpus only).  

---

## 16. Background Calibration Policy

Raw similarity scores between unrelated programs vary depending on binary size and standard language idioms. Background calibration normalizes similarity relative to an empirical null distribution.

### 16.1 Empirical Background Null Distribution
Using the `dev` partition (strictly disjoint from `sealed_test`):
1. Form all pairs of mutually unrelated canonical programs $(P_A, P_B)$ where ground-truth reuse is known to be 0%.
2. Compute the uncalibrated aggregated similarity score $S_{\text{raw}}(P_A, P_B)$ for each negative pair.
3. Construct the empirical background null distribution $\mathcal{D}_{\text{bg}} = \{S_{\text{raw}}^{(1)}, S_{\text{raw}}^{(2)}, \dots, S_{\text{raw}}^{(K)}\}$.

### 16.2 Calibration Options & Decision Required
> `[OPEN — GATED FOR DEV SELECTION]`  
> **Supervisory Decision on Background Calibration Function (`[DECISION 4]`):**  
> - **Status:** OPEN — Gated for empirical selection on the `dev` partition prior to accessing `sealed_test`.  
> - **Option 1: Empirical Background Tail Probability (Current Working Recommendation for Supervisor Review):**  
>   $$p_{\text{bg}}(S) = \frac{1 + \sum_{i=1}^K \mathbb{I}[S_{i,\text{bg}} \ge S]}{K + 1}$$  
>   Calibrated evidence index:  
>   $$E_{\text{cal}} = 1 - p_{\text{bg}}(S)$$  
>   *Interpretation:* High $E_{\text{cal}}$ means the score is unusual relative to unrelated programs. This is evidence relative to an empirical background distribution. It is **NOT a probability of plagiarism**. Constructed using development data only; no sealed-test influence. Requires no Gaussian assumption.  
> - **Option 2: Gaussian Z-Score Normalization:**  
>   $$Z(S) = \frac{S - \mu_{\text{bg}}}{\sigma_{\text{bg}}}$$  
>   Parametric score measuring standard deviations above noise; unbounded output.  
> - **Option 3: Logistic Sigmoid Probability Calibration (Platt Scaling):**  
>   $$P(\text{Reuse} \mid S) = \frac{1}{1 + \exp(A \cdot S + B)}$$  
>   Parametric logistic fit producing posterior probabilities; enables Brier Score reporting.  
>  
> *Policy Rule:* Calibration parameters must be fit on `dev` data **only**. The sealed test must never influence calibration selection.  

---

## 17. Positive / Negative Classification Policy

To evaluate classification metrics (FPR, TPR, Precision, Recall, F1, PR-AUC), a formal operational boundary separating "positive" (meaningful code reuse) from "negative" (non-reuse / independent development) must be frozen prior to main testing.

> `[OPEN — GATED FOR DEV SELECTION]`  
> **Supervisory Decision on Ground-Truth Classification Policy (`[DECISION 5]`):**  
> - **Status:** OPEN — Gated for confirmation on `dev` partition prior to accessing `sealed_test`.  
> - **Policy A (Primary Candidate — Zero-Tolerance Boundary):**  
>   - Negative = 0% ($R^* = 0.0$).  
>   - Positive = 25%, 50%, 75%, 100% ($R^* \in \{0.25, 0.50, 0.75, 1.00\}$).  
>   - *Scientific Role:* Tests whether the framework detects *any* borrowing, including minor 25% borrowings.  
> - **Policy B (Secondary High-Reuse Sensitivity Candidate):**  
>   - Negative = 0% ($R^* = 0.0$).  
>   - Intermediate / excluded from this secondary binary analysis = 25% ($R^* = 0.25$ excluded to avoid mislabeling partial reuse as negative).  
>   - Positive = 50%, 75%, 100% ($R^* \in \{0.50, 0.75, 1.00\}$).  
>   - *Scientific Role:* Evaluates binary separation power for substantial high-reuse cases.  
>  
> *Policy Rule:* Do NOT classify known 25% reuse as negative. The classification policy must be formally frozen before running main experiments. (Current Working Recommendation for Supervisor Review: Policy A as primary; Policy B as high-reuse sensitivity analysis).  

---

## 18. Statistical Unit-of-Analysis & Metrics Summary

### 18.1 Statistical Unit-of-Analysis Policy (Pseudo-Replication Prevention)
In empirical software engineering, compiling the same source program under multiple compilers (Clang, GCC) and optimization levels (-O0, -O3) produces non-independent binary variants. Treating each variant pair as an independent statistical sample creates severe **pseudo-replication**, artificially deflating standard errors and overstating statistical power.

**Statistical Governance Rules:**
1. **Primary Inferential Unit:** The primary statistical unit of analysis for Level-B evaluation is the **canonical program pair** $(P_i, P_j)$.
2. **Canonical Sample Size:** For the recommended 7-program sealed test split, there are exactly:
   $$C(7, 2) = 21 \text{ unordered canonical program pairs}$$
   Delivering approximately 4.76% ($1/21$) raw false-positive resolution on unrelated pairs.
3. **Clustered Aggregation:**
   - Level-B metrics must be reported via **macro-aggregation across variants per canonical pair** (averaging metric scores across build variants for each canonical pair) or evaluated using **clustered bootstrap resampling** grouped by canonical program pair.
   - Low-FPR claims must be grounded in canonical pair diversity rather than inflated variant counts.
   - Cross-compiler and cross-optimization variant pairs are reported as structured sensitivity sub-dimensions.

### 18.2 Metrics Summary Table

| Evaluation Tier | Metric Name | Mathematical Definition / Objective | Primary Scientific Goal |
| :--- | :--- | :--- | :--- |
| **Level A: Function** | Precision, Recall, F1 | Standard binary classification metrics on function pairs | Pairwise matching quality |
| **Level A: Function** | ROC-AUC | Area under the True Positive vs False Positive Rate curve | Overall discrimination |
| **Level A: Function** | Recall@1 | $\mathbb{I}[\text{Rank}(\text{True Partner}) = 1]$ | Top-1 retrieval accuracy |
| **Level A: Function** | MRR | $\frac{1}{|Q|} \sum_{i=1}^{|Q|} \frac{1}{\text{rank}_i}$ | Mean rank quality in gallery |
| **Level B: Reuse** | MAE | $\frac{1}{N} \sum |R^* - R\_hat\_count|$ | Absolute reuse estimation error (RQ2) |
| **Level B: Reuse** | RMSE | $\sqrt{\frac{1}{N} \sum (R^* - R\_hat\_count)^2}$ | Penalizes large estimation errors (RQ2) |
| **Level B: Reuse** | Pearson ($r$) | $\frac{\text{Cov}(R^*, R\_hat\_count)}{\sigma_{R^*} \sigma_{R\_hat\_count}}$ | Linear alignment with truth (RQ2) |
| **Level B: Reuse** | Spearman ($\rho$) | Monotonic rank correlation between $R^*$ and $R\_hat\_count$ | Monotonic ranking consistency (RQ2) |
| **Level B: Risk** | FPR | $\frac{\text{FP}}{\text{FP} + \text{TN}}$ on 0% reuse pairs | False positive suppression (RQ3) |
| **Level B: Risk** | PR-AUC | Area under Precision-Recall curve | Performance under class imbalance (RQ3) |
| **Level B: Risk** | $\text{TPR}_{\text{low-FPR}}$ | TPR at predefined low-FPR operating points supported by available independent negative canonical pairs | High-confidence detection (RQ3) |
| **Level B: Calibration** | Brier Score | $\frac{1}{N} \sum (P_i - y_i)^2$ (*Conditional: Option 3 only*) | Probabilistic calibration error |

### Mandatory Brier Score Rule:
Brier Score will **only** be computed if the finalized background calibration method outputs genuinely probabilistic values in $[0, 1]$ (Option 3). If calibration outputs non-probabilistic evidence scores (Options 1 or 2), Brier Score is mathematically inapplicable and will **not** be reported.

### Low-FPR Operating Point Rule:
Rather than mandating fixed 1% or 5% FPR requirements that lack statistical power on a 21-pair canonical corpus, low-FPR operating points will be defined prior to testing based on the empirical number of independent negative canonical pairs available ($\sim 4.76\%$ per pair).

---

## 19. AsiaCCS V2 Sealed-Test Incident Policy

The `sealed_test` partition represents the definitive independent evaluation of the AsiaCCS 2027 study. If an unhandled technical exception occurs during sealed test evaluation, the following incident response policy is strictly enforced:

1. **Immediate Execution Halt:** If a purely technical failure occurs after sealed-test access, stop execution immediately. No subsequent inference or inspection of remaining test samples is permitted.
2. **Access Event Logging:** Log the access event, process ID, timestamp, and failure stack trace to `results/v2/sealed_incident.log`.
3. **Data Inspection Ban:** Do not inspect additional predictions, intermediate similarity matrices, or metric summaries for the partial run.
4. **Failure Classification:** Classify the failure as either:
   - **Implementation-Only:** Mechanical bugs (e.g., Python `TypeError`, missing import, file path format mismatch) where logic did not corrupt model state or leak test labels.
   - **Methodology-Affecting:** Conceptual errors in thresholding, feature extraction, or model weights.
5. **Repair Restrictions:**
   - Only implementation-only repair may be considered.
   - **ABSOLUTE FREEZE:** The following **MUST NOT** be modified: model parameters, thresholds, weighting functions, calibration, dataset composition, manifests, or evaluation definitions.
6. **Audit Disclosure:** Record failure reason, patch commit, exact code change, access count, and rerun justification in the reproducibility log. Any rerun must be disclosed in the experiment audit record.
7. **Methodology Failure Stop:** Methodology-affecting failure requires stopping the sealed test and supervisor review.

---

## 20. Multi-Tier Reproducibility Requirements

To ensure complete experimental auditability across all research stages:

### 20.1 Model Reproducibility
- **Frozen Architecture:** Immutable GCN definition in `src/graph_encoder.py`.
- **Deterministic Retraining:** Reconstructed via `experiments/run_final_experiment.py` on Phase-1 data.
- **Baseline Training Seeds:** Fixed 5-seed suite: `[42, 123, 2026, 7, 99]`.
- **Backend Sensitivity & Consistency Notice:** Fixed seeds support reproducible model reconstruction under the frozen software/hardware backend. Official runs must verify repeated-run consistency. Bitwise-identical behavior across different backends or accelerators is not assumed.
- **Mandatory Environment Telemetry:** Official experiment runs must log:
  - Python interpreter version
  - PyTorch version (`torch.__version__`)
  - PyG version (`torch_geometric.__version__`)
  - Device / backend (`cpu` or `mps`)
  - CPU architecture (ARM64 Apple Silicon)
  - PyTorch deterministic backend settings where applicable

### 20.2 Dataset Reproducibility
- **Deterministic PRNG Seeding:** Derived via $\text{SHA-256}(\text{canonical\_id} \mid \text{nominal\_reuse\_level} \mid \text{base\_dataset\_seed})$ mapped to integer seeds.
- **Immutable Split Manifests:** Recorded in `manifests/dataset_v2_splits.csv`.
- **Cryptographic Hashes:** Master checksum file (`SHA256SUMS.txt`) sealing canonical C sources, synthesized targets, and compiled binaries.

### 20.3 Toolchain Reproducibility
- **Compiler Binaries:** Explicit paths `/opt/homebrew/bin/gcc-15` (Homebrew GCC 15.1.0) and `/usr/bin/clang` (Apple Clang 21.0.0).
- **Disassembler:** `/usr/bin/otool` (`cctools-1040, LLVM 21.0.0`).
- **Official Python Runtime:** **Python 3.12.12** is the official AsiaCCS V2 Python interpreter runtime. (`requirements-frozen.txt` pins package dependencies; Python 3.14 is explicitly NOT part of the frozen official experimental environment unless separately validated in the future).
- **Dependencies:** Pinned via `requirements-frozen.txt` (`torch==2.13.0`, `torch-geometric==2.8.0.post1`, `networkx==3.6.1`, `scikit-learn==1.9.0`).
- **Git State Sealing:** Milestones sealed via immutable Git tags (e.g., `dataset-v2-freeze`, `asiaccs-final-freeze`) treated as immutable by project policy.

---

## 21. Experiment Logging Requirements

All experimental runs must generate persistent, machine-readable artifacts:
1. **Per-Run Manifests:** Record experiment ID, Git commit hash, random seed, execution timestamp, compiler paths, compiler banners, target triples, and hardware platform.
2. **Tabular Results:** Save metrics in standardized CSV files (`function_similarity_results.csv`, `program_reuse_results.csv`).
3. **Execution Logs:** Capture raw `stdout` and `stderr` streams in designated `results/v2/<experiment_id>/run.log` files.
4. **Configuration Snapshots:** Save exact JSON copies of input configurations alongside generated predictions.

---

## 22. Research Schedule

The research schedule adheres strictly to the approved timeline:

| Date Window | Research Phase | Deliverables / Milestone |
| :--- | :--- | :--- |
| **Oct 6 – 9** | Scope & Protocol | `EXPERIMENT_PROTOCOL_V2.md` draft & supervisory sign-off |
| **Oct 10 – 20** | Dataset V2 Construction | Canonical source collection, variant compilation, split freezing |
| **Oct 21 – 31** | Program Scoring | Bipartite matching, complexity weighting, uniqueness weighting |
| **Nov 1 – 10** | Calibration | Empirical background null distribution & calibration selection |
| **Nov 11 – 18** | Main Experiments | Evaluation of B1, B2, B3, and Proposed methods on dev split |
| **Nov 19 – 23** | Robustness & Ablation | Compiler cross-evaluations, threshold sensitivity analysis |
| **Nov 24 – 25** | Final Pre-Test Freeze | Cryptographic sealing of models, configs, manifests, checksums |
| **Nov 26 – 28** | Final Sealed Test | One-time execution on `sealed_test` partition |
| **Nov 29 – 30** | Final Analysis & Audit | Metric disclosure, statistical significance, audit sign-off |
| **Nov 30** | **EXPERIMENTAL CUTOFF** | **Absolute freeze: Zero new experiments or tuning permitted** |
| **Dec 1 – 10** | Paper Writing & Revision | LaTeX draft, figures, camera-ready tables, supervisory review |
| **Dec 11, 2026** | **ACM AsiaCCS 2027 Submission** | Final manuscript submission to ACM AsiaCCS 2027 |

---

## 23. Stop Conditions

An experiment or pipeline must halt immediately upon encountering any of the following conditions:
1. **Split Overlap Detection:** Automated leakage check detects identical canonical families across splits.
2. **Sealed-Test Premature Access:** Any script attempts to read `data/v2/sealed_test/` prior to the authorized one-shot execution window.
3. **Checksum Mismatch:** Any source, manifest, or checkpoint hash deviates from `SHA256SUMS.txt`.
4. **Non-Deterministic Execution:** Inconsistent metric outputs observed when re-running identical seeds under frozen configs.
5. **Experimental Cutoff Exceeded:** Any proposed tuning or experiment modification occurring after November 30, 2026.

---

## 24. Decision Summary Table

The status of the 9 key methodological decisions following the 2026-10-08 Human Supervisory Protocol Freeze is summarized below. Decisions 1, 7, and 8 are formally frozen and approved to authorize Dataset V2 generation. Decisions 2, 3, 4, 5, 6, and 9 remain explicitly open and gated for empirical evaluation on the `dev` partition prior to accessing `sealed_test`:

| Decision ID | Section | Topic | Governance Status | Frozen Resolution / Gated Selection Criteria |
| :--- | :--- | :--- | :--- | :--- |
| `[DECISION 1]` | 8.2 | **Eligible Function Size Threshold** | `[FROZEN — APPROVED BY SUPERVISOR 2026-10-08]` | **Option 2 Approved:** Evaluated strictly once on canonical reference build (`/usr/bin/clang`, `-O0`, boundary-preserving `__attribute__((noinline))`, ARM64). Rule: $|V| \ge 1$ and $N_{\text{inst}} \ge 3$ (excluding prologue/epilogue); cyclomatic complexity is a derived diagnostic field only (not an eligibility gate). Ground-truth denominator $M = |\mathcal{F}_{\text{canonical}}(Q)|$ permanently frozen. |
| `[DECISION 2]` | 14.2 | **Complexity Weighting $C(f)$** | `[OPEN — GATED FOR DEV SELECTION]` | Candidate formulations (Option A: $\log_2(1+N_{\text{inst}})$; Option B: $\sqrt{|V| \cdot M}$; Option C: $\ln(1+|V|)$). Gated for selection on `dev` partition. |
| `[DECISION 3]` | 15.2 | **Uniqueness Threshold $\tau_{\text{unique}}$** | `[OPEN — GATED FOR DEV SELECTION]` | Clustering threshold $\tau_{\text{unique}} \in \{0.75, 0.80, 0.85\}$ computed strictly on `train`/reference corpus. Gated for selection on `dev` partition. |
| `[DECISION 4]` | 16.2 | **Background Calibration Method** | `[OPEN — GATED FOR DEV SELECTION]` | Calibration model (Option 1: Empirical tail probability $E_{\text{cal}} = 1 - p_{\text{bg}}$; Option 2: Gaussian z-score; Option 3: Platt scaling). Gated for selection on `dev` partition. |
| `[DECISION 5]` | 17 | **Classification Policy** | `[OPEN — GATED FOR DEV SELECTION]` | Operational boundary (Policy A: 0% vs 25–100% primary; Policy B: 0% vs 50–100% secondary). Gated for confirmation on `dev` partition. |
| `[DECISION 6]` | 11.3 | **Function Match Threshold $\tau_{\text{function}}$** | `[OPEN — GATED FOR DEV SELECTION]` | Grid search $\tau_{\text{function}} \in [0.50, 0.85]$ in steps of 0.05 on `train`/`dev` pairs. Gated for freezing on `dev` prior to sealed test. |
| `[DECISION 7]` | 9.2 | **Dataset Allocation** | `[FROZEN — APPROVED BY SUPERVISOR 2026-10-08]` | **Approved Canonical Allocation:** Total ~21 canonical programs partitioned as **7 `train` / 7 `dev` / 7 `sealed_test`** ($C(7,2) = 21$ canonical unrelated pairs per partition, yielding ~4.76% raw false-positive resolution). Dataset V2 `train` is used strictly for uniqueness corpus statistics ($\text{freq}_{\text{ref}}$) and never retrains the baseline `GraphEncoder` (which is reconstructed solely on Phase-1 data). |
| `[DECISION 8]` | 7.1 | **Partial-Reuse Construction Procedure** | `[FROZEN — APPROVED BY SUPERVISOR 2026-10-08]` | **Approved Construction Procedure:** Deterministic SHA-256 PRNG selection ($\text{SHA256}(\text{canonical\_id} \parallel \text{nominal\_reuse\_level} \parallel \text{base\_dataset\_seed})$); transitive dependency closure within $Q$; intra-split donor pairing; directional ground truth $R^*(Q, T) = k / M$; frozen `base_dataset_seed = 20261008`. |
| `[DECISION 9]` | 13 | **Method-Specific Program Thresholds ($\theta_{\text{B1}}, \theta_{\text{B2}}, \theta_{\text{B3}}, \alpha_{\text{bg}}$)** | `[OPEN — GATED FOR DEV SELECTION]` | Selection policy for method-specific decision thresholds on `dev` (e.g., Option B: uniform low-FPR operating point). Gated for freezing on `dev` prior to sealed test, with threshold-independent PR-AUC reporting. |

---

## 25. Explicit Out-of-Scope Items

To prevent scope creep and maintain strict deadline adherence, the following directions are formally declared **OUT OF SCOPE** for this AsiaCCS 2027 study:
- **New GNN Architectures:** Introducing GraphSAGE, Graph Attention Networks (GAT), or transformer-based graph encoders.
- **Metric Learning Overhauls:** Implementing Supervised Contrastive Learning (SupCon) or triplet loss variants during this phase.
- **Historical Checkpoint Reuse:** Ingesting or evaluating historical `PooledMLPEncoder` checkpoints from the opened independent test.
- **Disassembler Migrations:** Migrating to Capstone, LIEF, or IDA Pro (disassembly remains based on the verified baseline pipeline).
- **Large-Scale Retrieval Scaling:** Implementing approximate nearest neighbors (HNSW / FAISS) or large-scale galleries ($> 10^5$ functions).
- **Broad Cross-Architecture Evaluation:** Expanding to multi-CPU cross-architecture evaluation (e.g., x86 $\leftrightarrow$ ARM $\leftrightarrow$ MIPS).
- **External BinKit-Scale Expansion:** Ingesting large-scale external multi-package benchmark suites.
- **Advanced Obfuscation:** Commercial packers, virtualization obfuscation (Tigress / VMProtect), or instruction virtualization.
- **Fine-Grained Subgraph Plagiarism:** Exact AST/statement-level token plagiarism detection.
- **Automated Legal Plagiarism Verdicts:** Claiming definitive automated plagiarism adjudication.
- **DeepWeak Vulnerability Extensions:** Expanding vulnerability-specific security feature extractors.
- **Reuse of Previous Independent Test:** Ingesting the 24-program transformation suite evaluated under Protocol V2.
- **Post-Sealed-Test Tuning:** Re-running or adjusting models after observing sealed test metrics.

---
*(End of Protocol Document — Revision 2.2.1 Approved and Frozen on 2026-10-08).*
