#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p02_f01_heap_init(void) {
    p02_heap_t h; p02_f01_heap_init(&h); assert(h.size == 0 && h.capacity == P02_MAX_CAP);
}

static void test_p02_f02_heap_sift_up(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 5; p02_f02_heap_sift_up(&h, 1); assert(h.data[0] == 5);
}

static void test_p02_f03_heap_sift_down(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 5; p02_f03_heap_sift_down(&h, 0); assert(h.data[0] == 5);
}

static void test_p02_f04_heap_push(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 0; assert(p02_f04_heap_push(&h, 42) && h.size == 1);
}

static void test_p02_f05_heap_pop_min(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 1; h.data[0] = 42; int32_t val; assert(p02_f05_heap_pop_min(&h, &val) && val == 42);
}

static void test_p02_f06_heap_peek(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 1; h.data[0] = 99; int32_t val = 0; assert(p02_f06_heap_peek(&h, &val) && val == 99);
}

static void test_p02_f07_heapify_array(void) {
    int32_t arr[3] = {3, 1, 2}; p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; p02_f07_heapify_array(&h, arr, 3); assert(h.size == 3);
}

static void test_p02_f08_heapsort_asc(void) {
    int32_t arr[4] = {40, 10, 30, 20}; p02_f08_heapsort_asc(arr, 4); assert(arr[0] == 10 && arr[3] == 40);
}

static void test_p02_f09_heap_replace(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 20; int32_t old_val = 0; assert(p02_f09_heap_replace(&h, 30, &old_val) && old_val == 10);
}

static void test_p02_f10_heap_delete_at(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 3; h.data[0] = 10; h.data[1] = 20; h.data[2] = 30; assert(p02_f10_heap_delete_at(&h, 1) && h.size == 2);
}

static void test_p02_f11_heap_increase_key(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 20; assert(p02_f11_heap_increase_key(&h, 0, 50)); assert(h.data[0] == 20 && h.data[1] == 60);
}

static void test_p02_f12_heap_decrease_key(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 20; assert(p02_f12_heap_decrease_key(&h, 1, 15)); assert(h.data[0] == 5 && h.data[1] == 10);
}

static void test_p02_f13_heap_merge(void) {
    p02_heap_t h1, h2; memset(&h1, 0, sizeof(h1)); memset(&h2, 0, sizeof(h2)); h1.capacity = P02_MAX_CAP; h2.capacity = P02_MAX_CAP; h1.size = 1; h1.data[0] = 10; h2.size = 1; h2.data[0] = 5; assert(p02_f13_heap_merge(&h1, &h2) && h1.size == 2 && h1.data[0] == 5);
}

static void test_p02_f14_heap_is_valid(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 3; h.data[0] = 10; h.data[1] = 20; h.data[2] = 30; assert(p02_f14_heap_is_valid(&h));
}

static void test_p02_f15_heap_kth_smallest(void) {
    int32_t arr[5] = {34, 12, 89, 5, 23}; assert(p02_f15_heap_kth_smallest(arr, 5, 2) == 12);
}

static void test_p02_f16_heap_clear(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 5; p02_f16_heap_clear(&h); assert(h.size == 0);
}

int main(void) {
    test_p02_f01_heap_init();
    test_p02_f02_heap_sift_up();
    test_p02_f03_heap_sift_down();
    test_p02_f04_heap_push();
    test_p02_f05_heap_pop_min();
    test_p02_f06_heap_peek();
    test_p02_f07_heapify_array();
    test_p02_f08_heapsort_asc();
    test_p02_f09_heap_replace();
    test_p02_f10_heap_delete_at();
    test_p02_f11_heap_increase_key();
    test_p02_f12_heap_decrease_key();
    test_p02_f13_heap_merge();
    test_p02_f14_heap_is_valid();
    test_p02_f15_heap_kth_smallest();
    test_p02_f16_heap_clear();
    printf("PASS: test_p05_reuse000 passed.\n");
    return 0;
}
