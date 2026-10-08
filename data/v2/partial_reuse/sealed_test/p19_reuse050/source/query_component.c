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

P19_NOINLINE void p19_f05_floyd_warshall(const p19_graph_t *g, int32_t dist[P19_MAX_VERTICES][P19_MAX_VERTICES]) {
    if (!g || !dist) return;
    for (size_t i = 0; i < g->num_vertices; ++i) {
        for (size_t j = 0; j < g->num_vertices; ++j) {
            dist[i][j] = g->weights[i][j];
        }
    }
    for (size_t k = 0; k < g->num_vertices; ++k) {
        for (size_t i = 0; i < g->num_vertices; ++i) {
            for (size_t j = 0; j < g->num_vertices; ++j) {
                if (dist[i][k] < P19_INF && dist[k][j] < P19_INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

P19_NOINLINE size_t p19_f06_reconstruct_path(const int32_t *parent, size_t dst, size_t *out_path, size_t max_nodes) {
    if (!parent || !out_path || max_nodes == 0) return 0;
    size_t temp[P19_MAX_VERTICES];
    size_t count = 0;
    int32_t curr = (int32_t)dst;
    while (curr >= 0 && count < P19_MAX_VERTICES) {
        temp[count++] = (size_t)curr;
        curr = parent[curr];
    }
    size_t out_cnt = (count > max_nodes) ? max_nodes : count;
    for (size_t i = 0; i < out_cnt; ++i) {
        out_path[i] = temp[count - 1 - i];
    }
    return out_cnt;
}

P19_NOINLINE int32_t p19_f08_bidirectional_dijkstra(const p19_graph_t *g, size_t src, size_t dst) {
    if (!g || src >= g->num_vertices || dst >= g->num_vertices) return P19_INF;
    int32_t d_fwd[P19_MAX_VERTICES], d_bwd[P19_MAX_VERTICES];
    p19_f03_dijkstra(g, src, d_fwd, NULL);
    p19_f03_dijkstra(g, dst, d_bwd, NULL);
    return d_fwd[dst];
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
