#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

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
P19_NOINLINE void    p19_f01_sp_init(p19_graph_t *g, size_t n);
P19_NOINLINE void    p19_f03_dijkstra(const p19_graph_t *g, size_t src, int32_t *dist, int32_t *parent);
P19_NOINLINE int32_t p19_f11_eccentricity(const p19_graph_t *g, size_t v);
P19_NOINLINE void    p19_f16_sp_clear(p19_graph_t *g);

#endif /* TARGET_QUERY_COMPONENT_H */
