#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P11_NOINLINE __attribute__((noinline))
#define P11_FP_SHIFT 16
#define P11_FP_ONE   (1 << P11_FP_SHIFT)
#define P11_FP_PI    205887  // 3.14159265 in Q16.16

typedef int32_t p11_q16_t;

/* Benchmark Function Prototypes */
P11_NOINLINE p11_q16_t p11_f01_fp_from_int(int32_t val);
P11_NOINLINE p11_q16_t p11_f04_fp_sub(p11_q16_t a, p11_q16_t b);
P11_NOINLINE p11_q16_t p11_f12_fp_cos_cordic(p11_q16_t theta);
P11_NOINLINE p11_q16_t p11_f15_fp_clamp(p11_q16_t val, p11_q16_t min_v, p11_q16_t max_v);

#endif /* TARGET_QUERY_COMPONENT_H */
