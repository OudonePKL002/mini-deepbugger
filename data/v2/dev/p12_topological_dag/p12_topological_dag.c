#include "p12_topological_dag.h"
#include <string.h>

#define INF_DIST 100000000

P12_NOINLINE void p12_f01_dag_init(p12_dag_t *g, size_t n) {
    if (!g) return;
    g->num_vertices = (n > P12_MAX_VERTICES) ? P12_MAX_VERTICES : n;
    for (size_t i = 0; i < P12_MAX_VERTICES; ++i) {
        for (size_t j = 0; j < P12_MAX_VERTICES; ++j) {
            g->adj[i][j] = 0;
            g->weights[i][j] = 0;
        }
    }
}

P12_NOINLINE bool p12_f02_dag_add_edge(p12_dag_t *g, size_t u, size_t v, int32_t weight) {
    if (!g || u >= g->num_vertices || v >= g->num_vertices || u == v) return false;
    g->adj[u][v] = 1;
    g->weights[u][v] = weight;
    return true;
}

P12_NOINLINE void p12_f03_dag_compute_indegrees(const p12_dag_t *g, size_t *in_deg) {
    if (!g || !in_deg) return;
    for (size_t i = 0; i < g->num_vertices; ++i) in_deg[i] = 0;
    for (size_t u = 0; u < g->num_vertices; ++u) {
        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->adj[u][v]) in_deg[v]++;
        }
    }
}

P12_NOINLINE void p12_f04_dag_compute_outdegrees(const p12_dag_t *g, size_t *out_deg) {
    if (!g || !out_deg) return;
    for (size_t i = 0; i < g->num_vertices; ++i) out_deg[i] = 0;
    for (size_t u = 0; u < g->num_vertices; ++u) {
        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->adj[u][v]) out_deg[u]++;
        }
    }
}

P12_NOINLINE bool p12_f05_dag_kahn_toposort(const p12_dag_t *g, size_t *order, size_t *out_len) {
    if (!g || !order) return false;
    size_t in_deg[P12_MAX_VERTICES];
    p12_f03_dag_compute_indegrees(g, in_deg);

    size_t queue[P12_MAX_VERTICES];
    size_t head = 0, tail = 0;
    for (size_t i = 0; i < g->num_vertices; ++i) {
        if (in_deg[i] == 0) queue[tail++] = i;
    }
    size_t count = 0;
    while (head < tail) {
        size_t u = queue[head++];
        order[count++] = u;
        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->adj[u][v]) {
                if (--in_deg[v] == 0) queue[tail++] = v;
            }
        }
    }
    if (out_len) *out_len = count;
    return count == g->num_vertices;
}

P12_NOINLINE bool p12_f06_dag_has_cycle(const p12_dag_t *g) {
    if (!g) return false;
    size_t order[P12_MAX_VERTICES];
    size_t len = 0;
    return !p12_f05_dag_kahn_toposort(g, order, &len);
}

P12_NOINLINE int32_t p12_f07_dag_longest_path(const p12_dag_t *g, size_t src, size_t dst) {
    if (!g || src >= g->num_vertices || dst >= g->num_vertices) return -INF_DIST;
    size_t order[P12_MAX_VERTICES];
    size_t len = 0;
    if (!p12_f05_dag_kahn_toposort(g, order, &len)) return -INF_DIST;

    int32_t dist[P12_MAX_VERTICES];
    for (size_t i = 0; i < g->num_vertices; ++i) dist[i] = -INF_DIST;
    dist[src] = 0;

    for (size_t i = 0; i < len; ++i) {
        size_t u = order[i];
        if (dist[u] != -INF_DIST) {
            for (size_t v = 0; v < g->num_vertices; ++v) {
                if (g->adj[u][v]) {
                    int32_t w = g->weights[u][v];
                    if (dist[u] + w > dist[v]) {
                        dist[v] = dist[u] + w;
                    }
                }
            }
        }
    }
    return dist[dst];
}

P12_NOINLINE int32_t p12_f08_dag_shortest_path(const p12_dag_t *g, size_t src, size_t dst) {
    if (!g || src >= g->num_vertices || dst >= g->num_vertices) return INF_DIST;
    size_t order[P12_MAX_VERTICES];
    size_t len = 0;
    if (!p12_f05_dag_kahn_toposort(g, order, &len)) return INF_DIST;

    int32_t dist[P12_MAX_VERTICES];
    for (size_t i = 0; i < g->num_vertices; ++i) dist[i] = INF_DIST;
    dist[src] = 0;

    for (size_t i = 0; i < len; ++i) {
        size_t u = order[i];
        if (dist[u] != INF_DIST) {
            for (size_t v = 0; v < g->num_vertices; ++v) {
                if (g->adj[u][v]) {
                    int32_t w = g->weights[u][v];
                    if (dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                    }
                }
            }
        }
    }
    return dist[dst];
}

P12_NOINLINE size_t p12_f09_dag_count_ancestors(const p12_dag_t *g, size_t target) {
    if (!g || target >= g->num_vertices) return 0;
    bool visited[P12_MAX_VERTICES];
    for (size_t i = 0; i < g->num_vertices; ++i) visited[i] = false;
    size_t queue[P12_MAX_VERTICES];
    size_t head = 0, tail = 0;
    queue[tail++] = target;
    visited[target] = true;
    size_t count = 0;

    while (head < tail) {
        size_t curr = queue[head++];
        for (size_t u = 0; u < g->num_vertices; ++u) {
            if (g->adj[u][curr] && !visited[u]) {
                visited[u] = true;
                queue[tail++] = u;
                count++;
            }
        }
    }
    return count;
}

P12_NOINLINE size_t p12_f10_dag_count_descendants(const p12_dag_t *g, size_t target) {
    if (!g || target >= g->num_vertices) return 0;
    bool visited[P12_MAX_VERTICES];
    for (size_t i = 0; i < g->num_vertices; ++i) visited[i] = false;
    size_t queue[P12_MAX_VERTICES];
    size_t head = 0, tail = 0;
    queue[tail++] = target;
    visited[target] = true;
    size_t count = 0;

    while (head < tail) {
        size_t curr = queue[head++];
        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->adj[curr][v] && !visited[v]) {
                visited[v] = true;
                queue[tail++] = v;
                count++;
            }
        }
    }
    return count;
}

P12_NOINLINE void p12_f11_dag_assign_levels(const p12_dag_t *g, size_t *levels) {
    if (!g || !levels) return;
    size_t order[P12_MAX_VERTICES];
    size_t len = 0;
    p12_f05_dag_kahn_toposort(g, order, &len);
    for (size_t i = 0; i < g->num_vertices; ++i) levels[i] = 0;

    for (size_t i = 0; i < len; ++i) {
        size_t u = order[i];
        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->adj[u][v] && levels[u] + 1 > levels[v]) {
                levels[v] = levels[u] + 1;
            }
        }
    }
}

P12_NOINLINE void p12_f12_dag_transitive_reduction(p12_dag_t *g) {
    if (!g) return;
    for (size_t j = 0; j < g->num_vertices; ++j) {
        for (size_t i = 0; i < g->num_vertices; ++i) {
            if (g->adj[i][j]) {
                for (size_t k = 0; k < g->num_vertices; ++k) {
                    if (g->adj[j][k]) {
                        g->adj[i][k] = 0; // eliminate transitive shortcut
                    }
                }
            }
        }
    }
}

P12_NOINLINE size_t p12_f13_dag_find_sources(const p12_dag_t *g, size_t *sources, size_t max_s) {
    if (!g || !sources || max_s == 0) return 0;
    size_t in_deg[P12_MAX_VERTICES];
    p12_f03_dag_compute_indegrees(g, in_deg);
    size_t count = 0;
    for (size_t i = 0; i < g->num_vertices && count < max_s; ++i) {
        if (in_deg[i] == 0) sources[count++] = i;
    }
    return count;
}

P12_NOINLINE size_t p12_f14_dag_find_sinks(const p12_dag_t *g, size_t *sinks, size_t max_s) {
    if (!g || !sinks || max_s == 0) return 0;
    size_t out_deg[P12_MAX_VERTICES];
    p12_f04_dag_compute_outdegrees(g, out_deg);
    size_t count = 0;
    for (size_t i = 0; i < g->num_vertices && count < max_s; ++i) {
        if (out_deg[i] == 0) sinks[count++] = i;
    }
    return count;
}

P12_NOINLINE bool p12_f15_dag_is_reachable(const p12_dag_t *g, size_t src, size_t dst) {
    if (!g || src >= g->num_vertices || dst >= g->num_vertices) return false;
    if (src == dst) return true;
    bool visited[P12_MAX_VERTICES];
    for (size_t i = 0; i < g->num_vertices; ++i) visited[i] = false;
    size_t queue[P12_MAX_VERTICES];
    size_t head = 0, tail = 0;
    queue[tail++] = src;
    visited[src] = true;

    while (head < tail) {
        size_t u = queue[head++];
        if (u == dst) return true;
        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->adj[u][v] && !visited[v]) {
                visited[v] = true;
                queue[tail++] = v;
            }
        }
    }
    return false;
}

P12_NOINLINE void p12_f16_dag_clear(p12_dag_t *g) {
    if (!g) return;
    p12_f01_dag_init(g, g->num_vertices);
}
