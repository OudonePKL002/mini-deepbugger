#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p19_f01_sp_init(void) {
    p19_graph_t g; p19_f01_sp_init(&g, 4); assert(g.num_vertices == 4);
}

static void test_p19_f02_sp_add_edge(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; assert(p19_f02_sp_add_edge(&g, 0, 1, 5) && g.weights[0][1] == 5);
}

static void test_p19_f03_dijkstra(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t d[2], p[2]; p19_f03_dijkstra(&g, 0, d, p); assert(d[1] == 5);
}

static void test_p19_f04_bellman_ford(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t d[2]; assert(p19_f04_bellman_ford(&g, 0, d) && d[1] == 5);
}

static void test_p19_f05_floyd_warshall(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; int32_t fw[P19_MAX_VERTICES][P19_MAX_VERTICES]; p19_f05_floyd_warshall(&g, fw); assert(fw[0][1] == 5);
}

static void test_p19_f06_reconstruct_path(void) {
    int32_t p[2] = {-1, 0}; size_t path[2]; assert(p19_f06_reconstruct_path(p, 1, path, 2) == 2);
}

static void test_p19_f07_has_negative_cycle(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(!p19_f07_has_negative_cycle(&g));
}

static void test_p19_f08_bidirectional_dijkstra(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f08_bidirectional_dijkstra(&g, 0, 1) == 5);
}

static void test_p19_f09_prim_mst(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; g.weights[1][0] = 5; assert(p19_f09_prim_mst(&g, 0) == 5);
}

static void test_p19_f10_kruskal_mst(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f10_kruskal_mst(&g) == 5);
}

static void test_p19_f11_eccentricity(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f11_eccentricity(&g, 0) == 5);
}

static void test_p19_f12_graph_diameter(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f12_graph_diameter(&g) >= 5);
}

static void test_p19_f13_graph_radius(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f13_graph_radius(&g) >= 0);
}

static void test_p19_f14_count_connected_components(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f14_count_connected_components(&g) == 1);
}

static void test_p19_f15_is_bipartite(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(!p19_f15_is_bipartite(&g));
}

static void test_p19_f16_sp_clear(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; p19_f16_sp_clear(&g); assert(g.weights[0][1] == P19_INF);
}

int main(void) {
    test_p19_f01_sp_init();
    test_p19_f02_sp_add_edge();
    test_p19_f03_dijkstra();
    test_p19_f04_bellman_ford();
    test_p19_f05_floyd_warshall();
    test_p19_f06_reconstruct_path();
    test_p19_f07_has_negative_cycle();
    test_p19_f08_bidirectional_dijkstra();
    test_p19_f09_prim_mst();
    test_p19_f10_kruskal_mst();
    test_p19_f11_eccentricity();
    test_p19_f12_graph_diameter();
    test_p19_f13_graph_radius();
    test_p19_f14_count_connected_components();
    test_p19_f15_is_bipartite();
    test_p19_f16_sp_clear();
    printf("PASS: test_p20_reuse000 passed.\n");
    return 0;
}
