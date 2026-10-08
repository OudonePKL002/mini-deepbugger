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
P14_NOINLINE bool     p14_f02_bm_set_bit(p14_bitmap_t *bm, size_t idx);
P14_NOINLINE bool     p14_f03_bm_clear_bit(p14_bitmap_t *bm, size_t idx);
P14_NOINLINE bool     p14_f04_bm_test_bit(const p14_bitmap_t *bm, size_t idx);
P14_NOINLINE int64_t  p14_f08_bm_find_first_zero(const p14_bitmap_t *bm);
P14_NOINLINE int64_t  p14_f09_bm_find_next_set(const p14_bitmap_t *bm, size_t from_idx);
P14_NOINLINE void     p14_f10_bm_bitwise_and(p14_bitmap_t *dst, const p14_bitmap_t *a, const p14_bitmap_t *b);
P14_NOINLINE void     p14_f12_bm_bitwise_xor(p14_bitmap_t *dst, const p14_bitmap_t *a, const p14_bitmap_t *b);
P14_NOINLINE void     p14_f14_bm_fill_range(p14_bitmap_t *bm, size_t start, size_t end);

#endif /* TARGET_QUERY_COMPONENT_H */
