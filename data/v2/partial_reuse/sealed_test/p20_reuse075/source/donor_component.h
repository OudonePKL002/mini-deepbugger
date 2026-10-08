#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P19_NOINLINE __attribute__((noinline))
#define P19_MAX_VERTICES 24
#define P19_INF 10000000

typedef struct {
    int32_t weights[P19_MAX_VERTICES][P19_MAX_VERTICES];
    size_t num_vertices;
} p19_graph_t;

/* Benchmark Function Prototypes */
P19_NOINLINE void    p19_f03_dijkstra(const p19_graph_t *g, size_t src, int32_t *dist, int32_t *parent);
P19_NOINLINE bool    p19_f04_bellman_ford(const p19_graph_t *g, size_t src, int32_t *dist);
P19_NOINLINE bool    p19_f07_has_negative_cycle(const p19_graph_t *g);
P19_NOINLINE bool    p19_f15_is_bipartite(const p19_graph_t *g);

#endif /* TARGET_DONOR_COMPONENT_H */
