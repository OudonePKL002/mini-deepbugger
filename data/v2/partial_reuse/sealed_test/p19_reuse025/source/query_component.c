#include "query_component.h"
#include <string.h>
#include <math.h>

P19_NOINLINE void p19_f01_sp_init(p19_graph_t *g, size_t n) {
    if (!g) return;
    g->num_vertices = (n > P19_MAX_VERTICES) ? P19_MAX_VERTICES : n;
    for (size_t i = 0; i < P19_MAX_VERTICES; ++i) {
        for (size_t j = 0; j < P19_MAX_VERTICES; ++j) {
            g->weights[i][j] = (i == j) ? 0 : P19_INF;
        }
    }
}

P19_NOINLINE void p19_f03_dijkstra(const p19_graph_t *g, size_t src, int32_t *dist, int32_t *parent) {
    if (!g || !dist || src >= g->num_vertices) return;
    bool visited[P19_MAX_VERTICES];
    for (size_t i = 0; i < g->num_vertices; ++i) {
        dist[i] = P19_INF;
        visited[i] = false;
        if (parent) parent[i] = -1;
    }
    dist[src] = 0;

    for (size_t step = 0; step < g->num_vertices; ++step) {
        int32_t min_d = P19_INF;
        size_t u = P19_MAX_VERTICES;
        for (size_t i = 0; i < g->num_vertices; ++i) {
            if (!visited[i] && dist[i] < min_d) {
                min_d = dist[i];
                u = i;
            }
        }
        if (u == P19_MAX_VERTICES) break;
        visited[u] = true;

        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (!visited[v] && g->weights[u][v] < P19_INF) {
                int32_t new_d = dist[u] + g->weights[u][v];
                if (new_d < dist[v]) {
                    dist[v] = new_d;
                    if (parent) parent[v] = (int32_t)u;
                }
            }
        }
    }
}

P19_NOINLINE int32_t p19_f11_eccentricity(const p19_graph_t *g, size_t v) {
    if (!g || v >= g->num_vertices) return P19_INF;
    int32_t dist[P19_MAX_VERTICES];
    p19_f03_dijkstra(g, v, dist, NULL);
    int32_t max_d = 0;
    for (size_t i = 0; i < g->num_vertices; ++i) {
        if (dist[i] > max_d && dist[i] < P19_INF) max_d = dist[i];
    }
    return max_d;
}

P19_NOINLINE void p19_f16_sp_clear(p19_graph_t *g) {
    if (!g) return;
    p19_f01_sp_init(g, g->num_vertices);
}
