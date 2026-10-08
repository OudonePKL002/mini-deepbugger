#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p21_f07_log_filter_level(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].level = P21_LOG_INFO; p21_log_entry_t ents[1]; assert(p21_f07_log_filter_level(&log, P21_LOG_INFO, ents, 1) == 1);
}

static void test_p21_f08_log_search_substring(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].seq = 1; strcpy(log.entries[0].msg, "test"); assert(p21_f08_log_search_substring(&log, "test") == 1);
}

static void test_p21_f09_log_checksum(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); assert(p21_f09_log_checksum(&log) == 0x55555555U);
}

static void test_p21_f14_log_truncate_older(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].timestamp = 100; p21_f14_log_truncate_older(&log, 150); assert(log.count == 0);
}

static void test_p20_f01_ring_init(void) {
    p20_ring_t r; r.count = 5; p20_f01_ring_init(&r); assert(r.count == 0 && r.head == 0 && r.tail == 0);
}

static void test_p20_f02_ring_is_empty(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f02_ring_is_empty(&r));
}

static void test_p20_f03_ring_is_full(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(!p20_f03_ring_is_full(&r));
}

static void test_p20_f04_ring_available(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f04_ring_available(&r) == 0);
}

static void test_p20_f05_ring_free_space(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f05_ring_free_space(&r) == P20_RING_CAP);
}

static void test_p20_f06_ring_write_byte(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f06_ring_write_byte(&r, 'A') && r.count == 1);
}

static void test_p20_f07_ring_read_byte(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t b; assert(p20_f07_ring_read_byte(&r, &b) && b == 'A');
}

static void test_p20_f08_ring_write_slice(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); uint8_t s[2] = {1, 2}; assert(p20_f08_ring_write_slice(&r, s, 2) == 2);
}

static void test_p20_f10_ring_peek_byte(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t b; assert(p20_f10_ring_peek_byte(&r, 0, &b) && b == 'A');
}

static void test_p20_f11_ring_discard(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; assert(p20_f11_ring_discard(&r, 1) == 1 && r.count == 0);
}

static void test_p20_f14_ring_high_watermark(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.max_count_seen = 3; assert(p20_f14_ring_high_watermark(&r) == 3);
}

static void test_p20_f16_ring_clear(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; p20_f16_ring_clear(&r); assert(r.count == 0);
}

int main(void) {
    test_p21_f07_log_filter_level();
    test_p21_f08_log_search_substring();
    test_p21_f09_log_checksum();
    test_p21_f14_log_truncate_older();
    test_p20_f01_ring_init();
    test_p20_f02_ring_is_empty();
    test_p20_f03_ring_is_full();
    test_p20_f04_ring_available();
    test_p20_f05_ring_free_space();
    test_p20_f06_ring_write_byte();
    test_p20_f07_ring_read_byte();
    test_p20_f08_ring_write_slice();
    test_p20_f10_ring_peek_byte();
    test_p20_f11_ring_discard();
    test_p20_f14_ring_high_watermark();
    test_p20_f16_ring_clear();
    printf("PASS: test_p21_reuse025 passed.\n");
    return 0;
}
