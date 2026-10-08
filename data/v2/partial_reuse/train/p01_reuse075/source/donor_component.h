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
P05_NOINLINE bool   p05_f02_adj_add_edge(p05_graph_t *g, size_t u, size_t v);
P05_NOINLINE size_t p05_f12_graph_density(const p05_graph_t *g);
P05_NOINLINE int    p05_f16_graph_vertex_eccentricity(const p05_graph_t *g, size_t u);

#endif /* TARGET_DONOR_COMPONENT_H */
