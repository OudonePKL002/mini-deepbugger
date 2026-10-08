#include "donor_component.h"
#include <string.h>
#include <math.h>

P18_NOINLINE double p18_f01_vec_mean(const double *v, size_t n) {
    if (!v || n == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) sum += v[i];
    return sum / (double)n;
}

P18_NOINLINE double p18_f02_vec_variance(const double *v, size_t n) {
    if (!v || n <= 1) return 0.0;
    double m = p18_f01_vec_mean(v, n);
    double sum_sq = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double d = v[i] - m;
        sum_sq += d * d;
    }
    return sum_sq / (double)(n - 1);
}

P18_NOINLINE double p18_f06_vec_l1_norm(const double *v, size_t n) {
    if (!v || n == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) sum += fabs(v[i]);
    return sum;
}

P18_NOINLINE void p18_f16_vec_scale_add(double *dst, const double *src, double scale, double bias, size_t n) {
    if (!dst || !src) return;
    for (size_t i = 0; i < n; ++i) {
        dst[i] = src[i] * scale + bias;
    }
}
