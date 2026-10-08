#include "query_component.h"
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

P05_NOINLINE bool p05_f03_adj_has_edge(const p05_graph_t *g, size_t u, size_t v) {
    if (!g || u >= g->num_vertices || v >= g->num_vertices) return false;
    return g->adj[u][v] == 1;
}

P05_NOINLINE size_t p05_f04_adj_in_degree(const p05_graph_t *g, size_t u) {
    if (!g || u >= g->num_vertices) return 0;
    size_t deg = 0;
    for (size_t r = 0; r < g->num_vertices; ++r) {
        if (g->adj[r][u]) deg++;
    }
    return deg;
}

P05_NOINLINE size_t p05_f06_graph_bfs(const p05_graph_t *g, size_t start, size_t *order, size_t max_order) {
    if (!g || !order || start >= g->num_vertices || max_order == 0) return 0;
    bool visited[P05_MAX_V] = {false};
    size_t queue[P05_MAX_V];
    size_t q_head = 0, q_tail = 0;
    size_t count = 0;

    visited[start] = true;
    queue[q_tail++] = start;

    while (q_head < q_tail && count < max_order) {
        size_t u = queue[q_head++];
        order[count++] = u;
        for (size_t v = 0; v < g->num_vertices; ++v) {
            if (g->adj[u][v] && !visited[v]) {
                visited[v] = true;
                queue[q_tail++] = v;
            }
        }
    }
    return count;
}

P05_NOINLINE size_t p05_f07_graph_dfs_iterative(const p05_graph_t *g, size_t start, size_t *order, size_t max_order) {
    if (!g || !order || start >= g->num_vertices || max_order == 0) return 0;
    bool visited[P05_MAX_V] = {false};
    size_t stack[P05_MAX_V * 2];
    size_t top = 0;
    size_t count = 0;

    stack[top++] = start;
    while (top > 0 && count < max_order) {
        size_t u = stack[--top];
        if (!visited[u]) {
            visited[u] = true;
            order[count++] = u;
            for (int v = (int)g->num_vertices - 1; v >= 0; --v) {
                if (g->adj[u][v] && !visited[v]) {
                    stack[top++] = (size_t)v;
                }
            }
        }
    }
    return count;
}

P05_NOINLINE bool p05_f10_graph_bipartite_check(const p05_graph_t *g) {
    if (!g) return false;
    int color[P05_MAX_V];
    for (size_t i = 0; i < g->num_vertices; ++i) color[i] = -1;

    for (size_t i = 0; i < g->num_vertices; ++i) {
        if (color[i] == -1) {
            size_t q[P05_MAX_V];
            size_t head = 0, tail = 0;
            color[i] = 0;
            q[tail++] = i;
            while (head < tail) {
                size_t u = q[head++];
                for (size_t v = 0; v < g->num_vertices; ++v) {
                    if (g->adj[u][v] || g->adj[v][u]) {
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

P05_NOINLINE void p05_f15_graph_subgraph_induced(const p05_graph_t *g, const bool *keep_mask, p05_graph_t *sub) {
    if (!g || !keep_mask || !sub) return;
    p05_f01_adj_init(sub, g->num_vertices);
    for (size_t r = 0; r < g->num_vertices; ++r) {
        if (!keep_mask[r]) continue;
        for (size_t c = 0; c < g->num_vertices; ++c) {
            if (keep_mask[c] && g->adj[r][c]) {
                sub->adj[r][c] = 1;
            }
        }
    }
}
