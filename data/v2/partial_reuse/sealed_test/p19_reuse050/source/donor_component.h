#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P18_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P18_NOINLINE double p18_f01_vec_mean(const double *v, size_t n);
P18_NOINLINE void   p18_f04_vec_min_max(const double *v, size_t n, double *out_min, double *out_max);
P18_NOINLINE double p18_f05_vec_dot_product(const double *a, const double *b, size_t n);
P18_NOINLINE double p18_f07_vec_l2_norm(const double *v, size_t n);
P18_NOINLINE bool   p18_f09_vec_normalize(double *v, size_t n);
P18_NOINLINE double p18_f10_vec_pearson_corr(const double *x, const double *y, size_t n);
P18_NOINLINE double p18_f14_vec_quantile(double *v, size_t n, double q);
P18_NOINLINE void   p18_f16_vec_scale_add(double *dst, const double *src, double scale, double bias, size_t n);

#endif /* TARGET_DONOR_COMPONENT_H */
