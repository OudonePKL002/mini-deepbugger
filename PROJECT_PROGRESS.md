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
- Git commit: f732b4f
- Next proposed micro-project: STEP-002 — Verify and document the exact 29-D semantic A-CFG feature schema and baseline SHA-256 integrity (Phase 0 preparation before baseline reproduction and freeze).

---

### STEP-002 — Baseline reproduction preflight

- Date: 2026-10-23
- Status: DONE
- Goal: Perform a read-only / non-training preflight to record the exact local machine/toolchain environment, fingerprint the historical baseline binaries, verify tracked-file SHA-256 integrity, confirm DeepClone inputs and historical result references, and determine reproduction readiness.
- Master Plan section: Section 37 (Phase 0 — Baseline Reproduction and Freeze), Section 41 (Rule 1 — Baseline Freeze), Section 43 (Immediate Next Actions 1–3)
- Branch: deepbugger-extension
- Platform:
  - `uname -a`: `Darwin MacBook-Air.local 25.6.0 Darwin Kernel Version 25.6.0: Fri Jul 31 19:17:12 PDT 2026; root:xnu-12377.161.14~5/RELEASE_ARM64_T8103 arm64`
  - `sw_vers`: `macOS 26.6.2 (Build 25G83)`
  - `git --version`: `git version 2.53.0`
- Python:
  - System default (`which python3`): `/opt/homebrew/bin/python3` (`Python 3.14.3`, `pip 26.0`, no project packages installed)
  - Project virtual environment (`.venv`): Python `3.12.12` (`.venv/bin/python3 -> /Users/admin/.pyenv/versions/3.12.12/bin/python3`, recorded in `.venv/pyvenv.cfg`, matching `README.md` Section 4)
- Compiler:
  - `/usr/bin/clang`: `Apple clang version 21.0.0 (clang-2100.1.1.101)`, `Target: arm64-apple-darwin25.6.0` (matches `README.md` Section 4)
- otool:
  - `/usr/bin/otool`: `llvm-otool(1): Apple Inc. version cctools-1040`, `disassembler: LLVM version 21.0.0`
- Required dependency versions:
  - `requirements.txt` and `requirements-frozen.txt` (identical): `torch==2.13.0`, `torch-geometric==2.8.0.post1`, `numpy==2.5.2`, `pandas==3.0.5`, `scikit-learn==1.9.0`, `networkx==3.6.1`, `matplotlib==3.11.1`
- Installed dependency versions:
  - System `/opt/homebrew/bin/python3` (`3.14.3`): `NOT INSTALLED` for all 7 packages
  - Project `.venv` (`3.12.12`, inspected via `.venv/lib/python3.12/site-packages/*.dist-info`): `torch 2.13.0` (`MATCH`), `torch-geometric 2.8.0.post1` (`MATCH`), `numpy 2.5.2` (`MATCH`), `pandas 3.0.5` (`MATCH`), `scikit-learn 1.9.0` (`MATCH`), `networkx 3.6.1` (`MATCH`), `matplotlib 3.11.1` (`MATCH`)
- Historical binary inventory:
  - `sandbox/variants/hello_O0`: exists (`YES`), `Mach-O 64-bit executable arm64`, `34,888` bytes, modified `Aug 31 15:49:16 2026`
  - `sandbox/variants/hello_O1`: exists (`YES`), `Mach-O 64-bit executable arm64`, `34,888` bytes, modified `Aug 31 15:49:16 2026`
  - `sandbox/variants/hello_O2`: exists (`YES`), `Mach-O 64-bit executable arm64`, `34,888` bytes, modified `Aug 31 15:49:16 2026`
  - `sandbox/variants/hello_O3`: exists (`YES`), `Mach-O 64-bit executable arm64`, `34,888` bytes, modified `Aug 31 15:49:16 2026`
  - Verified with `otool -tvV sandbox/variants/hello_O0` that all 31 expected function symbols (`_add:` through `_sum_three:`) plus `_main:` are present and readable by `extract_function`.
- Historical binary SHA-256:
  - `sandbox/variants/hello_O0`: `b03e786e94664b48af30e951ffee1c74dc7b8632a40d3e9ac6f12f3edf99784c`
  - `sandbox/variants/hello_O1`: `42f4f57aa035e0d8ab3d4f9205c450272a7b49e0c30f549917c977192ac2ba57`
  - `sandbox/variants/hello_O2`: `b563d76883a532b15a4ee7ea142b66cf70a1ce8b823d2f758f6b4f36901925c0`
  - `sandbox/variants/hello_O3`: `eea484872eb13a00aaae13a0d3ebcc4b4f3dadf5ce40ac82bcd46b366df0c18d`
- Checksum verification:
  - `CODE`: 7/7 OK (`src/acfg_builder.py`, `src/graph_encoder.py`, `experiments/run_final_experiment.py`, `experiments/evaluate_clone_pooled_mlp_baseline.py`, `src/deepweak_features.py`, `src/deepweak_graph_builder.py`, `src/deepweak_classifier.py`, `experiments/evaluate_deepweak_w2_frozen.py` — 8/8 code files OK)
  - `CONFIG`: 2/2 OK (`config/experiment.json`, `requirements.txt`)
  - `MANIFEST`: 4/4 OK (`data/variant_pair_manifest.csv`, `data/phase2_independent_manifest.csv`, `data/deepweak_w1_split_v2.csv`, `data/deepweak_w2_manifest.csv`)
  - `RESULT`: 3/3 OK (`results/final/clone_pooled_mlp_per_seed.csv`, `results/final/clone_pooled_mlp_summary.csv`, `results/deepweak_w2_frozen_results.csv`; also recorded current SHA-256 of `results/final/final_experiment_summary.csv` = `58b3b18e5157a28024d4f4cceaf2c327fba71b33c6c2fe21b50c0e915baa53af` and `results/final/phase2_graph_per_seed.csv` = `b640849d4a887f9e1de10a3ec72ddda96a21cbb653311483836ef846ece984d6`)
  - `DOCUMENTATION`: 3/5 OK (`docs/EXPERIMENT_PROTOCOL.md`, `docs/REPRODUCIBILITY_CHECKLIST.md`, `docs/CLONE_POOLED_MLP_PROTOCOL.txt`), 2/5 MISMATCH (`README.md`, `docs/DEEPWEAK_V1_PROTOCOL.md`)
- Documentation-only checksum drift:
  - `README.md`: recorded `644111307911a4b4bf21db2ab29519a761db7682a2b2f8fb39a1783e8cdadbfb` in `docs/POC_V1_SHA256SUMS.txt` vs current `b825fbb474893f6afb8742c5f87754ea26c1517e7b679032930757da6bbd5797`
  - `docs/DEEPWEAK_V1_PROTOCOL.md`: recorded `25498ef0196ecaf8eb29cf250ae4978c34bfd608fee179f656a04d61f2823082` in `docs/DEEPWEAK_V1_SHA256SUMS.txt` vs current `a185d563753d088c676f93ae24ac158866fac2d6920b35d9b107d9ed192f33af`
- Critical input integrity:
  - All 8 DeepClone critical files (`config/experiment.json`, `src/acfg_builder.py`, `src/graph_encoder.py`, `experiments/run_final_experiment.py`, `experiments/evaluate_clone_pooled_mlp_baseline.py`, `data/variant_pair_manifest.csv`, `data/phase2_independent_manifest.csv`, `sandbox/hello.c`) exist and are readable.
  - `data/variant_pair_manifest.csv`: 156 data rows (120 `train` rows: 60 pos / 60 neg; 36 `test` rows: 18 pos / 18 neg).
  - `data/phase2_independent_manifest.csv`: 120 data rows (60 positive, 60 negative).
- Historical result references:
  - DeepClone GraphEncoder (`results/final/final_experiment_summary.csv` & `phase2_graph_per_seed.csv`): F1 = `0.775883293606493 ± 0.004545179401549354` (`0.7759 ± 0.0045`), Accuracy = `0.7516666666666667 ± 0.0037267799624996364` (`0.7517 ± 0.0037`), Precision = `0.7071235102925244 ± 0.009341224207796935` (`0.7071 ± 0.0093`), Recall = `0.86 ± 0.022360679774997918` (`0.8600 ± 0.0224`), Positive mean = `0.8542568600177765 ± 0.02591326212407821` (`0.8543 ± 0.0259`), Negative mean = `0.3787666478927713 ± 0.023401277394816317` (`0.3788 ± 0.0234`), Separation = `0.4754902121250052 ± 0.03182988040264492` (`0.4755 ± 0.0318`).
  - Handcrafted A-CFG Baseline (`results/final/final_experiment_summary.csv`): F1 = `0.5309734513274337` (`0.5310`), Accuracy = `0.5583333333333333` (`0.5583`), Precision = `0.5660377358490566` (`0.5660`), Recall = `0.5` (`0.5000`), Positive mean = `0.6462890081107616`, Negative mean = `0.45258812978863716`, Separation = `0.19370087832212446` (`0.1937`).
  - Pooled-MLP Baseline (`results/final/clone_pooled_mlp_summary.csv` & `clone_pooled_mlp_per_seed.csv`): F1 = `0.7531376990780363 ± 0.030897680657538562` (`0.7531 ± 0.0309`), Accuracy = `0.7033333333333334 ± 0.030390970881350794` (`0.7033 ± 0.0304`), Precision = `0.6460757939750379 ± 0.029492084696913733` (`0.6461 ± 0.0295`), Recall = `0.9099999999999999 ± 0.08944271909999157` (`0.9100 ± 0.0894`), Positive mean = `0.8957507936656475 ± 0.06627019148397004`, Negative mean = `0.5300020549855738 ± 0.08843542158389567`, Separation = `0.3657487386800736 ± 0.033130125271054696` (`0.3657 ± 0.0331`).
- Reproduction readiness:
  - `READY` (using `.venv/bin/python` which has Python 3.12.12 and exact matching package versions, Apple Clang 21.0.0, `otool` LLVM 21.0.0, all 4 historical binaries intact, and 100% matching code/config/manifest/result SHA-256 checksums).
- Risks:
  - System default `python3` (`/opt/homebrew/bin/python3`, Python 3.14.3) does not have project dependencies installed; reproduction commands must explicitly invoke `.venv/bin/python` (which symlinks to `/Users/admin/.pyenv/versions/3.12.12/bin/python3`).
  - `experiments/run_final_experiment.py` and `experiments/evaluate_clone_pooled_mlp_baseline.py` write directly to `results/final/*.csv`, so when reproduction is executed in the next step, `git diff` / `shasum -a 256` on `results/final/*.csv` will directly verify whether the reproduced outputs are byte-for-byte identical to the tracked historical results.
- Unresolved items:
  - None blocking controlled reproduction.
- Commands executed:
  - `git branch --show-current`, `git status`, `git log --oneline --decorate -5`, `uname -a`, `sw_vers`, `python3 --version`, `which python3`, `clang --version`, `which clang`, `otool --version`, `which otool`, `git --version`, `python3 -m pip --version`, `python3 -m pip show ...`, `ls -la .venv/lib/python*/site-packages/*.dist-info`, `ls -la .venv/bin/python*`, `ls -lhT sandbox/variants/hello_O[0-3]`, `file sandbox/variants/hello_O[0-3]`, `shasum -a 256 sandbox/variants/hello_O[0-3]`, `otool -tvV sandbox/variants/hello_O0`, `stat -f "%N %z %Sm" sandbox/variants/hello_O[0-3]`, `wc -l data/*.csv`, `shasum -a 256 README.md docs/DEEPWEAK_V1_PROTOCOL.md`, `shasum -a 256 results/final/*.csv`, plus read-only Python CSV row/label count verification.
- Verification performed:
  - Confirmed `deepbugger-extension` is current branch, `main` is untouched at `194999e`, no binaries/code/datasets/manifests/configs/results/checksums were modified, and only `PROJECT_PROGRESS.md` changed.
- Result: Preflight complete; repository and `.venv` environment are `READY` for controlled baseline reproduction.
- Decisions made:
  - Recorded SHA-256 fingerprints of the 4 gitignored historical binaries (`hello_O0`..`hello_O3`) without rebuilding them.
  - Recorded the 2 documentation-only checksum mismatches (`README.md`, `docs/DEEPWEAK_V1_PROTOCOL.md`) as historical documentation drift without regenerating checksum files.
- Open questions: None
- Git commit: 2c5cf04
- Next proposed micro-project: STEP-003 — Reproduce the historical Mini-DeepBugger DeepClone GraphEncoder baseline only.

---

### STEP-003 — Reproduce DeepClone GraphEncoder baseline

- Date: 2026-10-23
- Status: DONE
- Goal: Reproduce historical Mini-DeepBugger DeepClone GraphEncoder baseline using the frozen repository state, existing historical binaries, and exact project virtual environment.
- Master Plan section: Section 37 (Phase 0 — Baseline Reproduction and Freeze)
- Branch: deepbugger-extension
- Reproduction workspace: Isolated temporary workspace (`/tmp/step003_repro`)
- Python environment: Python 3.12.12 (`.venv/bin/python`), torch 2.13.0, torch-geometric 2.8.0.post1, numpy 2.5.2, pandas 3.0.5, scikit-learn 1.9.0, networkx 3.6.1, matplotlib 3.11.1
- Historical binary hashes verified:
  - `sandbox/variants/hello_O0`: `b03e786e94664b48af30e951ffee1c74dc7b8632a40d3e9ac6f12f3edf99784c` (EXACT MATCH)
  - `sandbox/variants/hello_O1`: `42f4f57aa035e0d8ab3d4f9205c450272a7b49e0c30f549917c977192ac2ba57` (EXACT MATCH)
  - `sandbox/variants/hello_O2`: `b563d76883a532b15a4ee7ea142b66cf70a1ce8b823d2f758f6b4f36901925c0` (EXACT MATCH)
  - `sandbox/variants/hello_O3`: `eea484872eb13a00aaae13a0d3ebcc4b4f3dadf5ce40ac82bcd46b366df0c18d` (EXACT MATCH)
- Historical result hashes before run:
  - `results/final/final_experiment_summary.csv`: `58b3b18e5157a28024d4f4cceaf2c327fba71b33c6c2fe21b50c0e915baa53af`
  - `results/final/phase2_graph_per_seed.csv`: `b640849d4a887f9e1de10a3ec72ddda96a21cbb653311483836ef846ece984d6`
- Experiment command: `cd /tmp/step003_repro && /Users/admin/Desktop/Mini-DeepBugger/.venv/bin/python experiments/run_final_experiment.py`
- Seeds: `[42, 123, 2026, 7, 99]`
- Execution note:
  The first STEP-003 attempt was interrupted by Antigravity quota exhaustion.
  The task was resumed only after inspecting repository and temporary-workspace state.
- Reproduced GraphEncoder metrics:
  - F1: `0.775883293606493 ± 0.004545179401549354` (`0.7759 ± 0.0045`)
  - Accuracy: `0.7516666666666667 ± 0.0037267799624996364` (`0.7517 ± 0.0037`)
  - Precision: `0.7071235102925244 ± 0.009341224207796935` (`0.7071 ± 0.0093`)
  - Recall: `0.86 ± 0.022360679774997918` (`0.8600 ± 0.0224`)
  - Positive Mean: `0.8542568600177765 ± 0.02591326212407821` (`0.8543 ± 0.0259`)
  - Negative Mean: `0.3787666478927713 ± 0.023401277394816317` (`0.3788 ± 0.0234`)
  - Separation: `0.4754902121250052 ± 0.03182988040264492` (`0.4755 ± 0.0318`)
- Per-seed GraphEncoder metrics:
  - Seed 42: acc=`0.7500`, prec=`0.7083`, rec=`0.8500`, f1=`0.7727`, pos_mean=`0.862666`, neg_mean=`0.409312`, sep=`0.453354`, tp=51, fp=21, fn=9, tn=39
  - Seed 123: acc=`0.7500`, prec=`0.7083`, rec=`0.8500`, f1=`0.7727`, pos_mean=`0.832329`, neg_mean=`0.351242`, sep=`0.481087`, tp=51, fp=21, fn=9, tn=39
  - Seed 2026: acc=`0.7500`, prec=`0.7083`, rec=`0.8500`, f1=`0.7727`, pos_mean=`0.826242`, neg_mean=`0.392550`, sep=`0.433692`, tp=51, fp=21, fn=9, tn=39
  - Seed 7: acc=`0.7500`, prec=`0.6923`, rec=`0.9000`, f1=`0.7826`, pos_mean=`0.890629`, neg_mean=`0.379549`, sep=`0.511080`, tp=54, fp=24, fn=6, tn=36
  - Seed 99: acc=`0.7583`, prec=`0.7183`, rec=`0.8500`, f1=`0.7786`, pos_mean=`0.859419`, neg_mean=`0.361180`, sep=`0.498238`, tp=51, fp=20, fn=9, tn=40
- Reproduced Handcrafted metrics:
  - F1: `0.5309734513274337` (`0.5310`)
  - Accuracy: `0.5583333333333333` (`0.5583`)
  - Precision: `0.5660377358490566` (`0.5660`)
  - Recall: `0.5` (`0.5000`)
  - Positive Mean: `0.6462890081107616` (`0.6463`)
  - Negative Mean: `0.45258812978863716` (`0.4526`)
  - Separation: `0.19370087832212446` (`0.1937`)
- Historical comparison:
  - GraphEncoder F1: historical `0.775883293606493`, reproduced `0.775883293606493`, diff = `0.0` (EXACT)
  - GraphEncoder Accuracy: historical `0.7516666666666667`, reproduced `0.7516666666666667`, diff = `0.0` (EXACT)
  - GraphEncoder Precision: historical `0.7071235102925244`, reproduced `0.7071235102925244`, diff = `0.0` (EXACT)
  - GraphEncoder Recall: historical `0.86`, reproduced `0.86`, diff = `0.0` (EXACT)
  - GraphEncoder Positive Mean: historical `0.8542568600177765`, reproduced `0.8542568600177765`, diff = `0.0` (EXACT)
  - GraphEncoder Negative Mean: historical `0.3787666478927713`, reproduced `0.3787666478927713`, diff = `0.0` (EXACT)
  - GraphEncoder Separation: historical `0.4754902121250052`, reproduced `0.4754902121250052`, diff = `0.0` (EXACT)
  - Handcrafted F1: historical `0.5309734513274337`, reproduced `0.5309734513274337`, diff = `0.0` (EXACT)
  - Handcrafted Accuracy: historical `0.5583333333333333`, reproduced `0.5583333333333333`, diff = `0.0` (EXACT)
  - Handcrafted Precision: historical `0.5660377358490566`, reproduced `0.5660377358490566`, diff = `0.0` (EXACT)
  - Handcrafted Recall: historical `0.5`, reproduced `0.5`, diff = `0.0` (EXACT)
  - Handcrafted Positive Mean: historical `0.6462890081107616`, reproduced `0.6462890081107616`, diff = `0.0` (EXACT)
  - Handcrafted Negative Mean: historical `0.45258812978863716`, reproduced `0.45258812978863716`, diff = `0.0` (EXACT)
  - Handcrafted Separation: historical `0.19370087832212446`, reproduced `0.19370087832212446`, diff = `0.0` (EXACT)
- Reproduced result hashes:
  - `results/final/final_experiment_summary.csv`: `58b3b18e5157a28024d4f4cceaf2c327fba71b33c6c2fe21b50c0e915baa53af` (BYTE-IDENTICAL)
  - `results/final/phase2_graph_per_seed.csv`: `b640849d4a887f9e1de10a3ec72ddda96a21cbb653311483836ef846ece984d6` (BYTE-IDENTICAL)
- Reproduction classification: EXACT REPRODUCTION
- Primary baseline files unchanged: YES. SHA-256 verified byte-for-byte identical; working tree clean.
- Risks/ambiguities: None. Baseline DeepClone GraphEncoder reproduction is 100% deterministic and byte-identical across all seeds and summary metrics.
- Commands executed:
  - `git branch --show-current`, `git status`, `git log --oneline --decorate -5`
  - `shasum -a 256 results/final/final_experiment_summary.csv results/final/phase2_graph_per_seed.csv`
  - Preparation and verification of `/tmp/step003_repro`
  - `cd /tmp/step003_repro && /Users/admin/Desktop/Mini-DeepBugger/.venv/bin/python experiments/run_final_experiment.py`
  - `shasum -a 256 /tmp/step003_repro/results/final/final_experiment_summary.csv /tmp/step003_repro/results/final/phase2_graph_per_seed.csv`
  - `rm -rf /tmp/step003_repro`
- Verification performed:
  - Verified 100% metric match with 0.0 numerical difference across all summary metrics, distributions, and per-seed rows.
  - Verified reproduced CSV hashes match historical tracked CSV hashes byte-for-byte (`BYTE-IDENTICAL`).
  - Verified primary working tree and tracked baseline results remained completely unchanged.
- Result: EXACT REPRODUCTION achieved.
- Decisions made:
  - Executed reproduction strictly in an isolated reproduction workspace (`/tmp/step003_repro`) to protect primary repository baseline files from accidental overwrite.
  - Cleaned up temporary workspace after confirming byte-identical output.
- Open questions: None.
- Git commit: 20d983e
- Next proposed micro-project: STEP-004 — Reproduce and verify the historical Mini-DeepBugger Pooled-Node MLP baseline only.

---

### STEP-004 — Reproduce Pooled-Node MLP baseline

- Date: 2026-10-23
- Status: DONE
- Goal: Reproduce historical Pooled-Node MLP ablation baseline using the exact frozen Mini-DeepBugger inputs and project environment in an isolated workspace.
- Master Plan section: Section 37 (Phase 0 — Baseline Reproduction and Freeze)
- Branch: deepbugger-extension
- Reproduction workspace: Isolated temporary workspace (`/tmp/step004_repro`)
- Python environment: Python 3.12.12 (`.venv/bin/python`), torch 2.13.0, torch-geometric 2.8.0.post1, numpy 2.5.2, pandas 3.0.5, scikit-learn 1.9.0, networkx 3.6.1, matplotlib 3.11.1
- Historical result hashes before run:
  - `results/final/clone_pooled_mlp_summary.csv`: `ad9c89402a7ce7e33e52fec88470756319ec65f10322a3b383d1c077fec29591`
  - `results/final/clone_pooled_mlp_per_seed.csv`: `2492f5eef647d23e04dde0b020fe403fc700003782dd2b9add72045fbb7010ca`
- Experiment script: `experiments/evaluate_clone_pooled_mlp_baseline.py`
- Experiment configuration:
  - Architecture: `PooledMLPEncoder` (29-D node mean-pooling $\to$ Linear(29, 32) $\to$ ReLU $\to$ Linear(32, 16) $\to$ ReLU)
  - Loss: `CosineEmbeddingLoss(margin=0.5)`
  - Optimizer: `Adam(lr=0.01)`
  - Epochs: 100
  - Threshold: 0.5
  - Graph samples: 80 cached Phase-1/Phase-2 A-CFGs
  - Evaluation pairs: 120 balanced Phase-2 pairs (60 positive / 60 negative, 0 family overlap with training)
- Seeds: `[42, 123, 2026, 7, 99]`
- Reproduced metrics:
  - Pooled-MLP Accuracy: `0.7033333333333334 ± 0.030390970881350794` (`0.7033 ± 0.0304`)
  - Pooled-MLP Precision: `0.6460757939750379 ± 0.029492084696913733` (`0.6461 ± 0.0295`)
  - Pooled-MLP Recall: `0.9099999999999999 ± 0.08944271909999157` (`0.9100 ± 0.0894`)
  - Pooled-MLP F1: `0.7531376990780363 ± 0.030897680657538562` (`0.7531 ± 0.0309`)
  - Pooled-MLP Positive Mean: `0.8957507936656475 ± 0.06627019148397004` (`0.8958 ± 0.0663`)
  - Pooled-MLP Negative Mean: `0.5300020549855738 ± 0.08843542158389567` (`0.5300 ± 0.0884`)
  - Pooled-MLP Separation: `0.3657487386800736 ± 0.033130125271054696` (`0.3657 ± 0.0331`)
  - Handcrafted Accuracy: `0.5583333333333333` (`0.5583`)
  - Handcrafted Precision: `0.5660377358490566` (`0.5660`)
  - Handcrafted Recall: `0.5` (`0.5000`)
  - Handcrafted F1: `0.5309734513274337` (`0.5310`)
  - Handcrafted Positive Mean: `0.6462890081107616` (`0.6463`)
  - Handcrafted Negative Mean: `0.45258812978863716` (`0.4526`)
  - Handcrafted Separation: `0.19370087832212446` (`0.1937`)
- Per-seed results:
  - Seed 42: acc=`0.6667`, prec=`0.6064`, rec=`0.9500`, f1=`0.7403`, pos_mean=`0.911466`, neg_mean=`0.560013`, sep=`0.351453`, tp=57, fp=37, fn=3, tn=23
  - Seed 123: acc=`0.7000`, prec=`0.6333`, rec=`0.9500`, f1=`0.7600`, pos_mean=`0.923041`, neg_mean=`0.576318`, sep=`0.346724`, tp=57, fp=33, fn=3, tn=27
  - Seed 2026: acc=`0.7500`, prec=`0.6786`, rec=`0.9500`, f1=`0.7917`, pos_mean=`0.942391`, neg_mean=`0.544390`, sep=`0.398001`, tp=57, fp=27, fn=3, tn=33
  - Seed 7: acc=`0.7083`, prec=`0.6404`, rec=`0.9500`, f1=`0.7651`, pos_mean=`0.922975`, neg_mean=`0.593997`, sep=`0.328978`, tp=57, fp=32, fn=3, tn=28
  - Seed 99: acc=`0.6917`, prec=`0.6716`, rec=`0.7500`, f1=`0.7087`, pos_mean=`0.778880`, neg_mean=`0.375292`, sep=`0.403588`, tp=45, fp=22, fn=15, tn=38
- Historical comparison:
  - Pooled-MLP F1: historical `0.7531376990780363`, reproduced `0.7531376990780363`, diff = `0.0` (EXACT)
  - Pooled-MLP Accuracy: historical `0.7033333333333334`, reproduced `0.7033333333333334`, diff = `0.0` (EXACT)
  - Pooled-MLP Precision: historical `0.6460757939750379`, reproduced `0.6460757939750379`, diff = `0.0` (EXACT)
  - Pooled-MLP Recall: historical `0.9099999999999999`, reproduced `0.9099999999999999`, diff = `0.0` (EXACT)
  - Pooled-MLP Positive Mean: historical `0.8957507936656475`, reproduced `0.8957507936656475`, diff = `0.0` (EXACT)
  - Pooled-MLP Negative Mean: historical `0.5300020549855738`, reproduced `0.5300020549855738`, diff = `0.0` (EXACT)
  - Pooled-MLP Separation: historical `0.3657487386800736`, reproduced `0.3657487386800736`, diff = `0.0` (EXACT)
  - Handcrafted F1: historical `0.5309734513274337`, reproduced `0.5309734513274337`, diff = `0.0` (EXACT)
  - Handcrafted Accuracy: historical `0.5583333333333333`, reproduced `0.5583333333333333`, diff = `0.0` (EXACT)
  - Handcrafted Precision: historical `0.5660377358490566`, reproduced `0.5660377358490566`, diff = `0.0` (EXACT)
  - Handcrafted Recall: historical `0.5`, reproduced `0.5`, diff = `0.0` (EXACT)
  - Handcrafted Positive Mean: historical `0.6462890081107616`, reproduced `0.6462890081107616`, diff = `0.0` (EXACT)
  - Handcrafted Negative Mean: historical `0.45258812978863716`, reproduced `0.45258812978863716`, diff = `0.0` (EXACT)
  - Handcrafted Separation: historical `0.19370087832212446`, reproduced `0.19370087832212446`, diff = `0.0` (EXACT)
- Reproduced result hashes:
  - `results/final/clone_pooled_mlp_summary.csv`: `ad9c89402a7ce7e33e52fec88470756319ec65f10322a3b383d1c077fec29591` (BYTE-IDENTICAL)
  - `results/final/clone_pooled_mlp_per_seed.csv`: `2492f5eef647d23e04dde0b020fe403fc700003782dd2b9add72045fbb7010ca` (BYTE-IDENTICAL)
- Reproduction classification: EXACT REPRODUCTION
- Primary baseline files unchanged: YES. All historical result files verified byte-for-byte identical; working tree clean.
- Risks/ambiguities: None. Baseline Pooled-Node MLP reproduction is 100% deterministic and byte-identical across all seeds and summary metrics.
- Commands executed:
  - `git branch --show-current`, `git status`, `git log --oneline --decorate -6`, `git rev-parse main`
  - `shasum -a 256 results/final/clone_pooled_mlp_summary.csv results/final/clone_pooled_mlp_per_seed.csv`
  - Preparation and verification of `/tmp/step004_repro`
  - `cd /tmp/step004_repro && /Users/admin/Desktop/Mini-DeepBugger/.venv/bin/python experiments/evaluate_clone_pooled_mlp_baseline.py`
  - `shasum -a 256 /tmp/step004_repro/results/final/clone_pooled_mlp_summary.csv /tmp/step004_repro/results/final/clone_pooled_mlp_per_seed.csv`
  - `rm -rf /tmp/step004_repro`
- Verification performed:
  - Verified 100% metric match with 0.0 numerical difference across all summary metrics, distributions, and per-seed rows.
  - Verified reproduced CSV hashes match historical tracked CSV hashes byte-for-byte (`BYTE-IDENTICAL`).
  - Verified primary working tree and tracked baseline results remained completely unchanged.
- Result: EXACT REPRODUCTION achieved.
- Decisions made:
  - Executed reproduction strictly in an isolated reproduction workspace (`/tmp/step004_repro`) to protect primary repository baseline files from accidental overwrite.
  - Cleaned up temporary workspace after confirming byte-identical output.
- Open questions: None.
- Git commit: c9e5ed9
- Next proposed micro-project: STEP-005 — Freeze the verified Mini-DeepBugger baseline.

---

### STEP-005 — Freeze Mini-DeepBugger baseline

- Date: 2026-10-23
- Status: DONE
- Goal: Create an immutable, auditable baseline-freeze record for the historical Mini-DeepBugger/APSEC baseline that has now been successfully reproduced.
- Master Plan section: Section 37 (Phase 0 — Baseline Reproduction and Freeze)
- Branch: deepbugger-extension
- Baseline branch: main
- Baseline commit: 194999ea1b692b771702484b3fd471386a75c81b
- Freeze documentation: `frozen_baseline/BASELINE_FREEZE.md`
- Freeze checksum manifest: `frozen_baseline/BASELINE_SHA256SUMS.txt`
- Frozen 29-D schema: `src/acfg_builder.py` (`compute_node_features`: 10 instruction categories, 5 register counts, 2 operand/immediate, 12 CFG/centrality topological features)
- Frozen manifests: `data/variant_pair_manifest.csv` (156 pairs, 120 train), `data/phase2_independent_manifest.csv` (120 pairs: 60 pos, 60 neg, 0 family overlap with training)
- Frozen configuration: `config/experiment.json` (input=29, hidden=32, emb=16, margin=0.5, lr=0.01, epochs=100, threshold=0.5, seeds=[42, 123, 2026, 7, 99])
- Frozen historical binaries: `sandbox/variants/hello_O0`..`hello_O3` (Mach-O ARM64, verified SHA-256 fingerprints)
- Frozen environment: macOS 26.6.2, Apple Clang 21.0.0, llvm-otool 21.0.0, Python 3.12.12 (.venv), pinned packages (`torch 2.13.0`, `torch-geometric 2.8.0.post1`, `numpy 2.5.2`, `pandas 3.0.5`, `scikit-learn 1.9.0`, `networkx 3.6.1`, `matplotlib 3.11.1`)
- Historical GraphEncoder reproduction: `EXACT REPRODUCTION` (STEP-003, commit `20d983e`, byte-identical CSV hashes)
- Historical Pooled-MLP reproduction: `EXACT REPRODUCTION` (STEP-004, commit `c9e5ed9`, byte-identical CSV hashes)
- Historical Handcrafted verification: F1=`0.5310`, Accuracy=`0.5583`, Precision=`0.5660`, Recall=`0.5000`, Positive Mean=`0.6463`, Negative Mean=`0.4526`, Separation=`0.1937` (verified exact match across both runs)
- DeepWeak status: Historical auxiliary Mini-DeepBugger component — preserved and unmodified; not reproduced during provenance baseline freeze
- Known documentation drift: `README.md` and `docs/DEEPWEAK_V1_PROTOCOL.md` modified after historical POC_V1/DEEPWEAK_V1 checksum generation; preserved as historical documentation drift without altering documents
- Verification performed: Verified SHA-256 integrity of all 17 baseline-critical files (13 tracked + 4 gitignored binaries) against `frozen_baseline/BASELINE_SHA256SUMS.txt`; verified zero modifications to research code, manifests, configs, results, or main branch
- Result: GATE A — BASELINE FREEZE: PASSED
- Decisions made: Created isolated `frozen_baseline/` directory with `BASELINE_FREEZE.md` and `BASELINE_SHA256SUMS.txt` rather than duplicating the source tree; established immutable baseline reference point for DeepBugger Extension
- Open questions: None
- Git commit: (pending local commit)
- Git tag: (pending tag creation)
- Next proposed micro-project: STEP-006 — Phase 1: Establish the DeepBugger Provenance Schema & Protocol (`PROVBENCH_SCHEMA.md`).







