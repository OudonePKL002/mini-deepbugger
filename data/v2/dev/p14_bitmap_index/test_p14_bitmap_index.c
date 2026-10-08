#include <stdio.h>
#include <assert.h>
#include "p14_bitmap_index.h"

int main(void) {
    p14_bitmap_t bm;
    p14_f01_bm_init(&bm, 128);

    assert(p14_f02_bm_set_bit(&bm, 5));
    assert(p14_f02_bm_set_bit(&bm, 70));
    assert(p14_f04_bm_test_bit(&bm, 5));
    assert(p14_f04_bm_test_bit(&bm, 70));
    assert(!p14_f04_bm_test_bit(&bm, 6));

    assert(p14_f06_bm_popcount(&bm) == 2);
    assert(p14_f07_bm_find_first_set(&bm) == 5);
    assert(p14_f09_bm_find_next_set(&bm, 6) == 70);
    assert(p14_f08_bm_find_first_zero(&bm) == 0);

    assert(p14_f05_bm_toggle_bit(&bm, 5));
    assert(!p14_f04_bm_test_bit(&bm, 5));

    p14_bitmap_t bm2;
    p14_f01_bm_init(&bm2, 128);
    p14_f14_bm_fill_range(&bm2, 10, 20);
    assert(p14_f06_bm_popcount(&bm2) == 10);
    p14_f15_bm_clear_range(&bm2, 15, 20);
    assert(p14_f06_bm_popcount(&bm2) == 5);

    p14_bitmap_t a, b;
    p14_f01_bm_init(&a, 64);
    p14_f01_bm_init(&b, 64);
    p14_f02_bm_set_bit(&a, 1);
    p14_f02_bm_set_bit(&a, 2);
    p14_f02_bm_set_bit(&b, 2);
    p14_f02_bm_set_bit(&b, 3);
    // intersection = {2} (1), union = {1, 2, 3} (3) -> 1/3 ~ 333
    uint32_t jacc = p14_f16_bm_jaccard_similarity(&a, &b);
    assert(jacc == 333);

    printf("PASS: p14_bitmap_index unit tests passed.\n");
    return 0;
}
