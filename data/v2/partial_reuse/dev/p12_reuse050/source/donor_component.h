#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

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
P14_NOINLINE bool     p14_f03_bm_clear_bit(p14_bitmap_t *bm, size_t idx);
P14_NOINLINE bool     p14_f04_bm_test_bit(const p14_bitmap_t *bm, size_t idx);
P14_NOINLINE bool     p14_f05_bm_toggle_bit(p14_bitmap_t *bm, size_t idx);
P14_NOINLINE size_t   p14_f06_bm_popcount(const p14_bitmap_t *bm);
P14_NOINLINE int64_t  p14_f07_bm_find_first_set(const p14_bitmap_t *bm);
P14_NOINLINE void     p14_f10_bm_bitwise_and(p14_bitmap_t *dst, const p14_bitmap_t *a, const p14_bitmap_t *b);
P14_NOINLINE void     p14_f11_bm_bitwise_or(p14_bitmap_t *dst, const p14_bitmap_t *a, const p14_bitmap_t *b);
P14_NOINLINE void     p14_f15_bm_clear_range(p14_bitmap_t *bm, size_t start, size_t end);

#endif /* TARGET_DONOR_COMPONENT_H */
