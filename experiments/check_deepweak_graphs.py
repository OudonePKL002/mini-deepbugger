import csv
import sys
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[1]

if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))


from src.acfg_builder import build_pyg_graph


MANIFEST = (
    PROJECT_ROOT
    / "data"
    / "deepweak_manifest.csv"
)


BINARIES = {
    "O0": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O0",
    "O1": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O1",
    "O2": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O2",
    "O3": PROJECT_ROOT / "sandbox/deepweak/weaknesses_O3",
}


rows = []

with open(
    MANIFEST,
    newline="",
    encoding="utf-8",
) as f:

    reader = csv.DictReader(f)

    rows.extend(reader)


print("=== Mini-DeepWeak Graph Sanity Check ===")
print("Samples:", len(rows))
print()


invalid = []
graphs = {}


for row in rows:

    sample_id = row["sample_id"]
    function_name = row["function_name"]
    optimization = row["optimization"]

    graph = build_pyg_graph(
        str(BINARIES[optimization]),
        function_name,
    )

    graphs[sample_id] = graph

    valid = (
        graph.num_nodes > 0
        and graph.num_node_features == 29
    )

    if not valid:
        invalid.append(sample_id)

    print(
        f"{sample_id:32} "
        f"nodes={graph.num_nodes:<3} "
        f"edges={graph.num_edges:<3} "
        f"features={graph.num_node_features:<3} "
        f"{'OK' if valid else 'INVALID'}"
    )


print()
print("=== Summary ===")
print("Total graphs:", len(graphs))
print("Invalid graphs:", len(invalid))


if invalid:

    print("\nInvalid samples:")

    for sample_id in invalid:
        print(sample_id)

else:

    print("\nAll Mini-DeepWeak graphs are valid.")