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

static void test_p19_f08_bidirectional_dijkstra(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f08_bidirectional_dijkstra(&g, 0, 1) == 5);
}

static void test_p19_f11_eccentricity(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(p19_f11_eccentricity(&g, 0) == 5);
}

static void test_p19_f15_is_bipartite(void) {
    p19_graph_t g; memset(&g, 0, sizeof(g)); g.num_vertices = 2; for(size_t i=0;i<P19_MAX_VERTICES;i++) for(size_t j=0;j<P19_MAX_VERTICES;j++) g.weights[i][j]=(i==j)?0:P19_INF; g.weights[0][1] = 5; assert(!p19_f15_is_bipartite(&g));
}

static void test_p18_f01_vec_mean(void) {
    double d[3] = {1, 2, 3}; assert(fabs(p18_f01_vec_mean(d, 3) - 2.0) < 1e-6);
}

static void test_p18_f04_vec_min_max(void) {
    double d[3] = {1, 5, 2}; double mn, mx; p18_f04_vec_min_max(d, 3, &mn, &mx); assert(mn == 1.0 && mx == 5.0);
}

static void test_p18_f05_vec_dot_product(void) {
    double a[2] = {1, 2}; double b[2] = {3, 4}; assert(p18_f05_vec_dot_product(a, b, 2) == 11.0);
}

static void test_p18_f07_vec_l2_norm(void) {
    double d[2] = {3, 4}; assert(fabs(p18_f07_vec_l2_norm(d, 2) - 5.0) < 1e-6);
}

static void test_p18_f09_vec_normalize(void) {
    double d[2] = {3, 4}; assert(p18_f09_vec_normalize(d, 2) && fabs(d[0] - 0.6) < 1e-4);
}

static void test_p18_f10_vec_pearson_corr(void) {
    double a[3] = {1, 2, 3}; double b[3] = {2, 4, 6}; assert(fabs(p18_f10_vec_pearson_corr(a, b, 3) - 1.0) < 1e-4);
}

static void test_p18_f14_vec_quantile(void) {
    double a[3] = {1, 2, 3}; assert(p18_f14_vec_quantile(a, 3, 0.5) >= 1.0);
}

static void test_p18_f16_vec_scale_add(void) {
    double s[2] = {1, 2}; double d[2]; p18_f16_vec_scale_add(d, s, 2.0, 1.0, 2); assert(d[0] == 3.0 && d[1] == 5.0);
}

int main(void) {
    test_p19_f01_sp_init();
    test_p19_f03_dijkstra();
    test_p19_f04_bellman_ford();
    test_p19_f05_floyd_warshall();
    test_p19_f06_reconstruct_path();
    test_p19_f08_bidirectional_dijkstra();
    test_p19_f11_eccentricity();
    test_p19_f15_is_bipartite();
    test_p18_f01_vec_mean();
    test_p18_f04_vec_min_max();
    test_p18_f05_vec_dot_product();
    test_p18_f07_vec_l2_norm();
    test_p18_f09_vec_normalize();
    test_p18_f10_vec_pearson_corr();
    test_p18_f14_vec_quantile();
    test_p18_f16_vec_scale_add();
    printf("PASS: test_p19_reuse050 passed.\n");
    return 0;
}
