#include "query_component.h"
#include <string.h>
#include <math.h>

P05_NOINLINE size_t p05_f05_adj_out_degree(const p05_graph_t *g, size_t u) {
    if (!g || u >= g->num_vertices) return 0;
    size_t deg = 0;
    for (size_t c = 0; c < g->num_vertices; ++c) {
        if (g->adj[u][c]) deg++;
    }
    return deg;
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
