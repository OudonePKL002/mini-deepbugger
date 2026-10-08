#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P05_NOINLINE __attribute__((noinline))
#define P05_MAX_V 16

typedef struct {
    uint8_t adj[P05_MAX_V][P05_MAX_V];
    size_t num_vertices;
} p05_graph_t;

/* Benchmark Function Prototypes */
P05_NOINLINE void   p05_f01_adj_init(p05_graph_t *g, size_t n);
P05_NOINLINE bool   p05_f02_adj_add_edge(p05_graph_t *g, size_t u, size_t v);
P05_NOINLINE bool   p05_f03_adj_has_edge(const p05_graph_t *g, size_t u, size_t v);
P05_NOINLINE size_t p05_f04_adj_in_degree(const p05_graph_t *g, size_t u);
P05_NOINLINE size_t p05_f05_adj_out_degree(const p05_graph_t *g, size_t u);
P05_NOINLINE size_t p05_f06_graph_bfs(const p05_graph_t *g, size_t start, size_t *order, size_t max_order);
P05_NOINLINE size_t p05_f07_graph_dfs_iterative(const p05_graph_t *g, size_t start, size_t *order, size_t max_order);
P05_NOINLINE bool   p05_f08_graph_has_cycle_directed(const p05_graph_t *g);
P05_NOINLINE size_t p05_f09_graph_connected_components(const p05_graph_t *g, int *component_id);
P05_NOINLINE bool   p05_f10_graph_bipartite_check(const p05_graph_t *g);
P05_NOINLINE void   p05_f11_graph_transitive_closure(const p05_graph_t *g, uint8_t reach[P05_MAX_V][P05_MAX_V]);
P05_NOINLINE size_t p05_f12_graph_density(const p05_graph_t *g);
P05_NOINLINE size_t p05_f13_graph_isolate_count(const p05_graph_t *g);
P05_NOINLINE bool   p05_f14_graph_eulerian_path_check(const p05_graph_t *g);
P05_NOINLINE void   p05_f15_graph_subgraph_induced(const p05_graph_t *g, const bool *keep_mask, p05_graph_t *sub);
P05_NOINLINE int    p05_f16_graph_vertex_eccentricity(const p05_graph_t *g, size_t u);

#endif /* TARGET_QUERY_COMPONENT_H */
