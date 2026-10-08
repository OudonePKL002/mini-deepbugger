#include "query_component.h"
#include <string.h>
#include <math.h>

P14_NOINLINE bool p14_f02_bm_set_bit(p14_bitmap_t *bm, size_t idx) {
    if (!bm || idx >= bm->num_bits) return false;
    bm->words[idx / 64] |= ((uint64_t)1 << (idx % 64));
    return true;
}

P14_NOINLINE bool p14_f03_bm_clear_bit(p14_bitmap_t *bm, size_t idx) {
    if (!bm || idx >= bm->num_bits) return false;
    bm->words[idx / 64] &= ~((uint64_t)1 << (idx % 64));
    return true;
}

P14_NOINLINE bool p14_f04_bm_test_bit(const p14_bitmap_t *bm, size_t idx) {
    if (!bm || idx >= bm->num_bits) return false;
    return (bm->words[idx / 64] & ((uint64_t)1 << (idx % 64))) != 0;
}

P14_NOINLINE int64_t p14_f08_bm_find_first_zero(const p14_bitmap_t *bm) {
    if (!bm) return -1;
    for (size_t i = 0; i < bm->num_bits; ++i) {
        if (!p14_f04_bm_test_bit(bm, i)) return (int64_t)i;
    }
    return -1;
}

P14_NOINLINE int64_t p14_f09_bm_find_next_set(const p14_bitmap_t *bm, size_t from_idx) {
    if (!bm) return -1;
    for (size_t i = from_idx; i < bm->num_bits; ++i) {
        if (p14_f04_bm_test_bit(bm, i)) return (int64_t)i;
    }
    return -1;
}

P14_NOINLINE void p14_f10_bm_bitwise_and(p14_bitmap_t *dst, const p14_bitmap_t *a, const p14_bitmap_t *b) {
    if (!dst || !a || !b) return;
    dst->num_bits = (a->num_bits < b->num_bits) ? a->num_bits : b->num_bits;
    for (size_t i = 0; i < P14_BM_WORDS; ++i) {
        dst->words[i] = a->words[i] & b->words[i];
    }
}

P14_NOINLINE void p14_f12_bm_bitwise_xor(p14_bitmap_t *dst, const p14_bitmap_t *a, const p14_bitmap_t *b) {
    if (!dst || !a || !b) return;
    dst->num_bits = (a->num_bits < b->num_bits) ? a->num_bits : b->num_bits;
    for (size_t i = 0; i < P14_BM_WORDS; ++i) {
        dst->words[i] = a->words[i] ^ b->words[i];
    }
}

P14_NOINLINE void p14_f14_bm_fill_range(p14_bitmap_t *bm, size_t start, size_t end) {
    if (!bm || start >= bm->num_bits) return;
    if (end > bm->num_bits) end = bm->num_bits;
    for (size_t i = start; i < end; ++i) {
        p14_f02_bm_set_bit(bm, i);
    }
}
