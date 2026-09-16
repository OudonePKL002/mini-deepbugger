# Mini-DeepWeak PoC v1
## Frozen Experiment Protocol

## Task

Binary function vulnerability classification:

- 0 = non-vulnerable
- 1 = vulnerable

## Shared Representation

Mini-DeepWeak reuses the Mini-DeepBugger shared semantic A-CFG.

Shared node features:

29 dimensions

DeepWeak security-aware extension:

10 dimensions

Total:

39-dimensional node representation

## Security-Aware Features

1. unsafe_copy_call
2. bounded_copy_call
3. null_check
4. bounds_check
5. division
6. zero_guard
7. arithmetic_overflow_guard
8. range_guard
9. dynamic_format_call
10. literal_format_call

Some guard features are propagated using function-level context
when the guard and protected operation occur in different basic blocks.

## Classifier

Input:
39-D node features

GCN Layer 1:
39 -> 32

ReLU

GCN Layer 2:
32 -> 16

ReLU

Global Mean Pooling

Linear classifier:
16 -> 1

Loss:
BCEWithLogitsLoss

Optimizer:
Adam

Learning rate:
0.01

Epochs:
100

Decision threshold:
0.5

Seeds:

42
123
2026
7
99

## W1 Development Dataset

W1 was used for feature and model development.

Final W1-v2 training families:

- buffer_index
- division
- format
- integer_overflow
- null_pointer
- signed_range
- text_copy

Development families:

- array_bounds
- buffer_copy

W1-v2 training samples:

56

W1 development samples:

16

W1 was not treated as independent evaluation.

## W2 Independent Evaluation

W2 contains six previously unseen function families:

- safe_divide_alt
- pointer_write
- index_write
- length_copy
- multiplication_guard
- format_wrapper

Samples:

48

Vulnerable:

24

Non-vulnerable:

24

W1/W2 family overlap:

0

## W2 Independent Results

Accuracy:
0.9917 +/- 0.0114

Precision:
1.0000 +/- 0.0000

Recall:
0.9833 +/- 0.0228

F1:
0.9915 +/- 0.0117

ROC-AUC:
1.0000 +/- 0.0000

Positive mean probability:
0.9874 +/- 0.0168

Negative mean probability:
0.0000 +/- 0.0000

Probability separation:
0.9874 +/- 0.0168

## Leakage Rules

W2 must not be used for:

- training
- feature engineering
- architecture tuning
- learning-rate tuning
- threshold tuning
- security-feature modification

Any model modification after observing W2 must be treated as
Mini-DeepWeak v2 and evaluated using a new independent dataset.

## Status

Mini-DeepWeak PoC v1
FINAL / FROZEN
