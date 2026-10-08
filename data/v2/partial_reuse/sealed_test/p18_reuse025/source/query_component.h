#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P18_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P18_NOINLINE double p18_f01_vec_mean(const double *v, size_t n);
P18_NOINLINE double p18_f02_vec_variance(const double *v, size_t n);
P18_NOINLINE double p18_f13_vec_median(double *v, size_t n);
P18_NOINLINE double p18_f14_vec_quantile(double *v, size_t n, double q);

#endif /* TARGET_QUERY_COMPONENT_H */
