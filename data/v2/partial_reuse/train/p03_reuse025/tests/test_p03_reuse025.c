#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p03_f01_str_trim(void) {
    char s[16] = "  abc  "; assert(p03_f01_str_trim(s) == 3 && strcmp(s, "abc") == 0);
}

static void test_p03_f05_str_ends_with(void) {
    assert(p03_f05_str_ends_with("hello world", "world") == true);
}

static void test_p03_f08_str_to_lower_ascii(void) {
    char s[16] = "HeLLo"; p03_f08_str_to_lower_ascii(s); assert(strcmp(s, "hello") == 0);
}

static void test_p03_f13_str_unescape_c(void) {
    char unesc[16]; assert(p03_f13_str_unescape_c("a\\nb", unesc, 16) > 0);
}

static void test_p07_f01_slab_init(void) {
    p07_slab_t s; p07_f01_slab_init(&s); assert(s.alloc_bitmap == 0 && s.free_count == 32);
}

static void test_p07_f04_slab_available_count(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f04_slab_available_count(&s) == P07_NUM_BLOCKS);
}

static void test_p07_f06_slab_mark_busy(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; p07_f06_slab_mark_busy(&s, 0); assert((s.alloc_bitmap & 1) == 1);
}

static void test_p07_f07_slab_mark_free(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 1; p07_f07_slab_mark_free(&s, 0); assert((s.alloc_bitmap & 1) == 0);
}

static void test_p07_f08_slab_find_first_free_bit(void) {
    assert(p07_f08_slab_find_first_free_bit(0) == 0);
}

static void test_p07_f09_slab_defragment_check(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; s.alloc_bitmap = 0x55; assert(p07_f09_slab_defragment_check(&s) > 0);
}

static void test_p07_f11_slab_block_index(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f11_slab_block_index(&s, s.memory_pool) == 0);
}

static void test_p07_f12_slab_index_to_pointer(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f12_slab_index_to_pointer(&s, 0) == (void*)s.memory_pool);
}

static void test_p07_f13_slab_stats_utilization(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f13_slab_stats_utilization(&s) == 0);
}

static void test_p07_f14_slab_audit_integrity(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; assert(p07_f14_slab_audit_integrity(&s) == true);
}

static void test_p07_f15_slab_scrub_pattern(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; p07_f15_slab_scrub_pattern(&s, 0, 0xAA); assert(s.memory_pool[0] == 0xAA);
}

static void test_p07_f16_slab_compact_scan(void) {
    p07_slab_t s; memset(&s, 0, sizeof(s)); s.total_blocks = P07_NUM_BLOCKS; s.free_count = P07_NUM_BLOCKS; size_t run; assert(p07_f16_slab_compact_scan(&s, &run) == 1 && run == 32);
}

int main(void) {
    test_p03_f01_str_trim();
    test_p03_f05_str_ends_with();
    test_p03_f08_str_to_lower_ascii();
    test_p03_f13_str_unescape_c();
    test_p07_f01_slab_init();
    test_p07_f04_slab_available_count();
    test_p07_f06_slab_mark_busy();
    test_p07_f07_slab_mark_free();
    test_p07_f08_slab_find_first_free_bit();
    test_p07_f09_slab_defragment_check();
    test_p07_f11_slab_block_index();
    test_p07_f12_slab_index_to_pointer();
    test_p07_f13_slab_stats_utilization();
    test_p07_f14_slab_audit_integrity();
    test_p07_f15_slab_scrub_pattern();
    test_p07_f16_slab_compact_scan();
    printf("PASS: test_p03_reuse025 passed.\n");
    return 0;
}
