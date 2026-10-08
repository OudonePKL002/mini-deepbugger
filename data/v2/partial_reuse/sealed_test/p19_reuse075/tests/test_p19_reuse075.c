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

static void test_p19_f07_has_negative_cycle(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(!p19_f07_has_negative_cycle(&g));
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

static void test_p19_f14_count_connected_components(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f14_count_connected_components(&g) == 1);
}

static void test_p19_f16_sp_clear(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; p19_f16_sp_clear(&g); assert(g.weights[0][1] == P19_INF);
}

static void test_p18_f01_vec_mean(void) {
    double d[3] = {1, 2, 3}; assert(fabs(p18_f01_vec_mean(d, 3) - 2.0) < 1e-6);
}

static void test_p18_f02_vec_variance(void) {
    double d[3] = {1, 2, 3}; assert(p18_f02_vec_variance(d, 3) > 0.0);
}

static void test_p18_f06_vec_l1_norm(void) {
    double d[2] = {-1, 2}; assert(p18_f06_vec_l1_norm(d, 2) == 3.0);
}

static void test_p18_f16_vec_scale_add(void) {
    double s[2] = {1, 2}; double d[2]; p18_f16_vec_scale_add(d, s, 2.0, 1.0, 2); assert(d[0] == 3.0 && d[1] == 5.0);
}

int main(void) {
    test_p19_f01_sp_init();
    test_p19_f02_sp_add_edge();
    test_p19_f03_dijkstra();
    test_p19_f04_bellman_ford();
    test_p19_f05_floyd_warshall();
    test_p19_f07_has_negative_cycle();
    test_p19_f09_prim_mst();
    test_p19_f10_kruskal_mst();
    test_p19_f11_eccentricity();
    test_p19_f12_graph_diameter();
    test_p19_f14_count_connected_components();
    test_p19_f16_sp_clear();
    test_p18_f01_vec_mean();
    test_p18_f02_vec_variance();
    test_p18_f06_vec_l1_norm();
    test_p18_f16_vec_scale_add();
    printf("PASS: test_p19_reuse075 passed.\n");
    return 0;
}
