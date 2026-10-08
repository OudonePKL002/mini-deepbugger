#include "query_component.h"
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

P04_NOINLINE float p04_f11_mat_det_3x3(const p04_mat3_t *a) {
    if (!a) return 0.0f;
    float term1 = a->m[0][0] * p04_f10_mat_det_2x2(a->m[1][1], a->m[1][2], a->m[2][1], a->m[2][2]);
    float term2 = a->m[0][1] * p04_f10_mat_det_2x2(a->m[1][0], a->m[1][2], a->m[2][0], a->m[2][2]);
    float term3 = a->m[0][2] * p04_f10_mat_det_2x2(a->m[1][0], a->m[1][1], a->m[2][0], a->m[2][1]);
    return term1 - term2 + term3;
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

P04_NOINLINE bool p04_f13_mat_is_symmetric(const p04_mat3_t *a, float eps) {
    if (!a) return false;
    for (int r = 0; r < 3; ++r) {
        for (int c = r + 1; c < 3; ++c) {
            if (fabsf(a->m[r][c] - a->m[c][r]) > eps) return false;
        }
    }
    return true;
}

P04_NOINLINE bool p04_f14_mat_lu_decompose_3x3(const p04_mat3_t *a, p04_mat3_t *l, p04_mat3_t *u) {
    if (!a || !l || !u) return false;
    p04_f01_mat_init_zero(l);
    p04_f01_mat_init_zero(u);
    for (int i = 0; i < 3; ++i) {
        for (int k = i; k < 3; ++k) {
            float sum = 0.0f;
            for (int j = 0; j < i; ++j) sum += l->m[i][j] * u->m[j][k];
            u->m[i][k] = a->m[i][k] - sum;
        }
        for (int k = i; k < 3; ++k) {
            if (i == k) {
                l->m[i][i] = 1.0f;
            } else {
                if (fabsf(u->m[i][i]) < 1e-6f) return false;
                float sum = 0.0f;
                for (int j = 0; j < i; ++j) sum += l->m[k][j] * u->m[j][i];
                l->m[k][i] = (a->m[k][i] - sum) / u->m[i][i];
            }
        }
    }
    return true;
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

P04_NOINLINE void p04_f16_mat_hadamard_product(const p04_mat3_t *a, const p04_mat3_t *b, p04_mat3_t *out) {
    if (!a || !b || !out) return;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            out->m[r][c] = a->m[r][c] * b->m[r][c];
        }
    }
}
