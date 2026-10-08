#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P14_NOINLINE __attribute__((noinline))
#define P14_BM_WORDS 8  // 8 * 64 = 512 bits

typedef struct {
    uint64_t words[P14_BM_WORDS];
    size_t num_bits;
} p14_bitmap_t;

/* Benchmark Function Prototypes */
P14_NOINLINE bool     p14_f05_bm_toggle_bit(p14_bitmap_t *bm, size_t idx);
P14_NOINLINE size_t   p14_f06_bm_popcount(const p14_bitmap_t *bm);
P14_NOINLINE void     p14_f11_bm_bitwise_or(p14_bitmap_t *dst, const p14_bitmap_t *a, const p14_bitmap_t *b);
P14_NOINLINE void     p14_f12_bm_bitwise_xor(p14_bitmap_t *dst, const p14_bitmap_t *a, const p14_bitmap_t *b);

#endif /* TARGET_QUERY_COMPONENT_H */
