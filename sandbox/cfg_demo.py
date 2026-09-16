import networkx as nx

G = nx.DiGraph()

G.add_node("BB0", semantic="condition")
G.add_node("BB1", semantic="jump")
G.add_node("BB2", semantic="set_true_result")
G.add_node("BB3", semantic="set_false_result")
G.add_node("BB4", semantic="return")

G.add_edge("BB0", "BB1", condition="x > 10")
G.add_edge("BB0", "BB3", condition="x <= 10")
G.add_edge("BB1", "BB2")
G.add_edge("BB2", "BB4")
G.add_edge("BB3", "BB4")

print("=== Mini-DeepBugger CFG Demo ===")

print("\nNodes:")
for node, data in G.nodes(data=True):
    print(node, data)

print("\nEdges:")
for src, dst, data in G.edges(data=True):
    print(src, "->", dst, data)

print("\nGraph Statistics:")
print("Number of nodes:", G.number_of_nodes())
print("Number of edges:", G.number_of_edges())