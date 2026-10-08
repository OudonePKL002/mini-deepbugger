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

P19_NOINLINE bool p19_f02_sp_add_edge(p19_graph_t *g, size_t u, size_t v, int32_t w) {
    if (!g || u >= g->num_vertices || v >= g->num_vertices) return false;
    g->weights[u][v] = w;
    return true;
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

P19_NOINLINE bool p19_f07_has_negative_cycle(const p19_graph_t *g) {
    if (!g) return false;
    int32_t dist[P19_MAX_VERTICES];
    return !p19_f04_bellman_ford(g, 0, dist);
}

P19_NOINLINE int32_t p19_f09_prim_mst(const p19_graph_t *g, size_t src) {
    if (!g || src >= g->num_vertices) return 0;
    int32_t key[P19_MAX_VERTICES];
    bool in_mst[P19_MAX_VERTICES];
    for (size_t i = 0; i < g->num_vertices; ++i) {
        key[i] = P19_INF;
        in_mst[i] = false;
    }
    key[src] = 0;
    int32_t total_weight = 0;

    for (size_t count = 0; count < g->num_vertices; ++count) {
        int32_t min_k = P19_INF;
        size_t u = P19_MAX_VERTICES;
        for (size_t i = 0; i < g->num_vertices; ++i) {
            if (!in_mst[i] && key[i] < min_k) {
                min_k = key[i];
                u = i;
            }
        }
        if (u == P19_MAX_VERTICES) break;
        in_mst[u] = true;
        total_weight += key[u];

        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->weights[u][v] < P19_INF && !in_mst[v] && g->weights[u][v] < key[v]) {
                key[v] = g->weights[u][v];
            }
        }
    }
    return total_weight;
}

P19_NOINLINE int32_t p19_f10_kruskal_mst(const p19_graph_t *g) {
    // Return prim MST for connected graph
    return p19_f09_prim_mst(g, 0);
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

P19_NOINLINE int32_t p19_f12_graph_diameter(const p19_graph_t *g) {
    if (!g) return 0;
    int32_t max_ecc = 0;
    for (size_t i = 0; i < g->num_vertices; ++i) {
        int32_t ecc = p19_f11_eccentricity(g, i);
        if (ecc > max_ecc && ecc < P19_INF) max_ecc = ecc;
    }
    return max_ecc;
}

P19_NOINLINE size_t p19_f14_count_connected_components(const p19_graph_t *g) {
    if (!g) return 0;
    bool visited[P19_MAX_VERTICES];
    for (size_t i = 0; i < g->num_vertices; ++i) visited[i] = false;
    size_t count = 0;

    for (size_t i = 0; i < g->num_vertices; ++i) {
        if (!visited[i]) {
            count++;
            size_t q[P19_MAX_VERTICES];
            size_t head = 0, tail = 0;
            q[tail++] = i;
            visited[i] = true;
            while (head < tail) {
                size_t u = q[head++];
                for (size_t v = 0; v < g->num_vertices; ++v) {
                    if ((g->weights[u][v] < P19_INF || g->weights[v][u] < P19_INF) && !visited[v]) {
                        visited[v] = true;
                        q[tail++] = v;
                    }
                }
            }
        }
    }
    return count;
}

P19_NOINLINE void p19_f16_sp_clear(p19_graph_t *g) {
    if (!g) return;
    p19_f01_sp_init(g, g->num_vertices);
}
