#include "query_component.h"
#include <string.h>
#include <math.h>

P11_NOINLINE int32_t p11_f02_fp_to_int(p11_q16_t val) {
    return val >> P11_FP_SHIFT;
}

P11_NOINLINE p11_q16_t p11_f03_fp_add(p11_q16_t a, p11_q16_t b) {
    return a + b;
}

P11_NOINLINE p11_q16_t p11_f04_fp_sub(p11_q16_t a, p11_q16_t b) {
    return a - b;
}

P11_NOINLINE p11_q16_t p11_f05_fp_mul(p11_q16_t a, p11_q16_t b) {
    int64_t prod = (int64_t)a * (int64_t)b;
    return (p11_q16_t)(prod >> P11_FP_SHIFT);
}

P11_NOINLINE p11_q16_t p11_f06_fp_div(p11_q16_t a, p11_q16_t b) {
    if (b == 0) return (a >= 0) ? INT32_MAX : INT32_MIN;
    int64_t num = ((int64_t)a) << P11_FP_SHIFT;
    return (p11_q16_t)(num / b);
}

P11_NOINLINE p11_q16_t p11_f07_fp_abs(p11_q16_t a) {
    return (a < 0) ? -a : a;
}

P11_NOINLINE p11_q16_t p11_f08_fp_sqrt(p11_q16_t a) {
    if (a <= 0) return 0;
    int64_t val = ((int64_t)a) << P11_FP_SHIFT;
    int64_t root = 0;
    int64_t bit = (int64_t)1 << 46;
    while (bit > val) bit >>= 2;
    while (bit != 0) {
        if (val >= root + bit) {
            val -= (root + bit);
            root = (root >> 1) + bit;
        } else {
            root >>= 1;
        }
        bit >>= 2;
    }
    return (p11_q16_t)root;
}

P11_NOINLINE p11_q16_t p11_f14_fp_lerp(p11_q16_t a, p11_q16_t b, p11_q16_t t) {
    // a + (b - a) * t
    return p11_f03_fp_add(a, p11_f05_fp_mul(p11_f04_fp_sub(b, a), t));
}
