#include "query_component.h"
#include <string.h>
#include <math.h>

/* Support helper: sort_double */
static void sort_double(double *v, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        double key = v[i];
        int j = (int)i - 1;
        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = key;
    }
}

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

P18_NOINLINE void p18_f04_vec_min_max(const double *v, size_t n, double *out_min, double *out_max) {
    if (!v || n == 0) return;
    double mn = v[0], mx = v[0];
    for (size_t i = 1; i < n; ++i) {
        if (v[i] < mn) mn = v[i];
        if (v[i] > mx) mx = v[i];
    }
    if (out_min) *out_min = mn;
    if (out_max) *out_max = mx;
}

P18_NOINLINE double p18_f05_vec_dot_product(const double *a, const double *b, size_t n) {
    if (!a || !b || n == 0) return 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) sum += a[i] * b[i];
    return sum;
}

P18_NOINLINE double p18_f07_vec_l2_norm(const double *v, size_t n) {
    return sqrt(p18_f05_vec_dot_product(v, v, n));
}

P18_NOINLINE void p18_f12_vec_histogram(const double *v, size_t n, double min_v, double max_v, size_t *bins, size_t num_bins) {
    if (!v || !bins || num_bins == 0 || min_v >= max_v) return;
    for (size_t i = 0; i < num_bins; ++i) bins[i] = 0;
    double range = max_v - min_v;
    for (size_t i = 0; i < n; ++i) {
        if (v[i] >= min_v && v[i] <= max_v) {
            size_t b = (size_t)((v[i] - min_v) / range * (double)num_bins);
            if (b >= num_bins) b = num_bins - 1;
            bins[b]++;
        }
    }
}

P18_NOINLINE double p18_f14_vec_quantile(double *v, size_t n, double q) {
    if (!v || n == 0 || q < 0.0 || q > 1.0) return 0.0;
    sort_double(v, n);
    size_t idx = (size_t)(q * (double)(n - 1));
    return v[idx];
}

P18_NOINLINE void p18_f16_vec_scale_add(double *dst, const double *src, double scale, double bias, size_t n) {
    if (!dst || !src) return;
    for (size_t i = 0; i < n; ++i) {
        dst[i] = src[i] * scale + bias;
    }
}
