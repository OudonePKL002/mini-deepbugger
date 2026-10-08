#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p15_f03_fnv1_64(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f03_fnv1_64(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f09_super_fast_hash(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f09_super_fast_hash(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f11_dek_hash(void) {
    assert(p15_f11_dek_hash("TestString") != 0);
}

static void test_p15_f15_hash_combine64(void) {
    assert(p15_f15_hash_combine64(10, 20) != 0);
}

static void test_p21_f02_log_append(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.next_seq = 1; assert(p21_f02_log_append(&log, 100, P21_LOG_INFO, "test"));
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

static void test_p21_f10_log_verify_checksum(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); assert(p21_f10_log_verify_checksum(&log, 0x55555555U));
}

static void test_p21_f11_rle_compress(void) {
    const uint8_t raw[] = {1, 1, 2}; uint8_t comp[8]; assert(p21_f11_rle_compress(raw, 3, comp, 8) > 0);
}

static void test_p21_f12_rle_decompress(void) {
    const uint8_t comp[4] = {2, 1, 1, 2}; uint8_t dec[4]; assert(p21_f12_rle_decompress(comp, 4, dec, 4) == 3);
}

static void test_p21_f15_log_export_stats(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].level = P21_LOG_INFO; size_t stats[4]; p21_f15_log_export_stats(&log, stats); assert(stats[P21_LOG_INFO] == 1);
}

int main(void) {
    test_p15_f03_fnv1_64();
    test_p15_f09_super_fast_hash();
    test_p15_f11_dek_hash();
    test_p15_f15_hash_combine64();
    test_p21_f02_log_append();
    test_p21_f03_log_get_by_seq();
    test_p21_f04_log_get_latest();
    test_p21_f05_log_get_oldest();
    test_p21_f06_log_count();
    test_p21_f07_log_filter_level();
    test_p21_f08_log_search_substring();
    test_p21_f09_log_checksum();
    test_p21_f10_log_verify_checksum();
    test_p21_f11_rle_compress();
    test_p21_f12_rle_decompress();
    test_p21_f15_log_export_stats();
    printf("PASS: test_p15_reuse025 passed.\n");
    return 0;
}
