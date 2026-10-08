#include "donor_component.h"
#include <string.h>
#include <math.h>

P04_NOINLINE void p04_f04_mat_sub(const p04_mat3_t *a, const p04_mat3_t *b, p04_mat3_t *out) {
    if (!a || !b || !out) return;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            out->m[r][c] = a->m[r][c] - b->m[r][c];
        }
    }
}

P04_NOINLINE void p04_f05_mat_scale(const p04_mat3_t *a, float s, p04_mat3_t *out) {
    if (!a || !out) return;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            out->m[r][c] = a->m[r][c] * s;
        }
    }
}

P04_NOINLINE void p04_f06_mat_transpose(const p04_mat3_t *a, p04_mat3_t *out) {
    if (!a || !out) return;
    p04_mat3_t tmp;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            tmp.m[r][c] = a->m[c][r];
        }
    }
    *out = tmp;
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

P04_NOINLINE void p04_f08_mat_vec_mul(const p04_mat3_t *a, const p04_vec3_t *x, p04_vec3_t *out) {
    if (!a || !x || !out) return;
    for (int r = 0; r < 3; ++r) {
        out->v[r] = a->m[r][0] * x->v[0] + a->m[r][1] * x->v[1] + a->m[r][2] * x->v[2];
    }
}

P04_NOINLINE float p04_f09_mat_trace(const p04_mat3_t *a) {
    if (!a) return 0.0f;
    return a->m[0][0] + a->m[1][1] + a->m[2][2];
}

P04_NOINLINE float p04_f10_mat_det_2x2(float a11, float a12, float a21, float a22) {
    return (a11 * a22) - (a12 * a21);
}

P04_NOINLINE void p04_f16_mat_hadamard_product(const p04_mat3_t *a, const p04_mat3_t *b, p04_mat3_t *out) {
    if (!a || !b || !out) return;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            out->m[r][c] = a->m[r][c] * b->m[r][c];
        }
    }
}
