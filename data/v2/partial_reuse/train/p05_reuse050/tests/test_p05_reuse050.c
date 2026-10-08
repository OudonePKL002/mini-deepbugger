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

static void test_p05_f06_graph_bfs(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = 1; g.adj[1][2] = 1; size_t ord[3]; assert(p05_f06_graph_bfs(&g, 0, ord, 3) == 3);
}

static void test_p05_f07_graph_dfs_iterative(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = 1; g.adj[1][2] = 1; size_t ord[3]; assert(p05_f07_graph_dfs_iterative(&g, 0, ord, 3) == 3);
}

static void test_p05_f10_graph_bipartite_check(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; g.adj[0][1] = 1; g.adj[1][0] = 1; assert(p05_f10_graph_bipartite_check(&g) == true);
}

static void test_p05_f15_graph_subgraph_induced(void) {
    p05_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 3; g.adj[0][1] = 1; bool mask[3] = {true, true, false}; p05_graph_t sub; p05_f15_graph_subgraph_induced(&g, mask, &sub); assert(sub.num_vertices == 3);
}

static void test_p02_f01_heap_init(void) {
    p02_heap_t h; p02_f01_heap_init(&h); assert(h.size == 0 && h.capacity == P02_MAX_CAP);
}

static void test_p02_f02_heap_sift_up(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 5; p02_f02_heap_sift_up(&h, 1); assert(h.data[0] == 5);
}

static void test_p02_f03_heap_sift_down(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 5; p02_f03_heap_sift_down(&h, 0); assert(h.data[0] == 5);
}

static void test_p02_f07_heapify_array(void) {
    int32_t arr[3] = {3, 1, 2}; p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; p02_f07_heapify_array(&h, arr, 3); assert(h.size == 3);
}

static void test_p02_f10_heap_delete_at(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 3; h.data[0] = 10; h.data[1] = 20; h.data[2] = 30; assert(p02_f10_heap_delete_at(&h, 1) && h.size == 2);
}

static void test_p02_f13_heap_merge(void) {
    p02_heap_t h1, h2; memset(&h1, 0, sizeof(h1)); memset(&h2, 0, sizeof(h2)); h1.capacity = P02_MAX_CAP; h2.capacity = P02_MAX_CAP; h1.size = 1; h1.data[0] = 10; h2.size = 1; h2.data[0] = 5; assert(p02_f13_heap_merge(&h1, &h2) && h1.size == 2 && h1.data[0] == 5);
}

static void test_p02_f14_heap_is_valid(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 3; h.data[0] = 10; h.data[1] = 20; h.data[2] = 30; assert(p02_f14_heap_is_valid(&h));
}

static void test_p02_f16_heap_clear(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 5; p02_f16_heap_clear(&h); assert(h.size == 0);
}

int main(void) {
    test_p05_f01_adj_init();
    test_p05_f02_adj_add_edge();
    test_p05_f03_adj_has_edge();
    test_p05_f04_adj_in_degree();
    test_p05_f06_graph_bfs();
    test_p05_f07_graph_dfs_iterative();
    test_p05_f10_graph_bipartite_check();
    test_p05_f15_graph_subgraph_induced();
    test_p02_f01_heap_init();
    test_p02_f02_heap_sift_up();
    test_p02_f03_heap_sift_down();
    test_p02_f07_heapify_array();
    test_p02_f10_heap_delete_at();
    test_p02_f13_heap_merge();
    test_p02_f14_heap_is_valid();
    test_p02_f16_heap_clear();
    printf("PASS: test_p05_reuse050 passed.\n");
    return 0;
}
