#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p18_f01_vec_mean(void) {
    double d[3] = {1, 2, 3}; assert(fabs(p18_f01_vec_mean(d, 3) - 2.0) < 1e-6);
}

static void test_p18_f02_vec_variance(void) {
    double d[3] = {1, 2, 3}; assert(p18_f02_vec_variance(d, 3) > 0.0);
}

static void test_p18_f03_vec_stddev(void) {
    double d[3] = {1, 2, 3}; assert(p18_f03_vec_stddev(d, 3) > 0.0);
}

static void test_p18_f04_vec_min_max(void) {
    double d[3] = {1, 5, 2}; double mn, mx; p18_f04_vec_min_max(d, 3, &mn, &mx); assert(mn == 1.0 && mx == 5.0);
}

static void test_p18_f05_vec_dot_product(void) {
    double a[2] = {1, 2}; double b[2] = {3, 4}; assert(p18_f05_vec_dot_product(a, b, 2) == 11.0);
}

static void test_p18_f06_vec_l1_norm(void) {
    double d[2] = {-1, 2}; assert(p18_f06_vec_l1_norm(d, 2) == 3.0);
}

static void test_p18_f07_vec_l2_norm(void) {
    double d[2] = {3, 4}; assert(fabs(p18_f07_vec_l2_norm(d, 2) - 5.0) < 1e-6);
}

static void test_p18_f08_vec_cosine_similarity(void) {
    double a[2] = {1, 0}; double b[2] = {1, 0}; assert(fabs(p18_f08_vec_cosine_similarity(a, b, 2) - 1.0) < 1e-4);
}

static void test_p18_f09_vec_normalize(void) {
    double d[2] = {3, 4}; assert(p18_f09_vec_normalize(d, 2) && fabs(d[0] - 0.6) < 1e-4);
}

static void test_p18_f10_vec_pearson_corr(void) {
    double a[3] = {1, 2, 3}; double b[3] = {2, 4, 6}; assert(fabs(p18_f10_vec_pearson_corr(a, b, 3) - 1.0) < 1e-4);
}

static void test_p18_f11_vec_z_score(void) {
    double a[3] = {1, 2, 3}; double z[3]; p18_f11_vec_z_score(a, z, 3); assert(fabs(z[1]) < 1e-6);
}

static void test_p18_f12_vec_histogram(void) {
    double a[3] = {1, 2, 3}; size_t bins[2] = {0}; p18_f12_vec_histogram(a, 3, 1, 3, bins, 2); assert(bins[0] + bins[1] == 3);
}

static void test_p18_f13_vec_median(void) {
    double a[3] = {3, 1, 2}; assert(p18_f13_vec_median(a, 3) == 2.0);
}

static void test_p18_f14_vec_quantile(void) {
    double a[3] = {1, 2, 3}; assert(p18_f14_vec_quantile(a, 3, 0.5) >= 1.0);
}

static void test_p18_f15_vec_covariance(void) {
    double a[3] = {1, 2, 3}; double b[3] = {2, 4, 6}; assert(p18_f15_vec_covariance(a, b, 3) > 0.0);
}

static void test_p18_f16_vec_scale_add(void) {
    double s[2] = {1, 2}; double d[2]; p18_f16_vec_scale_add(d, s, 2.0, 1.0, 2); assert(d[0] == 3.0 && d[1] == 5.0);
}

int main(void) {
    test_p18_f01_vec_mean();
    test_p18_f02_vec_variance();
    test_p18_f03_vec_stddev();
    test_p18_f04_vec_min_max();
    test_p18_f05_vec_dot_product();
    test_p18_f06_vec_l1_norm();
    test_p18_f07_vec_l2_norm();
    test_p18_f08_vec_cosine_similarity();
    test_p18_f09_vec_normalize();
    test_p18_f10_vec_pearson_corr();
    test_p18_f11_vec_z_score();
    test_p18_f12_vec_histogram();
    test_p18_f13_vec_median();
    test_p18_f14_vec_quantile();
    test_p18_f15_vec_covariance();
    test_p18_f16_vec_scale_add();
    printf("PASS: test_p18_reuse100 passed.\n");
    return 0;
}
