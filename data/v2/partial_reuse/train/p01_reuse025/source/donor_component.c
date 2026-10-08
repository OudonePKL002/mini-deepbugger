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

P05_NOINLINE size_t p05_f05_adj_out_degree(const p05_graph_t *g, size_t u) {
    if (!g || u >= g->num_vertices) return 0;
    size_t deg = 0;
    for (size_t c = 0; c < g->num_vertices; ++c) {
        if (g->adj[u][c]) deg++;
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

P05_NOINLINE bool p05_f08_graph_has_cycle_directed(const p05_graph_t *g) {
    if (!g) return false;
    uint8_t color[P05_MAX_V] = {0}; // 0=unvisited, 1=visiting, 2=done

    for (size_t i = 0; i < g->num_vertices; ++i) {
        if (color[i] == 0) {
            size_t stack[P05_MAX_V];
            size_t edge_idx[P05_MAX_V];
            size_t top = 0;

            stack[top] = i;
            edge_idx[top] = 0;
            color[i] = 1;
            top++;

            while (top > 0) {
                size_t curr = stack[top - 1];
                size_t e = edge_idx[top - 1];
                bool advanced = false;

                while (e < g->num_vertices) {
                    if (g->adj[curr][e]) {
                        if (color[e] == 1) return true;
                        if (color[e] == 0) {
                            edge_idx[top - 1] = e + 1;
                            color[e] = 1;
                            stack[top] = e;
                            edge_idx[top] = 0;
                            top++;
                            advanced = true;
                            break;
                        }
                    }
                    e++;
                }
                if (!advanced) {
                    color[curr] = 2;
                    top--;
                }
            }
        }
    }
    return false;
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

P05_NOINLINE void p05_f11_graph_transitive_closure(const p05_graph_t *g, uint8_t reach[P05_MAX_V][P05_MAX_V]) {
    if (!g || !reach) return;
    for (size_t i = 0; i < g->num_vertices; ++i) {
        for (size_t j = 0; j < g->num_vertices; ++j) {
            reach[i][j] = (i == j || g->adj[i][j]) ? 1 : 0;
        }
    }
    for (size_t k = 0; k < g->num_vertices; ++k) {
        for (size_t i = 0; i < g->num_vertices; ++i) {
            for (size_t j = 0; j < g->num_vertices; ++j) {
                reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]);
            }
        }
    }
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

P05_NOINLINE size_t p05_f13_graph_isolate_count(const p05_graph_t *g) {
    if (!g) return 0;
    size_t count = 0;
    for (size_t i = 0; i < g->num_vertices; ++i) {
        if (p05_f04_adj_in_degree(g, i) == 0 && p05_f05_adj_out_degree(g, i) == 0) {
            count++;
        }
    }
    return count;
}

P05_NOINLINE bool p05_f14_graph_eulerian_path_check(const p05_graph_t *g) {
    if (!g || g->num_vertices == 0) return false;
    size_t start_nodes = 0;
    size_t end_nodes = 0;
    for (size_t i = 0; i < g->num_vertices; ++i) {
        size_t out_d = p05_f05_adj_out_degree(g, i);
        size_t in_d = p05_f04_adj_in_degree(g, i);
        if (out_d > in_d + 1 || in_d > out_d + 1) return false;
        if (out_d == in_d + 1) start_nodes++;
        if (in_d == out_d + 1) end_nodes++;
    }
    return (start_nodes == 0 && end_nodes == 0) || (start_nodes == 1 && end_nodes == 1);
}
