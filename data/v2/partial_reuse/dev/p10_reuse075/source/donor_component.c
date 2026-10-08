#include "donor_component.h"
#include <string.h>
#include <math.h>

#ifndef INF_DIST
#define INF_DIST 100000000
#endif

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

P12_NOINLINE void p12_f03_dag_compute_indegrees(const p12_dag_t *g, size_t *in_deg) {
    if (!g || !in_deg) return;
    for (size_t i = 0; i < g->num_vertices; ++i) in_deg[i] = 0;
    for (size_t u = 0; u < g->num_vertices; ++u) {
        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->adj[u][v]) in_deg[v]++;
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
