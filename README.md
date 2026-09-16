# Mini-DeepClone PoC v1

Mini-DeepClone is a proof-of-concept system for binary function similarity analysis under compiler optimization transformations.

The system converts binary functions into semantic Attributed Control-Flow Graphs (A-CFGs), encodes them with a Graph Convolutional Network (GCN), and compares learned function embeddings using cosine similarity.

---

## 1. Project Goal

The main research question is:

> Can a learned graph representation identify semantically equivalent binary functions even when compiler optimization changes their instructions and control-flow structures?

Mini-DeepClone focuses on cross-optimization binary similarity using:

- Apple Clang
- ARM64 macOS binaries
- Optimization levels O0 / O1 / O2 / O3
- Semantic A-CFGs
- Graph Neural Networks
- Metric learning

---

## 2. Final Frozen Pipeline

```text
C Source Code
      ↓
Clang Compilation
-O0 / -O1 / -O2 / -O3
      ↓
ARM64 Binary
      ↓
Function Extraction
      ↓
Basic Block Construction
      ↓
Control-Flow Graph
      ↓
29-D Semantic Node Features
      ↓
Semantic A-CFG
      ↓
2-Layer GCN Encoder
29 → 32 → 16
      ↓
Global Mean Pooling
      ↓
16-D Function Embedding
      ↓
Cosine Similarity
      ↓
Similar / Different
```

## 3. Project Structure
Mini-DeepBugger/
│
├── README.md
├── requirements.txt
│
├── config/
│   └── experiment.json
│
├── src/
│   ├── __init__.py
│   ├── acfg_builder.py
│   └── graph_encoder.py
│
├── experiments/
│   └── run_final_experiment.py
│
├── data/
│   ├── variant_pair_manifest.csv
│   └── phase2_independent_manifest.csv
│
├── sandbox/
│   ├── hello.c
│   └── variants/
│       ├── hello_O0
│       ├── hello_O1
│       ├── hello_O2
│       └── hello_O3
│
├── results/
│   ├── phase1/
│   ├── phase2/
│   └── final/
│
├── figures/
│
├── docs/
│   └── EXPERIMENT_PROTOCOL.md
│
└── archive/
    └── legacy_scripts/

## 4. Frozen Environment
The final PoC v1 experiment was reproduced using:
Operating System : macOS
Architecture     : ARM64
Python           : 3.12.12
Compiler         : Apple Clang 21.0.0

Python dependencies are recorded in:
requirements.txt

Install dependencies using:
python -m venv .venv
source .venv/bin/activate

pip install -r requirements.txt

## 5. Compiler Variants
The experiment uses four optimization levels:
-O0
-O1
-O2
-O3

The frozen binaries are located in:
sandbox/variants/

Example compilation:
clang -O0 sandbox/hello.c -o sandbox/variants/hello_O0
clang -O1 sandbox/hello.c -o sandbox/variants/hello_O1
clang -O2 sandbox/hello.c -o sandbox/variants/hello_O2
clang -O3 sandbox/hello.c -o sandbox/variants/hello_O3

## 6. Semantic A-CFG Representation
Each binary function is automatically processed as:
Disassembly
→ Function Extraction
→ Basic Block Detection
→ CFG Edge Construction
→ Semantic Feature Extraction
→ PyTorch Geometric Data

Each basic block is represented using:
29 semantic node features

The implementation is located in:
src/acfg_builder.py

## 7. Graph Encoder
The frozen graph architecture is:
Input features
29 dimensions

GCN Layer 1
29 → 32

ReLU

GCN Layer 2
32 → 16

ReLU

Global Mean Pooling

Output embedding
16 dimensions

Implementation:
src/graph_encoder.py

## 8. Training Configuration
The frozen PoC v1 configuration is:
Loss:
CosineEmbeddingLoss

Margin:
0.5

Optimizer:
Adam

Learning rate:
0.01

Epochs:
100

Seeds:
42
123
2026
7
99

Similarity is calculated using:
Cosine Similarity

Fixed classification threshold:
0.5

The complete machine-readable configuration is stored in:
config/experiment.json

## 9. Phase-1 Protocol
Phase 1 is used for:
- training
- representation development
- controlled PoC evaluation
Manifest:
data/variant_pair_manifest.csv

Frozen training subset:
120 training pairs

60 positive
60 negative

Phase-1 results are considered development results and are not treated as the final independent generalization result.
Final development checkpoint:
Accuracy   = 0.9500 ± 0.0692
Precision  = 0.9401 ± 0.0851
Recall     = 0.9667 ± 0.0745
F1         = 0.9514 ± 0.0682
Separation = 0.8068 ± 0.1043

## 10. Phase-2 Independent Evaluation
Phase 2 is the main independent evaluation set.
Manifest:
data/phase2_independent_manifest.csv

Phase-2 contains:
10 unseen function families
40 graph samples
120 balanced pairs

60 positive pairs
60 negative pairs

The 10 unseen families are:
absolute_value
square
is_even
is_odd
average_two
max_three
min_three
in_range
clamp_zero_hundred
sum_three

## 11. Leakage Prevention
Phase-2 data must NEVER be used for:
- model training
- feature engineering
- architecture tuning
- learning-rate tuning
- loss tuning
- margin tuning
- threshold tuning
Phase 2 is evaluation-only.
If Mini-DeepClone is modified after examining Phase-2 results, that new system must be treated as:
Mini-DeepClone v2

and evaluated using a new independent dataset.

## 12. Final Reproducible Experiment
Run the complete frozen experiment with:
python experiments/run_final_experiment.py

The runner performs:
1. Load frozen configuration
2. Validate manifests
3. Validate binaries
4. Check Phase-1 / Phase-2 leakage
5. Build A-CFG graph cache
6. Validate 29-D node features
7. Train Phase-1 model for 5 seeds
8. Evaluate Phase-2
9. Evaluate handcrafted baseline
10. Save final CSV results

## 13. Final Independent Results
Handcrafted A-CFG Baseline
Accuracy   = 0.5583
Precision  = 0.5660
Recall     = 0.5000
F1         = 0.5310
Separation = 0.1937

Graph Encoder
Five-seed mean ± standard deviation:
Accuracy   = 0.7517 ± 0.0037
Precision  = 0.7071 ± 0.0093
Recall     = 0.8600 ± 0.0224
F1         = 0.7759 ± 0.0045
Separation = 0.4755 ± 0.0318

Similarity distributions:
Positive pairs
0.8543 ± 0.0259

Negative pairs
0.3788 ± 0.0234

## 14. Improvement over Handcrafted Baseline
Absolute F1 improvement:
+0.2449

Relative F1 improvement:
46.12%

Embedding separation:
Handcrafted:
0.1937

Graph Encoder:
0.4755 ± 0.0318

## 15. Final Result Files
The reproducible runner generates:
results/final/phase2_graph_per_seed.csv
results/final/final_experiment_summary.csv

Existing frozen Phase-1 and Phase-2 experiment records are stored in:

results/phase1/
results/phase2/

## 16. Scientific Interpretation
The current independent evaluation provides evidence that the learned semantic graph representation improves cross-optimization binary function similarity detection compared with direct handcrafted A-CFG feature matching.
However, Mini-DeepClone PoC v1 remains a controlled proof-of-concept.
The current result should not be interpreted as evidence of universal robustness across all binaries, compilers, architectures, or real-world software.

## 17. Current Limitations
The current PoC primarily uses:
- one compiler family
- ARM64 binaries
- small controlled C functions
- a relatively small independent evaluation set
- compiler optimization transformations only
Future work should evaluate:
- larger real-world binary functions
- GCC and Clang
- x86-64
- cross-compiler similarity
- cross-architecture similarity
- public binary similarity benchmarks
- additional graph models
- ROC-AUC / PR-AUC
- larger-scale statistical evaluation

## 18. Reproducibility Status
Mini-DeepClone PoC v1 has been successfully reproduced using the final single-command runner.

Frozen configuration          ✓
Semantic A-CFG extraction     ✓
Graph Encoder                 ✓
Phase-1 training              ✓
Phase-2 independent testing   ✓
Handcrafted baseline          ✓
Five-seed evaluation          ✓
Final CSV generation          ✓
Leakage validation            ✓

Status:
Mini-DeepClone Graph PoC v1
FINAL / FROZEN
