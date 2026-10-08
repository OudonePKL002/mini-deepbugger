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
P19_NOINLINE bool    p19_f02_sp_add_edge(p19_graph_t *g, size_t u, size_t v, int32_t w);
P19_NOINLINE void    p19_f03_dijkstra(const p19_graph_t *g, size_t src, int32_t *dist, int32_t *parent);
P19_NOINLINE bool    p19_f04_bellman_ford(const p19_graph_t *g, size_t src, int32_t *dist);
P19_NOINLINE void    p19_f05_floyd_warshall(const p19_graph_t *g, int32_t dist[P19_MAX_VERTICES][P19_MAX_VERTICES]);
P19_NOINLINE size_t  p19_f06_reconstruct_path(const int32_t *parent, size_t dst, size_t *out_path, size_t max_nodes);
P19_NOINLINE bool    p19_f07_has_negative_cycle(const p19_graph_t *g);
P19_NOINLINE int32_t p19_f08_bidirectional_dijkstra(const p19_graph_t *g, size_t src, size_t dst);
P19_NOINLINE int32_t p19_f09_prim_mst(const p19_graph_t *g, size_t src);
P19_NOINLINE int32_t p19_f10_kruskal_mst(const p19_graph_t *g);
P19_NOINLINE int32_t p19_f11_eccentricity(const p19_graph_t *g, size_t v);
P19_NOINLINE int32_t p19_f12_graph_diameter(const p19_graph_t *g);
P19_NOINLINE int32_t p19_f13_graph_radius(const p19_graph_t *g);
P19_NOINLINE size_t  p19_f14_count_connected_components(const p19_graph_t *g);
P19_NOINLINE bool    p19_f15_is_bipartite(const p19_graph_t *g);
P19_NOINLINE void    p19_f16_sp_clear(p19_graph_t *g);

#endif /* TARGET_QUERY_COMPONENT_H */
