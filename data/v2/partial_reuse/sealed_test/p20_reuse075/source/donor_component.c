#include "donor_component.h"
#include <string.h>
#include <math.h>

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

P19_NOINLINE bool p19_f04_bellman_ford(const p19_graph_t *g, size_t src, int32_t *dist) {
    if (!g || !dist || src >= g->num_vertices) return false;
    for (size_t i = 0; i < g->num_vertices; ++i) dist[i] = P19_INF;
    dist[src] = 0;

    for (size_t k = 1; k < g->num_vertices; ++k) {
        for (size_t u = 0; u < g->num_vertices; ++u) {
            if (dist[u] == P19_INF) continue;
            for (size_t v = 0; v < g->num_vertices; ++v) {
                if (g->weights[u][v] < P19_INF) {
                    if (dist[u] + g->weights[u][v] < dist[v]) {
                        dist[v] = dist[u] + g->weights[u][v];
                    }
                }
            }
        }
    }
    // Check negative cycle
    for (size_t u = 0; u < g->num_vertices; ++u) {
        if (dist[u] == P19_INF) continue;
        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->weights[u][v] < P19_INF) {
                if (dist[u] + g->weights[u][v] < dist[v]) return false;
            }
        }
    }
    return true;
}

P19_NOINLINE bool p19_f07_has_negative_cycle(const p19_graph_t *g) {
    if (!g) return false;
    int32_t dist[P19_MAX_VERTICES];
    return !p19_f04_bellman_ford(g, 0, dist);
}

P19_NOINLINE bool p19_f15_is_bipartite(const p19_graph_t *g) {
    if (!g) return false;
    int color[P19_MAX_VERTICES];
    for (size_t i = 0; i < g->num_vertices; ++i) color[i] = -1;

    for (size_t start = 0; start < g->num_vertices; ++start) {
        if (color[start] == -1) {
            color[start] = 1;
            size_t q[P19_MAX_VERTICES];
            size_t head = 0, tail = 0;
            q[tail++] = start;
            while (head < tail) {
                size_t u = q[head++];
                for (size_t v = 0; v < g->num_vertices; ++v) {
                    if (g->weights[u][v] < P19_INF) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u];
                            q[tail++] = v;
                        } else if (color[v] == color[u]) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}
