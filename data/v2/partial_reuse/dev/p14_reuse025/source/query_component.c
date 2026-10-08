#include "query_component.h"
#include <string.h>
#include <math.h>

/* Support helper: popcount64 */
static inline size_t popcount64(uint64_t x) {
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (x * 0x0101010101010101ULL) >> 56;
}

P14_NOINLINE bool p14_f05_bm_toggle_bit(p14_bitmap_t *bm, size_t idx) {
    if (!bm || idx >= bm->num_bits) return false;
    bm->words[idx / 64] ^= ((uint64_t)1 << (idx % 64));
    return true;
}

P14_NOINLINE size_t p14_f06_bm_popcount(const p14_bitmap_t *bm) {
    if (!bm) return 0;
    size_t count = 0;
    size_t words = (bm->num_bits + 63) / 64;
    for (size_t i = 0; i < words; ++i) {
        uint64_t w = bm->words[i];
        if (i == words - 1 && (bm->num_bits % 64) != 0) {
            w &= (((uint64_t)1 << (bm->num_bits % 64)) - 1);
        }
        count += popcount64(w);
    }
    return count;
}

P14_NOINLINE void p14_f11_bm_bitwise_or(p14_bitmap_t *dst, const p14_bitmap_t *a, const p14_bitmap_t *b) {
    if (!dst || !a || !b) return;
    dst->num_bits = (a->num_bits > b->num_bits) ? a->num_bits : b->num_bits;
    for (size_t i = 0; i < P14_BM_WORDS; ++i) {
        dst->words[i] = a->words[i] | b->words[i];
    }
}

P14_NOINLINE void p14_f12_bm_bitwise_xor(p14_bitmap_t *dst, const p14_bitmap_t *a, const p14_bitmap_t *b) {
    if (!dst || !a || !b) return;
    dst->num_bits = (a->num_bits < b->num_bits) ? a->num_bits : b->num_bits;
    for (size_t i = 0; i < P14_BM_WORDS; ++i) {
        dst->words[i] = a->words[i] ^ b->words[i];
    }
}
