#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p21_f01_log_init(void) {
    p21_circ_log_t log; log.count = 5; p21_f01_log_init(&log); assert(log.count == 0 && log.next_seq == 1);
}

static void test_p21_f02_log_append(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.next_seq = 1; assert(p21_f02_log_append(&log, 100, P21_LOG_INFO, "test"));
}

static void test_p21_f04_log_get_latest(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.next_seq = 2; log.entries[0].seq = 1; log.entries[0].level = P21_LOG_INFO; log.entries[0].timestamp = 100; strcpy(log.entries[0].msg, "test"); p21_log_entry_t ent; assert(p21_f04_log_get_latest(&log, &ent));
}

static void test_p21_f05_log_get_oldest(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.next_seq = 2; log.entries[0].seq = 1; log.entries[0].level = P21_LOG_INFO; log.entries[0].timestamp = 100; strcpy(log.entries[0].msg, "test"); p21_log_entry_t ent; assert(p21_f05_log_get_oldest(&log, &ent));
}

static void test_p21_f06_log_count(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; assert(p21_f06_log_count(&log) == 1);
}

static void test_p21_f07_log_filter_level(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].level = P21_LOG_INFO; p21_log_entry_t ents[1]; assert(p21_f07_log_filter_level(&log, P21_LOG_INFO, ents, 1) == 1);
}

static void test_p21_f08_log_search_substring(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].seq = 1; strcpy(log.entries[0].msg, "test"); assert(p21_f08_log_search_substring(&log, "test") == 1);
}

static void test_p21_f09_log_checksum(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); assert(p21_f09_log_checksum(&log) == 0x55555555U);
}

static void test_p21_f12_rle_decompress(void) {
    const uint8_t comp[4] = {2, 1, 1, 2}; uint8_t dec[4]; assert(p21_f12_rle_decompress(comp, 4, dec, 4) == 3);
}

static void test_p21_f13_log_filter_timestamp(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].timestamp = 100; p21_log_entry_t ents[1]; assert(p21_f13_log_filter_timestamp(&log, 50, 150, ents, 1) == 1);
}

static void test_p21_f14_log_truncate_older(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].timestamp = 100; p21_f14_log_truncate_older(&log, 150); assert(log.count == 0);
}

static void test_p21_f16_log_clear(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; p21_f16_log_clear(&log); assert(log.count == 0);
}

static void test_p20_f01_ring_init(void) {
    p20_ring_t r; r.count = 5; p20_f01_ring_init(&r); assert(r.count == 0 && r.head == 0 && r.tail == 0);
}

static void test_p20_f08_ring_write_slice(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); uint8_t s[2] = {1, 2}; assert(p20_f08_ring_write_slice(&r, s, 2) == 2);
}

static void test_p20_f11_ring_discard(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; assert(p20_f11_ring_discard(&r, 1) == 1 && r.count == 0);
}

static void test_p20_f14_ring_high_watermark(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.max_count_seen = 3; assert(p20_f14_ring_high_watermark(&r) == 3);
}

int main(void) {
    test_p21_f01_log_init();
    test_p21_f02_log_append();
    test_p21_f04_log_get_latest();
    test_p21_f05_log_get_oldest();
    test_p21_f06_log_count();
    test_p21_f07_log_filter_level();
    test_p21_f08_log_search_substring();
    test_p21_f09_log_checksum();
    test_p21_f12_rle_decompress();
    test_p21_f13_log_filter_timestamp();
    test_p21_f14_log_truncate_older();
    test_p21_f16_log_clear();
    test_p20_f01_ring_init();
    test_p20_f08_ring_write_slice();
    test_p20_f11_ring_discard();
    test_p20_f14_ring_high_watermark();
    printf("PASS: test_p21_reuse075 passed.\n");
    return 0;
}
