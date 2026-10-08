#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

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
P05_NOINLINE size_t p05_f04_adj_in_degree(const p05_graph_t *g, size_t u);
P05_NOINLINE size_t p05_f05_adj_out_degree(const p05_graph_t *g, size_t u);
P05_NOINLINE size_t p05_f06_graph_bfs(const p05_graph_t *g, size_t start, size_t *order, size_t max_order);
P05_NOINLINE void   p05_f11_graph_transitive_closure(const p05_graph_t *g, uint8_t reach[P05_MAX_V][P05_MAX_V]);
P05_NOINLINE size_t p05_f13_graph_isolate_count(const p05_graph_t *g);
P05_NOINLINE bool   p05_f14_graph_eulerian_path_check(const p05_graph_t *g);
P05_NOINLINE int    p05_f16_graph_vertex_eccentricity(const p05_graph_t *g, size_t u);

#endif /* TARGET_DONOR_COMPONENT_H */
