#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p15_f01_fnv1_32(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f01_fnv1_32(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f02_fnv1a_32(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f02_fnv1a_32(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f03_fnv1_64(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f03_fnv1_64(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f04_fnv1a_64(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f04_fnv1a_64(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f09_super_fast_hash(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f09_super_fast_hash(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f14_crc24_ble(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f14_crc24_ble(msg, sizeof(msg)-1, 0x555555) != 0);
}

static void test_p15_f15_hash_combine64(void) {
    assert(p15_f15_hash_combine64(10, 20) != 0);
}

static void test_p15_f16_checksum_parity_byte(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f16_checksum_parity_byte(msg, sizeof(msg)-1) != 0);
}

static void test_p21_f01_log_init(void) {
    p21_circ_log_t log; log.count = 5; p21_f01_log_init(&log); assert(log.count == 0 && log.next_seq == 1);
}

static void test_p21_f02_log_append(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.next_seq = 1; assert(p21_f02_log_append(&log, 100, P21_LOG_INFO, "test"));
}

static void test_p21_f04_log_get_latest(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.next_seq = 2; log.entries[0].seq = 1; log.entries[0].level = P21_LOG_INFO; log.entries[0].timestamp = 100; strcpy(log.entries[0].msg, "test"); p21_log_entry_t ent; assert(p21_f04_log_get_latest(&log, &ent));
}

static void test_p21_f07_log_filter_level(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].level = P21_LOG_INFO; p21_log_entry_t ents[1]; assert(p21_f07_log_filter_level(&log, P21_LOG_INFO, ents, 1) == 1);
}

static void test_p21_f08_log_search_substring(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].seq = 1; strcpy(log.entries[0].msg, "test"); assert(p21_f08_log_search_substring(&log, "test") == 1);
}

static void test_p21_f13_log_filter_timestamp(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].timestamp = 100; p21_log_entry_t ents[1]; assert(p21_f13_log_filter_timestamp(&log, 50, 150, ents, 1) == 1);
}

static void test_p21_f15_log_export_stats(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; log.head = 1; log.entries[0].level = P21_LOG_INFO; size_t stats[4]; p21_f15_log_export_stats(&log, stats); assert(stats[P21_LOG_INFO] == 1);
}

static void test_p21_f16_log_clear(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; p21_f16_log_clear(&log); assert(log.count == 0);
}

int main(void) {
    test_p15_f01_fnv1_32();
    test_p15_f02_fnv1a_32();
    test_p15_f03_fnv1_64();
    test_p15_f04_fnv1a_64();
    test_p15_f09_super_fast_hash();
    test_p15_f14_crc24_ble();
    test_p15_f15_hash_combine64();
    test_p15_f16_checksum_parity_byte();
    test_p21_f01_log_init();
    test_p21_f02_log_append();
    test_p21_f04_log_get_latest();
    test_p21_f07_log_filter_level();
    test_p21_f08_log_search_substring();
    test_p21_f13_log_filter_timestamp();
    test_p21_f15_log_export_stats();
    test_p21_f16_log_clear();
    printf("PASS: test_p15_reuse050 passed.\n");
    return 0;
}
