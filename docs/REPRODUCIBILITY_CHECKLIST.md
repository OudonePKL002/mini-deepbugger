# Mini-DeepClone PoC v1
## Reproducibility Checklist

### Core Scientific Configuration
- [x] ARM64 macOS environment recorded
- [x] Python version recorded
- [x] Apple Clang version recorded
- [x] 29-D semantic A-CFG features frozen
- [x] GCN architecture frozen: 29 -> 32 -> 16
- [x] Global mean pooling frozen
- [x] CosineEmbeddingLoss margin = 0.5
- [x] Adam learning rate = 0.01
- [x] Epochs = 100
- [x] Seeds = 42, 123, 2026, 7, 99
- [x] Cosine threshold = 0.5

### Data Protocol
- [x] Phase-1 manifest frozen
- [x] Phase-2 manifest frozen
- [x] Phase-1 training pairs = 120
- [x] Phase-2 evaluation pairs = 120
- [x] Phase-2 positive pairs = 60
- [x] Phase-2 negative pairs = 60
- [x] Phase-1 / Phase-2 family overlap = 0
- [x] Phase-2 used for evaluation only

### Core Code
- [x] src/acfg_builder.py
- [x] src/graph_encoder.py
- [x] experiments/run_final_experiment.py
- [x] config/experiment.json

### Final Results
- [x] Handcrafted A-CFG baseline reproduced
- [x] Graph Encoder 5-seed evaluation reproduced
- [x] Final CSV outputs generated
- [x] Independent Phase-2 results reproduced

### Final Frozen Phase-2 Metrics

Handcrafted A-CFG:
- Accuracy = 0.5583
- Precision = 0.5660
- Recall = 0.5000
- F1 = 0.5310
- Separation = 0.1937

Graph Encoder:
- Accuracy = 0.7517 +/- 0.0037
- Precision = 0.7071 +/- 0.0093
- Recall = 0.8600 +/- 0.0224
- F1 = 0.7759 +/- 0.0045
- Separation = 0.4755 +/- 0.0318

### Reproduction Command

```bash
python experiments/run_final_experiment.py