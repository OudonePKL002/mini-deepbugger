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

static void test_p21_f03_log_get_by_seq(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.next_seq = 2; log.entries[0].seq = 1; log.entries[0].level = P21_LOG_INFO; log.entries[0].timestamp = 100; strcpy(log.entries[0].msg, "test"); p21_log_entry_t ent; assert(p21_f03_log_get_by_seq(&log, 1, &ent));
}

static void test_p21_f04_log_get_latest(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.next_seq = 2; log.entries[0].seq = 1; log.entries[0].level = P21_LOG_INFO; log.entries[0].timestamp = 100; strcpy(log.entries[0].msg, "test"); p21_log_entry_t ent; assert(p21_f04_log_get_latest(&log, &ent));
}

static void test_p21_f05_log_get_oldest(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.next_seq = 2; log.entries[0].seq = 1; log.entries[0].level = P21_LOG_INFO; log.entries[0].timestamp = 100; strcpy(log.entries[0].msg, "test"); p21_log_entry_t ent; assert(p21_f05_log_get_oldest(&log, &ent));
}

static void test_p21_f07_log_filter_level(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].level = P21_LOG_INFO; p21_log_entry_t ents[1]; assert(p21_f07_log_filter_level(&log, P21_LOG_INFO, ents, 1) == 1);
}

static void test_p21_f09_log_checksum(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); assert(p21_f09_log_checksum(&log) == 0x55555555U);
}

static void test_p21_f11_rle_compress(void) {
    const uint8_t raw[] = {1, 1, 2}; uint8_t comp[8]; assert(p21_f11_rle_compress(raw, 3, comp, 8) > 0);
}

static void test_p21_f12_rle_decompress(void) {
    const uint8_t comp[4] = {2, 1, 1, 2}; uint8_t dec[4]; assert(p21_f12_rle_decompress(comp, 4, dec, 4) == 3);
}

static void test_p20_f04_ring_available(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); assert(p20_f04_ring_available(&r) == 0);
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

static void test_p20_f13_ring_linearize(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; uint8_t d[2]; assert(p20_f13_ring_linearize(&r, d, 1) == 1 && d[0] == 'A');
}

static void test_p20_f14_ring_high_watermark(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.max_count_seen = 3; assert(p20_f14_ring_high_watermark(&r) == 3);
}

static void test_p20_f15_ring_checksum(void) {
    p20_ring_t r; memset(&r, 0, sizeof(r)); r.buffer[0] = 'A'; r.head = 1; r.tail = 0; r.count = 1; assert(p20_f15_ring_checksum(&r) == 'A');
}

int main(void) {
    test_p21_f01_log_init();
    test_p21_f03_log_get_by_seq();
    test_p21_f04_log_get_latest();
    test_p21_f05_log_get_oldest();
    test_p21_f07_log_filter_level();
    test_p21_f09_log_checksum();
    test_p21_f11_rle_compress();
    test_p21_f12_rle_decompress();
    test_p20_f04_ring_available();
    test_p20_f06_ring_write_byte();
    test_p20_f07_ring_read_byte();
    test_p20_f08_ring_write_slice();
    test_p20_f10_ring_peek_byte();
    test_p20_f13_ring_linearize();
    test_p20_f14_ring_high_watermark();
    test_p20_f15_ring_checksum();
    printf("PASS: test_p21_reuse050 passed.\n");
    return 0;
}
