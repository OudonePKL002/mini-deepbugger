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

P18_NOINLINE double p18_f13_vec_median(double *v, size_t n) {
    if (!v || n == 0) return 0.0;
    sort_double(v, n);
    if (n % 2 == 1) return v[n / 2];
    return (v[n / 2 - 1] + v[n / 2]) / 2.0;
}

P18_NOINLINE double p18_f14_vec_quantile(double *v, size_t n, double q) {
    if (!v || n == 0 || q < 0.0 || q > 1.0) return 0.0;
    sort_double(v, n);
    size_t idx = (size_t)(q * (double)(n - 1));
    return v[idx];
}
