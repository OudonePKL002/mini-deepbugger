# Table I. Final Independent Evaluation Results

| Module | Independent Evaluation Set | Accuracy | Precision | Recall | F1 | Additional Metric |
|---|---|---:|---:|---:|---:|---:|
| Mini-DeepClone | 10 unseen families / 120 balanced pairs | 0.7517 ± 0.0037 | 0.7071 ± 0.0093 | 0.8600 ± 0.0224 | **0.7759 ± 0.0045** | Separation = 0.4755 ± 0.0318 |
| Mini-DeepWeak | 6 unseen families / 48 balanced samples | **0.9917 ± 0.0114** | **1.0000 ± 0.0000** | **0.9833 ± 0.0228** | **0.9915 ± 0.0117** | ROC-AUC = 1.0000 ± 0.0000 |

## Mini-DeepClone Baseline Comparison

| Method | Accuracy | F1 | Separation |
|---|---:|---:|---:|
| Handcrafted A-CFG | 0.5583 | 0.5310 | 0.1937 |
| Graph Encoder | **0.7517 ± 0.0037** | **0.7759 ± 0.0045** | **0.4755 ± 0.0318** |