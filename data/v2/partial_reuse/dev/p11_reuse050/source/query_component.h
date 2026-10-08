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
P11_NOINLINE int32_t   p11_f02_fp_to_int(p11_q16_t val);
P11_NOINLINE p11_q16_t p11_f03_fp_add(p11_q16_t a, p11_q16_t b);
P11_NOINLINE p11_q16_t p11_f04_fp_sub(p11_q16_t a, p11_q16_t b);
P11_NOINLINE p11_q16_t p11_f05_fp_mul(p11_q16_t a, p11_q16_t b);
P11_NOINLINE p11_q16_t p11_f06_fp_div(p11_q16_t a, p11_q16_t b);
P11_NOINLINE p11_q16_t p11_f07_fp_abs(p11_q16_t a);
P11_NOINLINE p11_q16_t p11_f08_fp_sqrt(p11_q16_t a);
P11_NOINLINE p11_q16_t p11_f14_fp_lerp(p11_q16_t a, p11_q16_t b, p11_q16_t t);

#endif /* TARGET_QUERY_COMPONENT_H */
