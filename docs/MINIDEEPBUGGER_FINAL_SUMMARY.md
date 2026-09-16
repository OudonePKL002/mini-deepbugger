# Mini-DeepBugger
## Final PoC Integration Summary

Mini-DeepBugger is composed of one shared binary graph representation
and two separate analysis objectives.

## Architecture

Binary Function
    |
    v
Disassembly
    |
    v
Basic Blocks / CFG
    |
    v
Shared 29-D Semantic A-CFG
    |
    +-------------------------------+
    |                               |
    v                               v
Mini-DeepClone                 Mini-DeepWeak
    |                               |
GCN 29 -> 32 -> 16            +10 security features
    |                          39-D node representation
16-D embedding                      |
    |                          GCN 39 -> 32 -> 16
Cosine similarity                   |
    |                          Linear classifier
Similarity score                    |
                               Vulnerable / Safe

## Mini-DeepClone

Objective:

Binary function similarity detection.

Independent evaluation:

- 10 unseen function families
- 40 graph samples
- 120 balanced pairs
- 5 random seeds

Results:

- Accuracy: 0.7517 +/- 0.0037
- Precision: 0.7071 +/- 0.0093
- Recall: 0.8600 +/- 0.0224
- F1: 0.7759 +/- 0.0045
- Similarity separation: 0.4755 +/- 0.0318

Handcrafted baseline:

- F1: 0.5310
- Separation: 0.1937

## Mini-DeepWeak

Objective:

Binary function vulnerability classification.

Independent evaluation:

- 6 unseen function families
- 48 graph samples
- 24 vulnerable
- 24 non-vulnerable
- 5 random seeds

Results:

- Accuracy: 0.9917 +/- 0.0114
- Precision: 1.0000 +/- 0.0000
- Recall: 0.9833 +/- 0.0228
- F1: 0.9915 +/- 0.0117
- ROC-AUC: 1.0000 +/- 0.0000

## Important Interpretation

Mini-DeepClone and Mini-DeepWeak share binary preprocessing and
semantic A-CFG concepts, but they solve different objectives.

A similarity score is not a vulnerability label.

Mini-DeepClone evaluates whether two binary functions are semantically
similar.

Mini-DeepWeak evaluates whether one binary function exhibits a
vulnerability-related pattern.

## Scope

The current system is a controlled proof-of-concept.

Current experiments primarily use:

- ARM64 macOS binaries
- Apple Clang
- O0/O1/O2/O3
- small controlled C functions
- family-level held-out evaluation

The results should therefore not be interpreted as universal
performance on arbitrary real-world binaries.

## Final Status

Shared Mini-DeepBugger backbone: COMPLETE

Mini-DeepClone PoC v1: FINAL / FROZEN

Mini-DeepWeak PoC v1: FINAL / FROZEN

Mini-DeepBugger integrated PoC: READY FOR PAPER PACKAGING