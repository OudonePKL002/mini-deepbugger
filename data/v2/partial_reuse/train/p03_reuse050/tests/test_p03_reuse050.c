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

static void test_p03_f02_str_split_delim(void) {
    char toks[4][32]; assert(p03_f02_str_split_delim("a,b,c", ',', toks, 4) == 3);
}

static void test_p03_f03_str_join(void) {
    char toks[2][32] = {"foo", "bar"}; char out[32]; assert(p03_f03_str_join(toks, 2, '-', out, 32) == 7 && strcmp(out, "foo-bar") == 0);
}

static void test_p03_f09_str_to_upper_ascii(void) {
    char s[16] = "hello"; p03_f09_str_to_upper_ascii(s); assert(strcmp(s, "HELLO") == 0);
}

static void test_p03_f10_str_reverse(void) {
    char s[16] = "12345"; p03_f10_str_reverse(s); assert(strcmp(s, "54321") == 0);
}

static void test_p03_f11_str_levenshtein(void) {
    assert(p03_f11_str_levenshtein("kitten", "sitting") == 3);
}

static void test_p03_f14_str_parse_int(void) {
    int32_t val = 0; assert(p03_f14_str_parse_int("-123", &val) == true && val == -123);
}

static void test_p03_f15_str_format_hex(void) {
    uint8_t raw[2] = {0x12, 0xAB}; char buf[8]; assert(p03_f15_str_format_hex(raw, 2, buf, 8) == 4);
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

int main(void) {
    test_p03_f01_str_trim();
    test_p03_f02_str_split_delim();
    test_p03_f03_str_join();
    test_p03_f09_str_to_upper_ascii();
    test_p03_f10_str_reverse();
    test_p03_f11_str_levenshtein();
    test_p03_f14_str_parse_int();
    test_p03_f15_str_format_hex();
    test_p07_f07_slab_mark_free();
    test_p07_f08_slab_find_first_free_bit();
    test_p07_f09_slab_defragment_check();
    test_p07_f11_slab_block_index();
    test_p07_f12_slab_index_to_pointer();
    test_p07_f13_slab_stats_utilization();
    test_p07_f14_slab_audit_integrity();
    test_p07_f15_slab_scrub_pattern();
    printf("PASS: test_p03_reuse050 passed.\n");
    return 0;
}
