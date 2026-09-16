import platform

import numpy as np
import pandas as pd
import sklearn
import networkx as nx
import torch


print("=== Mini-DeepBugger Environment Check ===")

print("Python platform:", platform.platform())
print("PyTorch:", torch.__version__)
print("NumPy:", np.__version__)
print("Pandas:", pd.__version__)
print("Scikit-learn:", sklearn.__version__)
print("NetworkX:", nx.__version__)

print("\n=== Compute Device ===")

if torch.backends.mps.is_available():
    device = torch.device("mps")
    print("Device: MPS")
else:
    device = torch.device("cpu")
    print("Device: CPU")

print("\nEnvironment setup completed successfully.")