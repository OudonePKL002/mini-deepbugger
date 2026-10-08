#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P18_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P18_NOINLINE double p18_f01_vec_mean(const double *v, size_t n);
P18_NOINLINE double p18_f02_vec_variance(const double *v, size_t n);
P18_NOINLINE double p18_f03_vec_stddev(const double *v, size_t n);
P18_NOINLINE double p18_f05_vec_dot_product(const double *a, const double *b, size_t n);
P18_NOINLINE double p18_f07_vec_l2_norm(const double *v, size_t n);
P18_NOINLINE bool   p18_f09_vec_normalize(double *v, size_t n);
P18_NOINLINE double p18_f10_vec_pearson_corr(const double *x, const double *y, size_t n);
P18_NOINLINE void   p18_f11_vec_z_score(const double *src, double *dst, size_t n);
P18_NOINLINE void   p18_f12_vec_histogram(const double *v, size_t n, double min_v, double max_v, size_t *bins, size_t num_bins);
P18_NOINLINE double p18_f13_vec_median(double *v, size_t n);
P18_NOINLINE double p18_f15_vec_covariance(const double *x, const double *y, size_t n);
P18_NOINLINE void   p18_f16_vec_scale_add(double *dst, const double *src, double scale, double bias, size_t n);

#endif /* TARGET_QUERY_COMPONENT_H */
