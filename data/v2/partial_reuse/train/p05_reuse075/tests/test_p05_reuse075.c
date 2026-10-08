#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p05_f01_adj_init(void) {
    p05_graph_t g; p05_f01_adj_init(&g, 4); assert(g.num_vertices == 4);
}

static void test_p05_f02_adj_add_edge(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 4; assert(p05_f02_adj_add_edge(&g, 0, 1) && g.adj[0][1] == 1);
}

static void test_p05_f03_adj_has_edge(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 4; g.adj[0][1] = 1; assert(p05_f03_adj_has_edge(&g, 0, 1));
}

static void test_p05_f04_adj_in_degree(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 4; g.adj[0][1] = 1; assert(p05_f04_adj_in_degree(&g, 1) == 1);
}

static void test_p05_f05_adj_out_degree(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 4; g.adj[0][1] = 1; assert(p05_f05_adj_out_degree(&g, 0) == 1);
}

static void test_p05_f06_graph_bfs(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = 1; g.adj[1][2] = 1; size_t ord[3]; assert(p05_f06_graph_bfs(&g, 0, ord, 3) == 3);
}

static void test_p05_f07_graph_dfs_iterative(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = 1; g.adj[1][2] = 1; size_t ord[3]; assert(p05_f07_graph_dfs_iterative(&g, 0, ord, 3) == 3);
}

static void test_p05_f10_graph_bipartite_check(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; g.adj[1][0] = 1; assert(p05_f10_graph_bipartite_check(&g) == true);
}

static void test_p05_f12_graph_density(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; assert(p05_f12_graph_density(&g) > 0);
}

static void test_p05_f13_graph_isolate_count(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; assert(p05_f13_graph_isolate_count(&g) == 3);
}

static void test_p05_f14_graph_eulerian_path_check(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; assert(p05_f14_graph_eulerian_path_check(&g) == true);
}

static void test_p05_f16_graph_vertex_eccentricity(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; assert(p05_f16_graph_vertex_eccentricity(&g, 0) == 1);
}

static void test_p02_f03_heap_sift_down(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 5; p02_f03_heap_sift_down(&h, 0); assert(h.data[0] == 5);
}

static void test_p02_f05_heap_pop_min(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 1; h.data[0] = 42; int32_t val; assert(p02_f05_heap_pop_min(&h, &val) && val == 42);
}

static void test_p02_f06_heap_peek(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 1; h.data[0] = 99; int32_t val = 0; assert(p02_f06_heap_peek(&h, &val) && val == 99);
}

static void test_p02_f09_heap_replace(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 20; int32_t old_val = 0; assert(p02_f09_heap_replace(&h, 30, &old_val) && old_val == 10);
}

int main(void) {
    test_p05_f01_adj_init();
    test_p05_f02_adj_add_edge();
    test_p05_f03_adj_has_edge();
    test_p05_f04_adj_in_degree();
    test_p05_f05_adj_out_degree();
    test_p05_f06_graph_bfs();
    test_p05_f07_graph_dfs_iterative();
    test_p05_f10_graph_bipartite_check();
    test_p05_f12_graph_density();
    test_p05_f13_graph_isolate_count();
    test_p05_f14_graph_eulerian_path_check();
    test_p05_f16_graph_vertex_eccentricity();
    test_p02_f03_heap_sift_down();
    test_p02_f05_heap_pop_min();
    test_p02_f06_heap_peek();
    test_p02_f09_heap_replace();
    printf("PASS: test_p05_reuse075 passed.\n");
    return 0;
}
