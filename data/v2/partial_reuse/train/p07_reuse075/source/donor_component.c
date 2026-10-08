#include "donor_component.h"
#include <string.h>
#include <math.h>

P04_NOINLINE void p04_f01_mat_init_zero(p04_mat3_t *out) {
    if (!out) return;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            out->m[r][c] = 0.0f;
        }
    }
}

P04_NOINLINE float p04_f09_mat_trace(const p04_mat3_t *a) {
    if (!a) return 0.0f;
    return a->m[0][0] + a->m[1][1] + a->m[2][2];
}

P04_NOINLINE float p04_f12_mat_frobenius_norm(const p04_mat3_t *a) {
    if (!a) return 0.0f;
    float sum_sq = 0.0f;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            sum_sq += a->m[r][c] * a->m[r][c];
        }
    }
    return sqrtf(sum_sq);
}

P04_NOINLINE bool p04_f15_mat_solve_upper_tri(const p04_mat3_t *u, const p04_vec3_t *b, p04_vec3_t *x) {
    if (!u || !b || !x) return false;
    for (int i = 2; i >= 0; --i) {
        if (fabsf(u->m[i][i]) < 1e-6f) return false;
        float sum = 0.0f;
        for (int j = i + 1; j < 3; ++j) {
            sum += u->m[i][j] * x->v[j];
        }
        x->v[i] = (b->v[i] - sum) / u->m[i][i];
    }
    return true;
}
