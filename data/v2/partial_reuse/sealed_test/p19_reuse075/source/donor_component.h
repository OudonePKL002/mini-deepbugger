#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P18_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P18_NOINLINE double p18_f01_vec_mean(const double *v, size_t n);
P18_NOINLINE double p18_f02_vec_variance(const double *v, size_t n);
P18_NOINLINE double p18_f06_vec_l1_norm(const double *v, size_t n);
P18_NOINLINE void   p18_f16_vec_scale_add(double *dst, const double *src, double scale, double bias, size_t n);

#endif /* TARGET_DONOR_COMPONENT_H */
