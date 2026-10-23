# Project Progress Log

- **Project:** DeepBugger Extension
- **Master Plan reference:** `DeepBugger_Extension_Final_Master_Plan_v4.md`
- **Current phase:** Phase 0 — Baseline Reproduction and Freeze
- **Current status:** Project initialized
- **Operating Mandate:** All future steps must strictly follow `AGENTS.md`.

---

### STEP-000 — establish DeepBugger agent workflow

- Date: 2026-10-23
- Status: DONE
- Goal: Establish safe, persistent working rules (`AGENTS.md`) and initialize the chronological project execution log (`PROJECT_PROGRESS.md`).
- Master Plan section: Sections 0–48 (Full Master Plan read; Section 37 Phase 0 & Section 43 Immediate Next Actions)
- Files inspected: `DeepBugger_Extension_Final_Master_Plan_v4.md`
- Files changed: `AGENTS.md`, `PROJECT_PROGRESS.md`
- Commands executed: `git status`, `git remote -v`, `git config user.name`, `git config user.email`, `git diff`, `git add AGENTS.md PROJECT_PROGRESS.md`, `git commit -m "step-000: establish DeepBugger agent workflow"`
- Verification performed: Read `AGENTS.md` and `PROJECT_PROGRESS.md` after writing; verified all 11 mandatory rules and checklist items are present; checked `git status` and `git diff` to confirm only `AGENTS.md` and `PROJECT_PROGRESS.md` were created/staged.
- Result: Persistent operating rules and progress tracking initialized and committed locally.
- Decisions made: Referenced root-level `DeepBugger_Extension_Final_Master_Plan_v4.md` as the research source of truth per user instruction; left `DeepBugger_Extension_Final_Master_Plan_v4.md` untracked so only the two permitted files (`AGENTS.md` and `PROJECT_PROGRESS.md`) were staged and committed.
- Open questions: None
- Git commit: Created in `step-000: establish DeepBugger agent workflow` (see git log / completion report for exact hash)
- Next proposed micro-project: `STEP-001 — audit Mini-DeepBugger repository` (read-only audit of existing repository structure, baseline code, datasets, manifests, configs, and historical results to prepare for Phase 0 baseline freeze).

---

### STEP-000A — Track DeepBugger Master Plan

- Date: 2026-10-23
- Status: DONE
- Goal: Add the approved DeepBugger Extension Final Master Plan v4 to version control as the project research source of truth.
- Files inspected:
  - DeepBugger_Extension_Final_Master_Plan_v4.md
- Files changed:
  - PROJECT_PROGRESS.md
- Files added:
  - DeepBugger_Extension_Final_Master_Plan_v4.md
- Verification performed:
  - Confirmed AGENTS.md was restored to committed state.
  - Confirmed PROJECT_PROGRESS.md accidental whitespace diff was removed before this entry.
  - Confirmed the Master Plan file was not modified.
  - Confirmed no research code or dataset files changed.
- Result: Master Plan added to repository tracking.
- Decisions made:
  - Keep the Master Plan at its current repository path for now.
  - Do not move or rename it during housekeeping.
- Open questions: None
- Git commit:
- Next proposed micro-project: STEP-001 — Audit Mini-DeepBugger repository

---

### STEP-000B — Create DeepBugger Extension branch

- Date: 2026-10-23
- Status: DONE
- Goal: Separate DeepBugger Extension development from the historical Mini-DeepBugger/APSEC baseline branch.
- Starting branch: main
- Extension branch: deepbugger-extension
- Files inspected:
  - AGENTS.md
  - PROJECT_PROGRESS.md
  - DeepBugger_Extension_Final_Master_Plan_v4.md
- Files changed:
  - PROJECT_PROGRESS.md
- Verification performed:
  - Confirmed main was clean before branching.
  - Confirmed no research code or dataset changes were introduced.
  - Confirmed current branch is deepbugger-extension.
  - Confirmed working tree was clean before progress-log update.
- Decision:
  - main is baseline/reference only.
  - all future DeepBugger Extension work must occur on deepbugger-extension.
- Open questions: None
- Git commit: f6d9201
- Next proposed micro-project: STEP-001 — Audit Mini-DeepBugger repository

---

### STEP-001 — Audit Mini-DeepBugger repository

- Date: 2026-10-23
- Status: DONE
- Goal: Perform a read-only audit of the existing Mini-DeepBugger repository to map the historical APSEC baseline pipeline, locate the exact 29-D semantic A-CFG implementation, separate DeepClone and DeepWeak, classify datasets/manifests/artifacts, and identify risks and unresolved items before any extension work.
- Master Plan section: Sections 6, 10, 11, 22, 31, 37 (Phase 0 — Baseline Reproduction and Freeze), 41, 43
- Branch: deepbugger-extension
- Files/directories inspected:
  - Root: `.gitignore`, `.gitattributes`, `README.md`, `requirements.txt`, `requirements-frozen.txt`, `test_environment.py`, `AGENTS.md`, `PROJECT_PROGRESS.md`, `DeepBugger_Extension_Final_Master_Plan_v4.md`
  - `config/experiment.json`, `configs/experiment.yaml`
  - `src/`: `__init__.py`, `acfg_builder.py`, `graph_encoder.py`, `deepweak_features.py`, `deepweak_graph_builder.py`, `deepweak_classifier.py`, and subdirectories (`adapters/`, `evaluation/`, `features/`, `models/`, `preprocessing/`, `program_similarity/`, `retrieval/`)
  - `experiments/`: `run_final_experiment.py`, `evaluate_clone_pooled_mlp_baseline.py`, `train_deepweak_w1_v2.py`, `evaluate_deepweak_w2_frozen.py`, `evaluate_deepweak_w2_ablation.py`, `build_deepweak_manifest.py`, `build_deepweak_w1_split_v2.py`, `build_deepweak_w2_manifest.py`, `generate_paper_figures.py`, plus helper/check scripts
  - `data/`: `variant_pair_manifest.csv`, `phase2_independent_manifest.csv`, `deepweak_manifest.csv`, `deepweak_w1_split.csv`, `deepweak_w1_split_v2.csv`, `deepweak_w2_manifest.csv`, `graph_cache/`, `raw/`, `processed/`
  - `docs/`: `EXPERIMENT_PROTOCOL.md`, `MINIDEEPBUGGER_FINAL_SUMMARY.md`, `TABLE1_FINAL_RESULTS.md`, `POC_V1_FREEZE.txt`, `POC_V1_SHA256SUMS.txt`, `CLONE_POOLED_MLP_PROTOCOL.txt`, `CLONE_POOLED_MLP_SHA256SUMS.txt`, `DEEPWEAK_V1_PROTOCOL.md`, `DEEPWEAK_V1_SHA256SUMS.txt`, `REPRODUCIBILITY_CHECKLIST.md`, `data_schema.md`, `experiment_spec.md`
  - `sandbox/`: `hello.c`, `hello`, `variants/hello_O[0-3]`, `deepweak/weaknesses.c`, `deepweak/w2_weaknesses.c`, `deepweak/weaknesses_O[0-3]`, `deepweak/w2_weaknesses_O[0-3]`, `acfg_builder.py`, `acfg_builder_v4_backup.py`, `build_balanced_variant_pairs.py`, `build_phase2_manifest.py`, `build_variant_dataset.py`, `test_acfg_builder.py`, `test_graph_encoder.py`, `test_optimization_variants.py`, plus legacy demo/eval scripts
  - `results/`: `checkpoints.txt`, `final/final_experiment_summary.csv`, `final/phase2_graph_per_seed.csv`, `final/clone_pooled_mlp_summary.csv`, `final/clone_pooled_mlp_per_seed.csv`, `deepweak_w2_frozen_results.csv`, `phase1/`, `phase2/`, `deepweak/`, `document reports/`
  - `figures/`, `scripts/`, `tests/`, `scratch/`
- Historical pipeline mapping:
  - Binary: `sandbox/variants/hello_O[0-3]` (DeepClone, compiled from `sandbox/hello.c`) and `sandbox/deepweak/weaknesses_O[0-3]` + `w2_weaknesses_O[0-3]` (DeepWeak, compiled from `sandbox/deepweak/weaknesses.c` and `w2_weaknesses.c`)
  - Disassembly / Binary Analysis: `src/acfg_builder.py` -> `disassemble_binary(binary_path)` invoking `otool -tvV <binary_path>` -> `extract_function(disassembly, function_name)` matching `_{function_name}:`
  - Basic Blocks: `src/acfg_builder.py` -> `find_leaders(instructions)` and `build_basic_blocks(instructions, leaders)`
  - CFG: `src/acfg_builder.py` -> `build_cfg(blocks)` returning `nx.DiGraph` (`BB0`, `BB1`, ...)
  - 29-D Semantic A-CFG: `src/acfg_builder.py` -> `categorize_instruction()`, `detect_semantic_predicates()`, `extract_block_features()`, `build_pyg_graph()` returning `torch_geometric.data.Data(x=[num_nodes, 29], edge_index=[2, num_edges])`
  - DeepClone / DeepWeak: DeepClone uses `src/graph_encoder.py` -> `GraphEncoder(input_dim=29, hidden_dim=32, embedding_dim=16)` (2-layer `GCNConv` + ReLU + `global_mean_pool`) and `experiments/evaluate_clone_pooled_mlp_baseline.py` -> `PooledMLPEncoder`. DeepWeak uses `src/deepweak_graph_builder.py` -> `build_deepweak_graph()` (29-D + 10-D from `src/deepweak_features.py` = 39-D) and `src/deepweak_classifier.py` -> `DeepWeakClassifier(input_dim=39, hidden_dim=32, embedding_dim=16)`
  - Training / Inference: `experiments/run_final_experiment.py` -> `train_model()` (`CosineEmbeddingLoss(margin=0.5)`, `Adam(lr=0.01)`, 100 epochs, per-pair step, seeds `[42, 123, 2026, 7, 99]`); `experiments/evaluate_deepweak_w2_frozen.py` -> `train_one_seed()` (`BCEWithLogitsLoss()`, `Adam(lr=0.01)`, 100 epochs, seeds `[42, 123, 2026, 7, 99]`)
  - Evaluation: `experiments/run_final_experiment.py` -> `evaluate_graph_model()` and `evaluate_handcrafted()` (cosine similarity threshold `0.5`); `experiments/evaluate_deepweak_w2_frozen.py` -> `evaluate_w2()` (sigmoid probability threshold `0.5`)
  - Historical APSEC Results: `results/final/final_experiment_summary.csv`, `results/final/phase2_graph_per_seed.csv`, `results/final/clone_pooled_mlp_summary.csv`, `results/deepweak_w2_frozen_results.csv`, `docs/TABLE1_FINAL_RESULTS.md`, `docs/MINIDEEPBUGGER_FINAL_SUMMARY.md`
- 29-D feature status:
  - RESOLVED in `src/acfg_builder.py` (`FEATURE_ORDER`, lines 10–49, length 29; computed per basic block in `extract_block_features` as raw integer counts cast to `torch.float32` with no normalization; shared directly by `src/deepweak_graph_builder.py`; duplicated identically in `sandbox/acfg_builder.py`, while `sandbox/acfg_builder_v4_backup.py` has an older 28-D list without `logical_shift_right`).
- DeepClone status:
  - Confirmed in `src/graph_encoder.py`, `config/experiment.json`, and `experiments/run_final_experiment.py`: `GraphEncoder` (`29 -> 32 -> 16`, 2 `GCNConv` layers, no residual connections, `global_mean_pool`), `CosineEmbeddingLoss(margin=0.5)`, `Adam(lr=0.01)`, 100 epochs, seeds `[42, 123, 2026, 7, 99]`, cosine threshold `0.5`, trained on 120 `train` pairs from `data/variant_pair_manifest.csv` (10 families), evaluated on 120 balanced pairs from `data/phase2_independent_manifest.csv` (10 unseen families). No `.pt` model checkpoints are saved by `run_final_experiment.py`.
- DeepWeak status:
  - Confirmed in `src/deepweak_features.py`, `src/deepweak_graph_builder.py`, `src/deepweak_classifier.py`, and `experiments/evaluate_deepweak_w2_frozen.py`: extends the frozen 29-D features with 10 security features (`SECURITY_FEATURE_ORDER`) to form 39-D node features; `DeepWeakClassifier` (`39 -> 32 -> 16 -> 1`); trained on W1 (`data/deepweak_w1_split_v2.csv`, 56 train / 16 dev) and evaluated on W2 (`data/deepweak_w2_manifest.csv`, 48 unseen samples). Separated from the main provenance-extension path.
- Dataset/manifests status:
  - Tracked CSV manifests in `data/` (`variant_pair_manifest.csv`, `phase2_independent_manifest.csv`, `deepweak_manifest.csv`, `deepweak_w1_split.csv`, `deepweak_w1_split_v2.csv`, `deepweak_w2_manifest.csv`) and tracked C sources (`sandbox/hello.c`, `sandbox/deepweak/weaknesses.c`, `sandbox/deepweak/w2_weaknesses.c`). Compiled ARM64 Mach-O binaries exist on disk in `sandbox/variants/hello_O[0-3]` and `sandbox/deepweak/*_O[0-3]` but are gitignored. Untracked `data/graph_cache/` contains 2,640 `.pt` files from another branch (`exp/asiaccs-v2`).
- Environment status:
  - `requirements.txt` and `requirements-frozen.txt` are identical (`torch==2.13.0`, `torch-geometric==2.8.0.post1`, `networkx==3.6.1`, `numpy==2.5.2`, `scikit-learn==1.9.0`, `pandas==3.0.5`, `matplotlib==3.11.1`). External tool dependency: macOS `otool -tvV` and `clang` (ARM64 Mach-O). No Ghidra or FAISS dependency exists on the current branch.
- Baseline-critical candidates:
  - `src/acfg_builder.py`, `src/graph_encoder.py`, `config/experiment.json`, `experiments/run_final_experiment.py`, `experiments/evaluate_clone_pooled_mlp_baseline.py`, `data/variant_pair_manifest.csv`, `data/phase2_independent_manifest.csv`, `sandbox/hello.c`, `sandbox/variants/hello_O[0-3]`, `results/final/*`, `results/phase1/*`, `results/phase2/*`, `docs/EXPERIMENT_PROTOCOL.md`, `docs/POC_V1_FREEZE.txt`, `docs/POC_V1_SHA256SUMS.txt`, `docs/CLONE_POOLED_MLP_PROTOCOL.txt`, `docs/CLONE_POOLED_MLP_SHA256SUMS.txt`, `docs/TABLE1_FINAL_RESULTS.md`, `docs/MINIDEEPBUGGER_FINAL_SUMMARY.md`, `requirements.txt`, `requirements-frozen.txt` (plus DeepWeak frozen files `src/deepweak_*.py`, `experiments/evaluate_deepweak_w2_frozen.py`, `data/deepweak_*.csv`, `sandbox/deepweak/*`).
- Reusable candidates:
  - `src/acfg_builder.py` (specifically the 29-D `FEATURE_ORDER`, `categorize_instruction`, `detect_semantic_predicates`, `extract_block_features`, and Mach-O ARM64 `otool` regression path), `src/graph_encoder.py` (as baseline reference), `experiments/evaluate_clone_pooled_mlp_baseline.py` (Pooled-MLP baseline reference).
- Generated/experimental artifacts:
  - Gitignored local binaries (`sandbox/hello`, `sandbox/variants/hello_O[0-3]`, `sandbox/deepweak/weaknesses_O[0-3]`, `sandbox/deepweak/w2_weaknesses_O[0-3]`); untracked `data/graph_cache/*.pt` (2,640 files) and orphan `__pycache__` directories (`src/adapters`, `src/evaluation`, `src/features`, `src/models`, `src/preprocessing`, `src/program_similarity`, `src/retrieval`, `scripts/dataset_v2`, `tests`) left over from other branches; tracked CSV/figure outputs in `results/` and `figures/`.
- Risks/ambiguities:
  - Duplicate feature extractor in `sandbox/acfg_builder.py` (29-D) and `sandbox/acfg_builder_v4_backup.py` (28-D); duplicate manifests in `results/variant_pair_manifest.csv` and `results/phase2_independent_manifest.csv`; gitignored baseline binaries (`sandbox/variants/hello_O[0-3]`) required by `run_final_experiment.py`; non-stripped symbol requirement (`_{function_name}:`) in `src/acfg_builder.py`; 2 SHA-256 mismatches in `docs/POC_V1_SHA256SUMS.txt` (`README.md`) and `docs/DEEPWEAK_V1_SHA256SUMS.txt` (`docs/DEEPWEAK_V1_PROTOCOL.md`); untracked `data/graph_cache/` and orphan `__pycache__` directories from other branches (`exp/asiaccs-v2`, etc.); outdated early planning docs (`docs/data_schema.md`, `docs/experiment_spec.md`) that do not match PoC v1.
- Unresolved items:
  - Whether any saved `.pt` model checkpoints for Mini-DeepClone PoC v1 were ever meant to be stored (currently `results/checkpoints/` is empty and `run_final_experiment.py` retrains from scratch across the 5 fixed seeds).
  - Why `README.md` and `docs/DEEPWEAK_V1_PROTOCOL.md` differ from their recorded hashes in `docs/POC_V1_SHA256SUMS.txt` and `docs/DEEPWEAK_V1_SHA256SUMS.txt`.
  - How untracked leftover directories from other branches (`data/graph_cache/`, empty `src/*/` and `tests/` subdirectories containing only `__pycache__`, empty `configs/experiment.yaml`) should be handled during Phase 0 baseline freeze.
- Commands executed:
  - `git status`, `git branch --show-current`, `git log --oneline --decorate -5`, `git ls-files`, `ls -la`, `find src scripts tests -name "*.pyc"`, `shasum -a 256 -c docs/POC_V1_SHA256SUMS.txt docs/CLONE_POOLED_MLP_SHA256SUMS.txt docs/DEEPWEAK_V1_SHA256SUMS.txt`, `git log --stat --all --oneline`, `git branch -a`, `git tag -l`, `git show 04baedd --stat`
- Verification performed:
  - Ran `git branch --show-current`, `git status`, and `git diff` to verify that `deepbugger-extension` is the current branch, `main` is untouched, no research code/dataset/config/checkpoint/result files were modified, and only `PROJECT_PROGRESS.md` changed.
- Result: Full read-only repository audit completed and documented.
- Decisions made: None (read-only audit; no implementation decisions made).
- Open questions: None blocking STEP-001 completion.
- Git commit:
- Next proposed micro-project: STEP-002 — Verify and document the exact 29-D semantic A-CFG feature schema and baseline SHA-256 integrity (Phase 0 preparation before baseline reproduction and freeze).



