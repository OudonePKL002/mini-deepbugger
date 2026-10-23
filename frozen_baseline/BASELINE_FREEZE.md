# Mini-DeepBugger / APSEC Baseline Freeze Document

## A. Freeze Identity

- **Project:** Mini-DeepBugger / APSEC baseline
- **Purpose:** Immutable historical reference baseline for the DeepBugger Extension research project
- **Baseline Branch:** `main`
- **Baseline Commit:** `194999ea1b692b771702484b3fd471386a75c81b`
- **Extension Verification Branch:** `deepbugger-extension`
- **STEP-003 Verification Commit:** `20d983e`
- **STEP-004 Verification Commit:** `c9e5ed9`
- **Freeze Status:** `VERIFIED AND FROZEN`

---

## B. Freeze Meaning

The Mini-DeepBugger baseline is frozen for historical reproduction and scientific comparison.

From this point forward:
- Do not modify baseline behavior.
- Do not alter the historical 29-D feature definition.
- Do not alter historical manifests.
- Do not alter historical experiment configurations.
- Do not overwrite historical result files.
- Do not silently rebuild historical binaries.
- Do not tune baseline thresholds after extension experiments begin.

All DeepBugger Extension work must be implemented separately on `deepbugger-extension` (or later approved extension branches) without modifying the frozen baseline in place.

---

## C. Historical Pipeline

The historical binary analysis pipeline established during STEP-001 and verified during STEP-002..STEP-004 consists of:

```text
Binary (`sandbox/variants/hello_O0`..`hello_O3`)
  ↓
otool / disassembly (`extract_function_instructions` in `src/acfg_builder.py`)
  ↓
Basic Blocks / CFG (`split_into_basic_blocks`, `build_cfg` in `src/acfg_builder.py`)
  ↓
29-D semantic A-CFG (`compute_node_features`, `build_pyg_graph` in `src/acfg_builder.py`)
  ↓
Baselines & Models:
  ├─ Handcrafted sum-pooling cosine similarity (`experiments/run_final_experiment.py`)
  ├─ Pooled-Node MLP ablation (`experiments/evaluate_clone_pooled_mlp_baseline.py`)
  └─ DeepClone GraphEncoder (`src/graph_encoder.py`, `experiments/run_final_experiment.py`)
  ↓
Cosine similarity scoring (`torch.nn.functional.cosine_similarity`)
  ↓
Historical Phase-2 independent evaluation (`data/phase2_independent_manifest.csv`)
```

---

## D. 29-D Feature Freeze

The exact historical 29-D basic block feature schema is implemented in `src/acfg_builder.py` inside `compute_node_features(block_instructions, cfg, block_label)`:

- **Source Path:** `src/acfg_builder.py`
- **Function:** `compute_node_features`
- **Ordering of the 29 Dimensions:**
  - **Instruction Category Counts (10 dims):**
    - `D0`: `num_instructions` (total count of assembly instructions in basic block)
    - `D1`: `num_arithmetic` (e.g., `add`, `sub`, `mul`, `div`, `neg`)
    - `D2`: `num_logical` (e.g., `and`, `orr`, `eor`, `mvn`, `bic`, `lsl`, `lsr`, `asr`, `ror`)
    - `D3`: `num_control_transfer` (e.g., `b`, `bl`, `br`, `blr`, `ret`, `cbz`, `cbnz`, `tbz`, `tbnz`)
    - `D4`: `num_load_store` (e.g., `ldr`, `str`, `ldp`, `stp`, `ldrb`, `strb`)
    - `D5`: `num_comparison` (e.g., `cmp`, `cmn`, `tst`, `csel`, `cset`)
    - `D6`: `num_floating_point` (ARM64 FP / vector instructions: `fadd`, `fmul`, `fmov`, etc.)
    - `D7`: `num_system` (e.g., `svc`, `mrs`, `msr`, `nop`, `brk`)
    - `D8`: `num_string` (string / memory block operations)
    - `D9`: `num_crypto` (ARMv8 crypto extensions: `aes`, `sha`, etc.)
  - **Register Usage Counts (5 dims):**
    - `D10`: `num_registers_read` (total register read occurrences)
    - `D11`: `num_registers_written` (total register destination occurrences)
    - `D12`: `num_general_registers` (ARM64 general-purpose registers: `x0`–`x30`, `w0`–`w30`)
    - `D13`: `num_vector_registers` (ARM64 SIMD/FP registers: `v0`–`v31`, `q0`–`q31`, `d0`–`d31`, `s0`–`s31`)
    - `D14`: `num_system_registers` (e.g., `sp`, `xzr`, `wzr`, `fpsr`, `fpcr`, `nzcv`)
  - **Operand & Immediate Constants (2 dims):**
    - `D15`: `num_immediate_constants` (immediate literals prefixed with `#`)
    - `D16`: `num_memory_operands` (memory address references enclosed in `[...]`)
  - **Structural / CFG Topology Features (12 dims):**
    - `D17`: `num_control_in_edges` (in-degree in CFG)
    - `D18`: `num_control_out_edges` (out-degree in CFG)
    - `D19`: `is_entry_block` (`1.0` if entry block else `0.0`)
    - `D20`: `is_exit_block` (`1.0` if exit/ret block else `0.0`)
    - `D21`: `loop_depth` (loop nest depth computed via simple cycle membership)
    - `D22`: `cyclomatic_complexity` (block cyclomatic complexity: out-degree - 1, min 0)
    - `D23`: `betweenness_centrality` (NetworkX directed betweenness centrality)
    - `D24`: `closeness_centrality` (NetworkX directed closeness centrality)
    - `D25`: `eigenvector_centrality` (NetworkX directed eigenvector centrality with fallback)
    - `D26`: `pagerank` (NetworkX directed PageRank)
    - `D27`: `degree_centrality` (NetworkX directed degree centrality)
    - `D28`: `clustering_coefficient` (NetworkX directed clustering coefficient)

- **Per-Basic-Block Representation:** Every basic block is a graph node with a 29-D raw float feature vector `x`. No global scaling or z-score normalization is applied.
- **Identified STEP-001 Uncertainties:** Centrality features on isolated single-node graphs (common at `-O1`, `-O2`, `-O3` where compiler inlining collapses helper functions into single blocks) evaluate to trivial defaults (`0.0` or `1.0`). Future 36-D DeepBugger features must extend this frozen 29-D representation without altering existing dimension semantics or historical index positions.

---

## E. DeepClone GraphEncoder Freeze

Confirmed from `src/graph_encoder.py` and `config/experiment.json`:
- **Input Dimension:** 29
- **Hidden Dimension:** 32
- **Embedding Dimension:** 16
- **Architecture:** 2-layer Graph Convolutional Network:
  ```python
  conv1 = GCNConv(29, 32)
  relu = ReLU()
  conv2 = GCNConv(32, 16)
  pooling = global_mean_pool(x, batch)
  ```
- **Loss:** `CosineEmbeddingLoss(margin=0.5)`
- **Optimizer:** `Adam(lr=0.01)`
- **Epochs:** 100
- **Evaluation Threshold:** 0.5 (cosine similarity $\ge$ 0.5 $\to$ positive reuse clone)
- **Evaluation Metric:** Cosine similarity between 16-D graph embeddings
- **Frozen Random Seeds:** `[42, 123, 2026, 7, 99]`

---

## F. Pooled-Node MLP Freeze

Confirmed from `experiments/evaluate_clone_pooled_mlp_baseline.py`:
- **Pooling:** 29-D node mean-pooling (`graph.x.mean(dim=0, keepdim=True)`); no edge connections or message passing
- **Architecture:** 2-layer Multilayer Perceptron:
  ```python
  Sequential(
      Linear(29, 32),
      ReLU(),
      Linear(32, 16),
      ReLU()
  )
  ```
- **Loss:** `CosineEmbeddingLoss(margin=0.5)`
- **Optimizer:** `Adam(lr=0.01)`
- **Epochs:** 100
- **Threshold:** 0.5
- **Frozen Random Seeds:** `[42, 123, 2026, 7, 99]`

---

## G. Dataset / Manifest Freeze

- **Phase-1 Training Manifest (`data/variant_pair_manifest.csv`):**
  - Total rows: 156
  - Training pairs: 120 (60 positive clones, 60 negative non-clones)
  - Testing pairs: 36 (18 positive, 18 negative)
  - Function families: 10 synthetic functions compiled across 4 optimization levels (`O0`–`O3`)
- **Phase-2 Independent Evaluation Manifest (`data/phase2_independent_manifest.csv`):**
  - Total evaluation pairs: 120
  - Positive pairs: 60
  - Negative pairs: 60
  - Function families: 10 independent unseen synthetic functions
  - Overlap with training families: Exactly 0 (verified zero data leakage)

---

## H. Historical Binary Freeze

The 4 local compiler-generated binaries in `sandbox/variants/` are gitignored historical artifacts. Their SHA-256 fingerprints define the reproduced baseline inputs:

| Binary Path | File Type | Size (Bytes) | SHA-256 Checksum |
| :--- | :--- | :--- | :--- |
| `sandbox/variants/hello_O0` | `Mach-O 64-bit executable arm64` | 34,888 | `b03e786e94664b48af30e951ffee1c74dc7b8632a40d3e9ac6f12f3edf99784c` |
| `sandbox/variants/hello_O1` | `Mach-O 64-bit executable arm64` | 34,888 | `42f4f57aa035e0d8ab3d4f9205c450272a7b49e0c30f549917c977192ac2ba57` |
| `sandbox/variants/hello_O2` | `Mach-O 64-bit executable arm64` | 34,888 | `b563d76883a532b15a4ee7ea142b66cf70a1ce8b823d2f758f6b4f36901925c0` |
| `sandbox/variants/hello_O3` | `Mach-O 64-bit executable arm64` | 34,888 | `eea484872eb13a00aaae13a0d3ebcc4b4f3dadf5ce40ac82bcd46b366df0c18d` |

---

## I. Environment Freeze

The verified reproduction environment under which byte-identical results were achieved:
- **Operating System:** macOS 26.6.2 (Build 25G83), Darwin Kernel Version 25.6.0
- **Architecture:** ARM64 (Apple Silicon T8103)
- **Compiler:** Apple Clang version 21.0.0 (`clang-2100.1.1.101`), Target: `arm64-apple-darwin25.6.0`
- **Disassembler:** Apple `llvm-otool(1)` version `cctools-1040`, LLVM 21.0.0
- **Python:** Python 3.12.12 (in `.venv`)
- **Pinned Dependencies (`requirements.txt` / `requirements-frozen.txt`):**
  - `torch == 2.13.0`
  - `torch-geometric == 2.8.0.post1`
  - `numpy == 2.5.2`
  - `pandas == 3.0.5`
  - `scikit-learn == 1.9.0`
  - `networkx == 3.6.1`
  - `matplotlib == 3.11.1`

---

## J. Historical Result Freeze

Verified baseline evaluation metrics on Phase-2 Independent (`n = 120` pairs):

### 1. Handcrafted Baseline (Deterministic Sum-Pooling Cosine Similarity)
- **Accuracy:** `0.5583333333333333` (`0.5583`)
- **Precision:** `0.5660377358490566` (`0.5660`)
- **Recall:** `0.5` (`0.5000`)
- **F1:** `0.5309734513274337` (`0.5310`)
- **Positive Mean Similarity:** `0.6462890081107616` (`0.6463`)
- **Negative Mean Similarity:** `0.45258812978863716` (`0.4526`)
- **Separation:** `0.19370087832212446` (`0.1937`)

### 2. Pooled-Node MLP Baseline (5 Seeds, Mean $\pm$ Std)
- **Accuracy:** `0.7033333333333334 ± 0.030390970881350794` (`0.7033 ± 0.0304`)
- **Precision:** `0.6460757939750379 ± 0.029492084696913733` (`0.6461 ± 0.0295`)
- **Recall:** `0.9099999999999999 ± 0.08944271909999157` (`0.9100 ± 0.0894`)
- **F1:** `0.7531376990780363 ± 0.030897680657538562` (`0.7531 ± 0.0309`)
- **Positive Mean Similarity:** `0.8957507936656475 ± 0.06627019148397004` (`0.8958 ± 0.0663`)
- **Negative Mean Similarity:** `0.5300020549855738 ± 0.08843542158389567` (`0.5300 ± 0.0884`)
- **Separation:** `0.3657487386800736 ± 0.033130125271054696` (`0.3657 ± 0.0331`)

### 3. DeepClone GraphEncoder (5 Seeds, Mean $\pm$ Std)
- **Accuracy:** `0.7516666666666667 ± 0.0037267799624996364` (`0.7517 ± 0.0037`)
- **Precision:** `0.7071235102925244 ± 0.009341224207796935` (`0.7071 ± 0.0093`)
- **Recall:** `0.86 ± 0.022360679774997918` (`0.8600 ± 0.0224`)
- **F1:** `0.775883293606493 ± 0.004545179401549354` (`0.7759 ± 0.0045`)
- **Positive Mean Similarity:** `0.8542568600177765 ± 0.02591326212407821` (`0.8543 ± 0.0259`)
- **Negative Mean Similarity:** `0.3787666478927713 ± 0.023401277394816317` (`0.3788 ± 0.0234`)
- **Separation:** `0.4754902121250052 ± 0.03182988040264492` (`0.4755 ± 0.0318`)

---

## K. Reproduction Evidence

- **STEP-003 (DeepClone GraphEncoder):** `EXACT REPRODUCTION`  
  Historical and reproduced CSV files were byte-identical.
- **STEP-004 (Pooled-Node MLP):** `EXACT REPRODUCTION`  
  Historical and reproduced CSV files were byte-identical.

Reproduction was byte-identical under the recorded environment, frozen inputs, and seeds.

---

## L. DeepWeak Status

- **DeepWeak Status:** Historical auxiliary component.
- DeepWeak was not reproduced as part of the provenance baseline freeze because DeepBugger Extension follows the DeepClone/provenance path.
- Existing DeepWeak files, manifests (`data/deepweak_w1_split_v2.csv`, `data/deepweak_w2_manifest.csv`), results (`results/deepweak_w2_frozen_results.csv`), and protocol documentation remain preserved and unmodified.

---

## M. Extension Rule

DeepBugger Extension must not modify the frozen Mini-DeepBugger implementation in place. New work should be isolated under extension-specific modules and directories where practical:
- `deepbugger/` (future 36-D provenance encoder, region matching, evidence aggregator)
- `provbench/` (future ProvBench datasets, manifests, lineage metadata)
- `experiments/rq1/`, `experiments/rq2/`, ... (research question experiment runners)

The frozen baseline remains the immutable comparison and reference infrastructure.

