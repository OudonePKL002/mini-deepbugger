# Mini-DeepBugger Experiment Specification

## Objective

Develop a lightweight proof-of-concept of the DeepBugger
concept for function-level binary similarity analysis and
software vulnerability detection.

## Research Questions

RQ1:
Can instruction/graph-based representations distinguish
similar and dissimilar binary functions?

RQ2:
Can graph-based representations distinguish vulnerable
and benign functions?

RQ3:
Do structural and semantic features improve performance
compared with simple instruction features?

## Experimental Unit

One function = one sample.

## Vulnerability Labels

0 = Benign
1 = Vulnerable

## Analysis Type

Static analysis.

## Initial Platform

ARM64 (Apple Silicon).

The first proof-of-concept experiment uses native ARM64
binaries to simplify compilation and binary preprocessing
on the available MacBook M1 environment.

x86-64 and cross-architecture evaluation are reserved
for future experiments.

## DeepClone Task

1:1 function similarity.

## DeepWeak Task

Binary classification:
Benign vs Vulnerable.

## Initial Feature Groups

F1 = Opcode features
F2 = Opcode + structural features
F3 = Opcode + structural + semantic features

## Evaluation

DeepClone:
- Similarity score
- Positive/negative pair separation

DeepWeak:
- Accuracy
- Precision
- Recall
- F1
- False Positive
- False Negative

## Not Included in v0.1

- Dynamic analysis
- Zero-day detection
- Multi-class CWE classification
- Cross-platform evaluation
- Few-shot learning
- Large-scale models

## Primary Dataset

NIST SARD Juliet C/C++ 1.3

Initial vulnerability category:
CWE-121 Stack-Based Buffer Overflow