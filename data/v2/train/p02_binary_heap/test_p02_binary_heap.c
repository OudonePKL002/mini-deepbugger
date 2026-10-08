#include <stdio.h>
#include <assert.h>
#include "p02_binary_heap.h"

int main(void) {
    p02_heap_t h;
    p02_f01_heap_init(&h);
    assert(h.size == 0);

    p02_f04_heap_push(&h, 45);
    p02_f04_heap_push(&h, 12);
    p02_f04_heap_push(&h, 78);
    p02_f04_heap_push(&h, 3);
    assert(p02_f14_heap_is_valid(&h));

    int32_t top = 0;
    assert(p02_f06_heap_peek(&h, &top) && top == 3);
    assert(p02_f05_heap_pop_min(&h, &top) && top == 3);
    assert(p02_f06_heap_peek(&h, &top) && top == 12);

    int32_t arr[6] = {90, 10, 40, 70, 20, 60};
    p02_f07_heapify_array(&h, arr, 6);
    assert(p02_f14_heap_is_valid(&h));

    int32_t sorted[6] = {90, 10, 40, 70, 20, 60};
    p02_f08_heapsort_asc(sorted, 6);
    assert(sorted[0] == 10 && sorted[5] == 90);

    int32_t old = 0;
    p02_f09_heap_replace(&h, 5, &old);
    assert(p02_f14_heap_is_valid(&h));

    p02_f10_heap_delete_at(&h, 1);
    assert(p02_f14_heap_is_valid(&h));

    p02_f11_heap_increase_key(&h, 0, 50);
    p02_f12_heap_decrease_key(&h, 2, 10);
    assert(p02_f14_heap_is_valid(&h));

    p02_heap_t h2;
    p02_f01_heap_init(&h2);
    p02_f04_heap_push(&h2, 1);
    p02_f04_heap_push(&h2, 2);
    assert(p02_f13_heap_merge(&h, &h2));
    assert(p02_f14_heap_is_valid(&h));

    int32_t raw[5] = {34, 12, 89, 5, 23};
    int32_t k2 = p02_f15_heap_kth_smallest(raw, 5, 2);
    assert(k2 == 12);

    p02_f16_heap_clear(&h);
    assert(h.size == 0);

    printf("PASS: p02_binary_heap unit tests passed.\n");
    return 0;
}
