#include "query_component.h"
#include <string.h>
#include <math.h>

P04_NOINLINE void p04_f02_mat_init_identity(p04_mat3_t *out) {
    if (!out) return;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            out->m[r][c] = (r == c) ? 1.0f : 0.0f;
        }
    }
}

P04_NOINLINE void p04_f03_mat_add(const p04_mat3_t *a, const p04_mat3_t *b, p04_mat3_t *out) {
    if (!a || !b || !out) return;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            out->m[r][c] = a->m[r][c] + b->m[r][c];
        }
    }
}

P04_NOINLINE void p04_f07_mat_mul(const p04_mat3_t *a, const p04_mat3_t *b, p04_mat3_t *out) {
    if (!a || !b || !out) return;
    p04_mat3_t tmp;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            float sum = 0.0f;
            for (int k = 0; k < 3; ++k) {
                sum += a->m[r][k] * b->m[k][c];
            }
            tmp.m[r][c] = sum;
        }
    }
    *out = tmp;
}

P04_NOINLINE float p04_f10_mat_det_2x2(float a11, float a12, float a21, float a22) {
    return (a11 * a22) - (a12 * a21);
}
