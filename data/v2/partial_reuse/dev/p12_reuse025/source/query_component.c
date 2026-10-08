#include "query_component.h"
#include <string.h>
#include <math.h>

#ifndef INF_DIST
#define INF_DIST 100000000
#endif

P12_NOINLINE bool p12_f02_dag_add_edge(p12_dag_t *g, size_t u, size_t v, int32_t weight) {
    if (!g || u >= g->num_vertices || v >= g->num_vertices || u == v) return false;
    g->adj[u][v] = 1;
    g->weights[u][v] = weight;
    return true;
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
