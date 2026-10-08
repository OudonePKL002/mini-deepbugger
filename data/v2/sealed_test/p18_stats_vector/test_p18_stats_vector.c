#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "p18_stats_vector.h"

int main(void) {
    double data[5] = {10.0, 20.0, 30.0, 40.0, 50.0};
    assert(fabs(p18_f01_vec_mean(data, 5) - 30.0) < 1e-6);
    assert(p18_f02_vec_variance(data, 5) > 0.0);
    assert(p18_f03_vec_stddev(data, 5) > 0.0);

    double mn = 0.0, mx = 0.0;
    p18_f04_vec_min_max(data, 5, &mn, &mx);
    assert(mn == 10.0 && mx == 50.0);

    double d2[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double dot = p18_f05_vec_dot_product(data, d2, 5);
    assert(dot > 0.0);

    assert(p18_f06_vec_l1_norm(data, 5) == 150.0);
    assert(p18_f07_vec_l2_norm(data, 5) > 0.0);

    double cos_sim = p18_f08_vec_cosine_similarity(data, d2, 5);
    assert(fabs(cos_sim - 1.0) < 1e-4);

    double z[5];
    p18_f11_vec_z_score(data, z, 5);
    assert(fabs(p18_f01_vec_mean(z, 5)) < 1e-6);

    double copy[5] = {50.0, 10.0, 40.0, 20.0, 30.0};
    assert(p18_f13_vec_median(copy, 5) == 30.0);

    printf("PASS: p18_stats_vector unit tests passed.\n");
    return 0;
}
