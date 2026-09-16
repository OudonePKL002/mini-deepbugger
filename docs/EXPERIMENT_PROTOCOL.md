# Mini-DeepClone PoC v1
## Final Reproducible Experiment Protocol

## 1. Purpose

Mini-DeepClone evaluates binary function similarity under compiler
optimization transformations.

The objective is to determine whether a learned graph representation
can identify semantically equivalent functions even when compiler
optimization changes their instruction sequences and control-flow
structures.


## 2. Platform

The frozen PoC v1 environment is:

- Architecture: ARM64
- Operating System: macOS
- Compiler: Apple Clang
- Python: 3.12
- PyTorch
- PyTorch Geometric


## 3. Compiler Variants

Each source function is compiled using four optimization levels:

- O0
- O1
- O2
- O3

Binary files:

sandbox/variants/hello_O0
sandbox/variants/hello_O1
sandbox/variants/hello_O2
sandbox/variants/hello_O3


## 4. Semantic A-CFG

Binary functions are automatically converted into semantic
Attributed Control-Flow Graphs (A-CFGs).

Pipeline:

Binary
-> Function Extraction
-> Basic Block Detection
-> CFG Construction
-> Semantic Feature Extraction
-> PyTorch Geometric Graph


Each basic block is represented by a:

29-dimensional semantic feature vector.


## 5. Graph Encoder

The frozen Graph Encoder architecture is:

Input:
29-dimensional node features

GCN Layer 1:
29 -> 32

ReLU

GCN Layer 2:
32 -> 16

ReLU

Global Mean Pooling

Output:
16-dimensional function embedding


## 6. Similarity

Similarity between two function embeddings is measured using:

Cosine Similarity


The fixed decision threshold is:

0.5


score >= 0.5
-> similar

score < 0.5
-> different


The threshold MUST NOT be tuned using Phase-2 data.


## 7. Metric Learning

Training uses:

CosineEmbeddingLoss

Margin:

0.5


Positive pair target:

+1


Negative pair target:

-1


Optimizer:

Adam


Learning rate:

0.01


Training epochs:

100


## 8. Random Seeds

All final experiments use:

42
123
2026
7
99


Results are reported as:

Mean +/- Standard Deviation


## 9. Phase-1 Dataset

Manifest:

data/variant_pair_manifest.csv


Phase 1 is used for:

- training
- development
- representation development


The final training subset contains:

120 training pairs

including:

60 positive pairs
60 negative pairs


Phase-1 evaluation results are considered development results,
not independent generalization results.


## 10. Phase-2 Independent Evaluation

Manifest:

data/phase2_independent_manifest.csv


Phase 2 contains:

10 unseen function families

40 graph samples

120 balanced pairs


Positive pairs:

60


Negative pairs:

60


Phase-2 function families:

- absolute_value
- square
- is_even
- is_odd
- average_two
- max_three
- min_three
- in_range
- clamp_zero_hundred
- sum_three


## 11. Leakage Prevention Rules

Phase-2 data MUST NOT be used for:

- model training
- feature engineering
- feature selection
- architecture tuning
- learning-rate tuning
- loss tuning
- margin tuning
- threshold tuning


Phase 2 is evaluation-only.


If the model is changed after examining Phase-2 results,
a new independent evaluation dataset must be created.


## 12. Handcrafted Baseline

The handcrafted baseline uses the same semantic A-CFG node features.

Graph representation is produced by summing basic-block feature vectors.

Similarity is calculated using cosine similarity.

Threshold:

0.5


This baseline is evaluated on exactly the same Phase-2 manifest.


## 13. Final Phase-2 Results

### Handcrafted A-CFG

Accuracy:
0.5583

Precision:
0.5660

Recall:
0.5000

F1:
0.5310

Separation:
0.1937


### Graph Encoder

Accuracy:
0.7517 +/- 0.0037

Precision:
0.7071 +/- 0.0093

Recall:
0.8600 +/- 0.0224

F1:
0.7759 +/- 0.0045

Positive similarity:
0.8543 +/- 0.0259

Negative similarity:
0.3788 +/- 0.0234

Separation:
0.4755 +/- 0.0318


## 14. Scientific Interpretation

The frozen Mini-DeepClone Graph Encoder outperforms direct
handcrafted A-CFG feature matching on the current independent
Phase-2 dataset.

The result provides evidence that learned graph representations can
improve robustness to compiler optimization transformations.

However, Mini-DeepClone PoC v1 remains a controlled proof-of-concept.

Future evaluation should include:

- larger function datasets
- real-world binaries
- additional compilers
- x86-64 binaries
- cross-compiler evaluation
- cross-architecture evaluation


## 15. Frozen Version

Version:

Mini-DeepClone Graph PoC v1

Once this protocol is frozen, Phase-2 results must not be used
to modify the v1 model.

Any subsequent model modification should be treated as:

Mini-DeepClone v2