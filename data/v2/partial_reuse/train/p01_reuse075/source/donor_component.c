#include "donor_component.h"
#include <string.h>
#include <math.h>

P05_NOINLINE void p05_f01_adj_init(p05_graph_t *g, size_t n) {
    if (!g) return;
    g->num_vertices = (n > P05_MAX_V) ? P05_MAX_V : n;
    for (size_t r = 0; r < P05_MAX_V; ++r) {
        for (size_t c = 0; c < P05_MAX_V; ++c) {
            g->adj[r][c] = 0;
        }
    }
}

P05_NOINLINE bool p05_f02_adj_add_edge(p05_graph_t *g, size_t u, size_t v) {
    if (!g || u >= g->num_vertices || v >= g->num_vertices) return false;
    g->adj[u][v] = 1;
    return true;
}

P05_NOINLINE size_t p05_f12_graph_density(const p05_graph_t *g) {
    if (!g || g->num_vertices < 2) return 0;
    size_t edges = 0;
    for (size_t r = 0; r < g->num_vertices; ++r) {
        for (size_t c = 0; c < g->num_vertices; ++c) {
            if (g->adj[r][c]) edges++;
        }
    }
    size_t max_edges = g->num_vertices * (g->num_vertices - 1);
    return (edges * 100) / max_edges;
}

P05_NOINLINE int p05_f16_graph_vertex_eccentricity(const p05_graph_t *g, size_t u) {
    if (!g || u >= g->num_vertices) return -1;
    int dist[P05_MAX_V];
    for (size_t i = 0; i < g->num_vertices; ++i) dist[i] = -1;
    size_t q[P05_MAX_V];
    size_t head = 0, tail = 0;
    dist[u] = 0;
    q[tail++] = u;

    int max_d = 0;
    while (head < tail) {
        size_t curr = q[head++];
        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->adj[curr][v] && dist[v] == -1) {
                dist[v] = dist[curr] + 1;
                if (dist[v] > max_d) max_d = dist[v];
                q[tail++] = v;
            }
        }
    }
    return max_d;
}
