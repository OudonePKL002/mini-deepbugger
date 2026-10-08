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
P05_NOINLINE size_t p05_f05_adj_out_degree(const p05_graph_t *g, size_t u);
P05_NOINLINE bool   p05_f08_graph_has_cycle_directed(const p05_graph_t *g);
P05_NOINLINE void   p05_f11_graph_transitive_closure(const p05_graph_t *g, uint8_t reach[P05_MAX_V][P05_MAX_V]);
P05_NOINLINE size_t p05_f12_graph_density(const p05_graph_t *g);

#endif /* TARGET_QUERY_COMPONENT_H */
